#include "InputManager.h"
#include "framework.h"

InputManager* InputManager::m_Instance = nullptr;

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}


void InputManager::Update(float deltaTime)
{
    BroadCastAllInputs();

    //XInputGetState();

    ClearDownUpKeys();
}

void InputManager::KeyUp(WPARAM wParam, LPARAM lParam)
{
    InputKeys keyCode = InputKeys(UINT8(wParam));

    KeyUp(keyCode);
}

void InputManager::KeyUp(InputKeys key)
{
    // add to keyUp list
    m_KeysUp.push_back(key);

    // remove the key from the keysPressed list if its in there
    size_t size{ m_KeysPressed.size() };
    size_t pos{ size };
    for (size_t i{}; i < size; ++i)
        if (m_KeysPressed[i] == key)
        {
            pos = i;
            break;
        }
    if (pos == size) return;

    m_KeysPressed[pos] = m_KeysPressed.back();
    m_KeysPressed.pop_back();
}

void InputManager::KeyDown(WPARAM wParam, LPARAM lParam)
{
    InputKeys keyCode = InputKeys(UINT8(wParam));

    KeyDown(keyCode);
}

void InputManager::KeyDown(InputKeys key)
{
    // add to keyDown list
    m_KeysDown.push_back(key);

    // if not in pressed list, add it
    if (std::find(m_KeysPressed.begin(), m_KeysPressed.end(), key) == m_KeysPressed.end())
        m_KeysPressed.push_back(key);
}

void InputManager::ClearInputs()
{
    // add the pressed keys to the up keys so that their events can still fire/cancel
    for (InputKeys key : m_KeysPressed) KeyUp(key);
    for (InputKeys key : m_KeysDown) KeyUp(key);

    m_KeysPressed.clear();
    m_KeysDown.clear();
}

void InputManager::MoveScrollWheel(short wheelDelta)
{
    //TODO
}

void InputManager::MoveMouse(WPARAM wParam, LPARAM lParam)
{
    m_CurrentMousePos = IVector2(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

    if (!m_IgnoreLastMovement) {
        m_MouseMovement = m_CurrentMousePos - m_PreviousMousePos;
        m_PreviousMousePos = m_CurrentMousePos;
    }
    else m_IgnoreLastMovement = false;

    if (m_IsMouseLocked) SetMousePosition(m_LockedMousePosition, true);
}

void InputManager::ClearDownUpKeys()
{
    m_KeysDown.clear();
    m_KeysUp.clear();
}

void InputManager::SetMousePosition(const IVector2& pos, bool teleport)
{
    SetCursorPos(pos.x, pos.y);
    m_IgnoreLastMovement = teleport ? true : m_IgnoreLastMovement;
}

void InputManager::BroadCastAllInputs()
{
    auto broadCastKeyInputs = [this](const std::vector<InputKeys>& activatedKeys, std::map<InputKeys, IDelegate<>>& actionDelegates)
    {
        for (auto key : activatedKeys)
        {
            auto it{ actionDelegates.find(key) };
            if (it != actionDelegates.end())
            {
                it->second.BroadCast();
            }
        }
    };

    // Fire events binded to the keys that became down
    broadCastKeyInputs(m_KeysDown, m_KeysDownActions);

    // Fire events binded to the keys that are pressed
    broadCastKeyInputs(m_KeysPressed, m_KeysPressedActions);
    
    // Fire events binded to the keys that became up
    broadCastKeyInputs(m_KeysUp, m_KeysUpActions);

    auto broadCastMouseInputs = [this](const std::vector<InputKeys>& activatedKeys, std::map<InputKeys, IDelegate<int,int>>& actionDelegates)
    {
        for (auto key : activatedKeys)
        {
            auto it{ actionDelegates.find(key) };
            if (it != actionDelegates.end())
            {
                it->second.BroadCast(this->GetMousePosition().x, this->GetMousePosition().y);
            }
        }
    };

    // Fire events binded to the mouse keys that became down
    broadCastMouseInputs(m_KeysDown, m_MouseDownActions);

    // Fire events binded to the mouse keys that are pressed
    broadCastMouseInputs(m_KeysPressed, m_MousePressedActions);

    // Fire events binded to the mouse keys that became up
    broadCastMouseInputs(m_KeysUp, m_MouseUpActions);
}

void InputManager::BindKeyPressedAction(InputKeys key, IBaseClass* pObject, const std::function<void()>& action)
{
    m_KeysPressedActions[key].BindFunction(pObject, action);
}

void InputManager::BindKeyUpAction(InputKeys key, IBaseClass* pObject, const std::function<void()>& action)
{
    m_KeysUpActions[key].BindFunction(pObject, action);
}

void InputManager::BindKeyDownAction(InputKeys key, IBaseClass* pObject, const std::function<void()>& action)
{
    m_KeysDownActions[key].BindFunction(pObject, action);
}

void InputManager::BindMouseDownAction(InputKeys mouseKey, IBaseClass* pObject, const std::function<void(int, int)>& action)
{
    m_MouseDownActions[mouseKey].BindFunction(pObject, action);
}

void InputManager::BindMouseUpAction(InputKeys mouseKey, IBaseClass* pObject, const std::function<void(int, int)>& action)
{
    m_MouseUpActions[mouseKey].BindFunction(pObject, action);
}

void InputManager::BindMousePressedAction(InputKeys mouseKey, IBaseClass* pObject, const std::function<void(int, int)>& action)
{
    m_MousePressedActions[mouseKey].BindFunction(pObject, action);
}
