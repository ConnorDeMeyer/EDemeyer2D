#pragma once
//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <map>

#include "Structs.h"

class Scene;
class IObject;

//-----------------------------------------------------
// Defines
//-----------------------------------------------------

#define ENGINE GameEngine::GetInstance()

//-----------------------------------------------------
// GameEngine Class									
//-----------------------------------------------------
class GameEngine final
{
private:
	GameEngine();		// Constructor
	~GameEngine();		// Destructor

	// -------------------------
	// Copy/move constructors and assignment operators
	// -------------------------    
	GameEngine(const GameEngine& other)					= delete;
	GameEngine(GameEngine&& other) noexcept				= delete;
	GameEngine& operator=(const GameEngine& other)		= delete;
	GameEngine& operator=(GameEngine&& other) noexcept	= delete;

public:
	//-------------------------------------------------
	// Static functions						
	//-------------------------------------------------
	static GameEngine* GetInstance();
	static void DeleteInstance();

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	void Initialize(HINSTANCE hInstance, int nCmdShow);
	void RunGame(class GameInstance* pGameInstance);
	LRESULT HandleEvent(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);

	HINSTANCE GetHInstance() const { return m_hInstance; }
	HWND GetWindowHandle() const { return m_hWindow; }
	const tstring& GetWindowTitle() const { return m_Title; }
	const tstring& GetAppName() const { return m_AppName; }
	int GetWindowWidth() const { return m_WindowSize.x; }
	int GetWindowHeight() const { return m_WindowSize.y; }
	const IVector2& GetWindowSize() const { return m_WindowSize; }
	const IVector2& GetWindowResolution() const{ return m_DisplayResolution; }
	const IVector2& GetClientSize() const { return m_ClientSize; }
	float GetDeltaTime() const { return float(m_DeltaTime); }
	double GetDeltaTimeD() const { return m_DeltaTime; }

	void SetWindowTitle(const tstring& text);
	void SetWindowWidth(int width);
	void SetWindowheight(int height);
	void SetWindowPos(const IVector2& pos);
	void SetWindowSize(const IVector2& size);
	void SetWindowResolution(int width, int height);
	void SetAutomaticResolutionChange(bool value) { m_bAutoChangeResolution = value; }

	void Quit(int returnValue = 0);

	//void SetFullscreen();
	//void SetWindowed();
	//void ShowCursor(bool value);
	//

	GameInstance* GetGameInstance() const { return m_pGameInstance; }
	Scene* GetCurrentScene() const;

private:
	//-------------------------------------------------
	// Static datamembers								
	//-------------------------------------------------
	static GameEngine* pInstance;

	//-------------------------------------------------
	// Private member functions								
	//-------------------------------------------------
	void UpdateWindowDimensions();
	void UpdateDisplayDimensions(int x, int y);

	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	
	// Window data members
	HINSTANCE m_hInstance = nullptr;
	HWND m_hWindow = nullptr;
	tstring m_AppName;
	tstring m_Title;
	IVector2 m_WindowSize			= {};
	IVector2 m_ClientSize			= {};
	int m_WindowPosX				= {};
	int m_WindowPosY				= {};
	IVector2 m_DisplayResolution	= {};
	bool m_IsFullscreen				= {};
	bool m_bAutoChangeResolution	= {};

	// Time & frame rate data members
	TimeStamp m_CurrentTime;
	double m_DeltaTime				= 0.0;
	double m_TargetFps				= 1.0 / 60.0;
	double m_MinimumFps				= 1.0 / 30.0;
	double m_TimeBuildUp			= 0.0;
	double m_TimeDilation			= 1.f;
	
	class GameInstance* m_pGameInstance{};


};

//---------------------------
// Static datamembers Initialization
//---------------------------



