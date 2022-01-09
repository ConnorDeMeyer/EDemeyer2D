#pragma once

#include "Structs.h"
#include <Xinput.h>
#include <vector>
#include <map>
#include <functional>
#include <memory>
#include <list>
#include "Delegates.h"

#define INPUT InputManager::GetInstance()

enum class InputKeys : UINT8
{
	MouseLeft		= VK_LBUTTON,
	MouseRight		= VK_RBUTTON,
	MouseMiddle		= VK_MBUTTON,
	MouseX1			= VK_XBUTTON1,
	MouseX2			= VK_XBUTTON2,

	BackSpace		= VK_BACK,
	Tab				= VK_TAB,
	Clear			= VK_CLEAR,
	Return			= VK_RETURN,

	Shift			= VK_SHIFT,
	LeftShift		= VK_LSHIFT,
	RightShift		= VK_RSHIFT,
	Control			= VK_CONTROL,
	LeftControl		= VK_LCONTROL,
	RightControl	= VK_RCONTROL,
	Alt				= VK_MENU,

	Pause			= VK_PAUSE,
	CapsLock		= VK_CAPITAL,
	Escape			= VK_ESCAPE,
	Space			= VK_SPACE,
	PageUp			= VK_PRIOR,
	PageDown		= VK_NEXT,
	End				= VK_END,
	Home			= VK_HOME,
	LeftArrow		= VK_LEFT,
	UpArrow			= VK_UP,
	RightArrow		= VK_RIGHT,
	DownArrow		= VK_DOWN,
	Select			= VK_SELECT,
	Print			= VK_PRINT,
	Execute			= VK_EXECUTE,
	PrintScreen		= VK_SNAPSHOT,
	Insert			= VK_INSERT,
	Delete			= VK_DELETE,
	Help			= VK_HELP,
	LeftMenu		= VK_LMENU,
	RightMenu		= VK_RMENU,

	LeftWindows		= VK_LWIN,
	RightWindows	= VK_RWIN,
	Applications	= VK_APPS,
	Sleep			= VK_SLEEP,
	VolumeMute		= VK_VOLUME_MUTE,
	VolumeDown		= VK_VOLUME_DOWN,
	VolumeUp		= VK_VOLUME_UP,
	NextTrack		= VK_MEDIA_NEXT_TRACK,
	PreviousTrack	= VK_MEDIA_PREV_TRACK,
	MediaStop		= VK_MEDIA_STOP,
	MediaPlayPause	= VK_MEDIA_PLAY_PAUSE,

	Multiply		= VK_MULTIPLY,
	Add				= VK_ADD,
	Separator		= VK_SEPARATOR,
	Substract		= VK_SUBTRACT,
	Decimal			= VK_DECIMAL,
	Divide			= VK_DIVIDE,

	NumLock			= VK_NUMLOCK,
	ScrollLock		= VK_SCROLL,

	Misc1			= VK_OEM_1, // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ';:' key
	Misc2			= VK_OEM_2, // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '/?' key
	Misc3			= VK_OEM_3, // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '`~' key
	Misc4			= VK_OEM_4, // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '[{' key
	Misc5			= VK_OEM_5, // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '\|' key
	Misc6			= VK_OEM_6, // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ']}' key
	Misc7			= VK_OEM_7, // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the 'single-quote/double-quote' key
	Misc8			= VK_OEM_8, // Used for miscellaneous characters; it can vary by keyboard.
	Misc9			= VK_OEM_102, // Either the angle bracket key or the backslash key on the RT 102-key keyboard
	
	Plus			= VK_OEM_PLUS,
	Comma			= VK_OEM_COMMA,
	Minus			= VK_OEM_MINUS,
	Period			= VK_OEM_PERIOD,

	key0			= 0x30,
	key1			= 0x31,
	key2			= 0x32,
	key3			= 0x33,
	key4			= 0x34,
	key5			= 0x35,
	key6			= 0x36,
	key7			= 0x37,
	key8			= 0x38,
	key9			= 0x39,

	num0			= 0x60,
	num1			= 0x61,
	num2			= 0x62,
	num3			= 0x63,
	num4			= 0x64,
	num5			= 0x65,
	num6			= 0x66,
	num7			= 0x67,
	num8			= 0x68,
	num9			= 0x69,

	keyA			= 0x41,
	keyB			= 0x42,
	keyC			= 0x43,
	keyD			= 0x44,
	keyE			= 0x45,
	keyF			= 0x46,
	keyG			= 0x47,
	keyH			= 0x48,
	keyI			= 0x49,
	keyJ			= 0x4A,
	keyK			= 0x4B,
	keyL			= 0x4C,
	keyM			= 0x4D,
	keyN			= 0x4E,
	keyO			= 0x4F,
	keyP			= 0x50,
	keyQ			= 0x51,
	keyR			= 0x52,
	keyS			= 0x53,
	keyT			= 0x54,
	keyU			= 0x55,
	keyV			= 0x56,
	keyW			= 0x57,
	keyX			= 0x58,
	keyY			= 0x59,
	keyZ			= 0x5A,

	F1				= VK_F1,
	F2				= VK_F2,
	F3				= VK_F3,
	F4				= VK_F4,
	F5				= VK_F5,
	F6				= VK_F6,
	F7				= VK_F7,
	F8				= VK_F8,
	F9				= VK_F9,
	F10				= VK_F10,
	F11				= VK_F11,
	F12				= VK_F12,
	F13				= VK_F13,
	F14				= VK_F14,
	F15				= VK_F15,
	F16				= VK_F16,
	F17				= VK_F17,
	F18				= VK_F18,
	F19				= VK_F19,
	F20				= VK_F20,
	F21				= VK_F21,
	F22				= VK_F22,
	F23				= VK_F23,
	F24				= VK_F24
	
};

class InputManager final
{
	friend class GameEngine;

private:

	InputManager();
	~InputManager();

	InputManager(const InputManager& other) = delete;
	InputManager(InputManager&& other) = delete;
	InputManager& operator=(const InputManager& other) = delete;
	InputManager& operator=(InputManager&& other) = delete;

	static InputManager* m_Instance;

public:

	static InputManager* GetInstance() { if (!m_Instance) m_Instance = new InputManager(); return m_Instance; }
	static void Destroy() { delete m_Instance; m_Instance = nullptr; }

	void Update(float deltaTime);

private:

	/** These functions should only be called by the game engine*/

	void KeyUp(WPARAM wParam, LPARAM lParam);
	void KeyUp(InputKeys key);
	void KeyDown(WPARAM wParam, LPARAM lParam);
	void KeyDown(InputKeys key);

	void MoveScrollWheel(short wheelDelta);
	void MoveMouse(WPARAM wParam, LPARAM lParam);
	
	void ClearInputs();
	void ClearDownUpKeys();

public:

	void SetMousePosition(const IVector2& pos, bool teleport = false);
	inline const IVector2& GetMousePosition() { return m_CurrentMousePos; }
	inline const IVector2& GetMouseMovement() { return m_MouseMovement; }
	inline void LockMouse() { m_IsMouseLocked = true; }

	void BindKeyDownAction(InputKeys key, IBaseClass* pObject, const std::function<void()>& action);
	void BindKeyUpAction(InputKeys key, IBaseClass* pObject, const std::function<void()>& action);
	void BindKeyPressedAction(InputKeys key, IBaseClass* pObject, const std::function<void()>& action);

	void BindMouseDownAction(InputKeys mouseKey, IBaseClass* pObject, const std::function<void(int, int)>& action);
	void BindMouseUpAction(InputKeys mouseKey, IBaseClass* pObject, const std::function<void(int, int)>& action);
	void BindMousePressedAction(InputKeys mouseKey, IBaseClass* pObject, const std::function<void(int, int)>& action);

private:

	// Key input variables
	std::vector<InputKeys> m_KeysPressed;
	std::vector<InputKeys> m_KeysDown; // gets cleared after every frame
	std::vector<InputKeys> m_KeysUp; // gets cleared after every frame

	// Mouse inputs
	bool m_IsMouseLocked = false;
	bool m_IgnoreLastMovement = false;
	IVector2 m_MouseMovement = {};
	IVector2 m_PreviousMousePos = {};
	IVector2 m_CurrentMousePos = {};
	IVector2 m_LockedMousePosition = {};

	std::map<InputKeys, IDelegate<>> m_KeysDownActions;
	std::map<InputKeys, IDelegate<>> m_KeysUpActions;
	std::map<InputKeys, IDelegate<>> m_KeysPressedActions;

	std::map<InputKeys, IDelegate<int, int>> m_MouseDownActions;
	std::map<InputKeys, IDelegate<int, int>> m_MouseUpActions;
	std::map<InputKeys, IDelegate<int, int>> m_MousePressedActions;

	// Controler inputs
	XINPUT_STATE m_ControllerStates[4]{};

private:

	void BroadCastAllInputs();
};


