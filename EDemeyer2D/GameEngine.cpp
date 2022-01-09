//---------------------------
// Includes
//---------------------------
#include "GameEngine.h"
#include "Resource.h"
#include "IObject.h"
#include "Structs.h"
#include "Scene.h"

#include <shobjidl.h>

#include "framework.h"
#include "GameInstance.h"

#include <cassert>
#include <thread>
#include <string>

GameEngine* GameEngine::pInstance = nullptr;

//---------------------------
// Static Functions
//---------------------------

GameEngine* GameEngine::GetInstance()
{
    if (!pInstance) pInstance = new GameEngine();
    return pInstance;
}

void GameEngine::DeleteInstance()
{
    SafeDelete(pInstance);
}

//---------------------------
// WIN32 functions
//---------------------------

LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // Route all Windows messages to the game engine
    return ENGINE->HandleEvent(hWindow, msg, wParam, lParam);
}

//---------------------------
// Constructor(s) & Destructor
//---------------------------

GameEngine::GameEngine()
{
    
}

GameEngine::~GameEngine()
{
    delete m_pGameInstance;

    InputManager::Destroy();
    DrawingManager::Destroy();
    ResourceManager::Destroy();
    PhysicsManager::Destroy();

}

void GameEngine::Initialize(HINSTANCE hInstance, int nCmdShow)
{
    // Return if engine is already initialized
    if (m_hInstance) return;

    // DataMembers
    m_hInstance = hInstance;
    m_AppName = _T("EDemeyer2D");
    m_Title =   _T("EDemeyer2D");

    // Register the window class (= choose the type of window you want)
    WNDCLASSEXW wcex;

    wcex.cbSize             = sizeof(WNDCLASSEX);
    wcex.style              = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc        = WndProc;
    wcex.cbClsExtra         = 0;
    wcex.cbWndExtra         = 0;
    wcex.hInstance          = m_hInstance;
    wcex.hIcon              = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor            = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground      = (HBRUSH)(CreateSolidBrush(RGB(255, 255, 255)));
    wcex.lpszMenuName       = nullptr;
    wcex.lpszClassName      = m_AppName.c_str();
    wcex.hIconSm            = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    RegisterClassExW(&wcex);

    m_DisplayResolution = { 224,256 };

    // Create the window, store the handle, check if all went well
    m_hWindow = CreateWindow(  m_AppName.c_str(),
                               m_Title.c_str(),        // window title
                               WS_OVERLAPPEDWINDOW,    // window style
                               CW_USEDEFAULT,          // default x
                               CW_USEDEFAULT,          // default y
                               m_DisplayResolution.x,  // default width
                               m_DisplayResolution.y,  // default height
                               nullptr,
                               nullptr,
                               m_hInstance,
                               nullptr);

    if (!m_hWindow)
    {
        MessageBox(NULL, _T("Failure to create program window"), _T("Error"), MB_ICONERROR);
        return;
    }

    // Set the window to show
    ShowWindow(m_hWindow, nCmdShow);	    // set the window to show mode
    UpdateWindow(m_hWindow);	            // update 

    UpdateWindowDimensions();

    // Initialize Singletons
    DRAW;
    INPUT;
    RESOURCES;
    PHYSICS;
}

void GameEngine::RunGame(GameInstance* pGameInstance)
{
    // set current time
    m_CurrentTime = std::chrono::high_resolution_clock::now();

    m_pGameInstance = pGameInstance;

    std::srand(unsigned int(std::time(nullptr)));

    // load keyboard shortcuts, start the Windows message loop and game loop
    HACCEL hAccelTable = LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDC_EDEMEYER2D));
    MSG msg;
    while (true)
    {
        // Check for messages
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                // Terminate the game and windows message loop if QUIT is called
                if (msg.message == WM_QUIT) break;

                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
            // Game Loop Here
        {
            // Frame rate control
            auto previousTime = m_CurrentTime;
            m_CurrentTime = std::chrono::high_resolution_clock::now();
            double deltaTime = std::chrono::duration<double>(m_CurrentTime - previousTime).count();

            m_DeltaTime = min(max(deltaTime, m_TargetFps), m_MinimumFps);

            Scene* scene = m_pGameInstance->GetCurrentScene();


            if (scene) {

                // Game input events
                INPUT->Update(float(m_DeltaTime));

                // Update before collision tests

                // Collision/trigger tests
                PHYSICS->PhysicsUpdate();

                // Game Update
                scene->Update(float(m_DeltaTime));

                // Game drawing

                DRAW->BeginDraw();
                DRAW->ClearBackground();
                scene->Draw(FVector2{ float(m_DisplayResolution.x), float(m_DisplayResolution.y) });
                HRESULT hr =  DRAW->EndDraw();
                assert(SUCCEEDED(hr));


                // Destroy objects that are flagged to be destroyed
                scene->DestroyFlaggedObjects();
            }
            // Clear key inputs
            INPUT->ClearDownUpKeys();
            
            auto afterGameLoopTime = std::chrono::high_resolution_clock::now();
            auto gameLoopTime = std::chrono::duration<double>(afterGameLoopTime - m_CurrentTime).count();
            auto timeToWait = m_TargetFps - gameLoopTime;
            if (timeToWait > 0)
                std::this_thread::sleep_for(std::chrono::duration<double>(timeToWait));
        }

        //WaitMessage();
    }

}

LRESULT GameEngine::HandleEvent(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        // Paint
    case WM_PAINT:
        ValidateRect(m_hWindow, NULL);
        return 0;

        // Inputs
    case WM_MOUSEMOVE:
        INPUT->MoveMouse(wParam, lParam);
        return 0;
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        INPUT->KeyDown(wParam, lParam);
        return 0;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        INPUT->KeyUp(wParam, lParam);
        return 0;
    case WM_LBUTTONDOWN:
        INPUT->KeyDown(InputKeys::MouseLeft);
        return 0;
    case WM_LBUTTONUP:
        INPUT->KeyUp(InputKeys::MouseLeft);
        return 0;
    case WM_RBUTTONDOWN:
        INPUT->KeyDown(InputKeys::MouseRight);
        return 0;
    case WM_RBUTTONUP:
        INPUT->KeyUp(InputKeys::MouseRight);
        return 0;
    case WM_MBUTTONDOWN:
        INPUT->KeyDown(InputKeys::MouseMiddle);
        return 0;
    case WM_MBUTTONUP:
        INPUT->KeyUp(InputKeys::MouseMiddle);
        return 0;
    case WM_MOUSEWHEEL:
        INPUT->MoveScrollWheel(GET_WHEEL_DELTA_WPARAM(wParam));
        return 0;
    case WM_KILLFOCUS:
        INPUT->ClearInputs();
        return 0;

        // Window
    case WM_CREATE:

        return 0;
    case WM_SIZING:
        UpdateWindowDimensions();
        return true;
    case WM_SIZE:
    case WM_WINDOWPOSCHANGED:
    case WM_MOVE:
        UpdateWindowDimensions();
        return 0;
    case WM_DISPLAYCHANGE:
        UpdateDisplayDimensions(LOWORD(lParam), HIWORD(lParam));
        return 0;
    case WM_DESTROY:
    case WM_CLOSE:
        if (hWindow == m_hWindow) {
            GameEngine::DeleteInstance();
            PostQuitMessage(0);
            return 0;
        }
        else return DefWindowProc(hWindow, msg, wParam, lParam);
    default:
        return DefWindowProc(hWindow, msg, wParam, lParam);
    }
}

void GameEngine::SetWindowTitle(const tstring& text)
{
    SetWindowText(m_hWindow, text.c_str());
    m_Title = text;
}

void GameEngine::SetWindowWidth(int width)
{
    MoveWindow(m_hWindow, m_WindowPosX, m_WindowPosY, width, m_WindowSize.y, false);
}

void GameEngine::SetWindowheight(int height)
{
    MoveWindow(m_hWindow, m_WindowPosX, m_WindowPosY, m_WindowSize.x, height, false);
}

void GameEngine::SetWindowPos(const IVector2& pos)
{
    MoveWindow(m_hWindow, pos.x, pos.y, m_WindowSize.x, m_WindowSize.y, false);
}

void GameEngine::SetWindowSize(const IVector2& size)
{
    MoveWindow(m_hWindow, m_WindowPosX, m_WindowPosY, size.x, size.y, false);
}

void GameEngine::SetWindowResolution(int width, int height)
{
    if (m_DisplayResolution == IVector2{ width, height }) return;
    m_DisplayResolution = { width,height };

    DRAW->GetRenderTarget()->Resize(D2D1::SizeU(width, height));
}

void GameEngine::Quit(int returnValue)
{
    if (m_hWindow) { DestroyWindow(m_hWindow), m_hWindow = nullptr; }
    GameEngine::DeleteInstance();
    PostQuitMessage(returnValue);
}

Scene* GameEngine::GetCurrentScene() const
{
    return m_pGameInstance->GetCurrentScene();
}

void GameEngine::UpdateWindowDimensions()
{
    RECT rc;
    GetWindowRect(m_hWindow, &rc);

    D2D1_SIZE_U windowSize = D2D1::SizeU(
        rc.right - rc.left,
        rc.bottom - rc.top);
    m_WindowSize.x = windowSize.width;
    m_WindowSize.y = windowSize.height;
    m_WindowPosX = rc.left;
    m_WindowPosY = rc.top;

    GetClientRect(m_hWindow, &rc);
    m_ClientSize = { rc.right - rc.left, rc.bottom - rc.top };

    if (m_bAutoChangeResolution) SetWindowResolution(m_ClientSize.x, m_ClientSize.y);
}

void GameEngine::UpdateDisplayDimensions(int x, int y)
{
    m_DisplayResolution.x = x;
    m_DisplayResolution.y = y;
}
