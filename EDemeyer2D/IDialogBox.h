#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Structs.h"
#include "Delegates.h"

class InputStringDialog
{
public:

	InputStringDialog(const tstring& text);
	virtual ~InputStringDialog();

	InputStringDialog(const InputStringDialog& other) = delete;
	InputStringDialog(InputStringDialog&& other) = delete;
	InputStringDialog& operator=(InputStringDialog&& other) = delete;
	InputStringDialog& operator=(const InputStringDialog& other) = delete;

public:

	HWND GetEditHandle() { return m_EditHandle; }

	HWND GetWindowHandle() { return m_Handle; }

	static IDelegate<const tstring&> OnOk;

	static IDelegate<> OnCancel;

	static HWND m_EditHandle;
protected:

	HWND m_Handle{};

	HWND m_StaticHandle{};
	HWND m_ButtonHandle{};
	HWND m_ButtonHandle2{};
};

