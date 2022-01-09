#include "IDialogBox.h"
#include "GameEngine.h"
#include <cassert>

///////////////////////////////////
////// Input String Dialog ////////
///////////////////////////////////

IDelegate<const tstring&> InputStringDialog::OnOk{};
IDelegate<> InputStringDialog::OnCancel{};

HWND InputStringDialog::m_EditHandle{};

LRESULT CALLBACK WndProcInputStringDialog(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_COMMAND:
		switch (wParam)
		{
		case 4:
			wchar_t text[64];
			GetWindowText(InputStringDialog::m_EditHandle, text, 64);
			text[63] = '\0'; // add a zero at end just to be sure
			InputStringDialog::OnOk.BroadCast(std::wstring(text));
			InputStringDialog::OnCancel.BroadCast();
			break;
		case 3:
			InputStringDialog::OnCancel.BroadCast();
			break;
		}
		return 0;
	case WM_CLOSE:
		InputStringDialog::OnCancel.BroadCast();
		return 0;
	default:
		return DefWindowProcW(hWindow, msg, wParam, lParam);
	}
}

void CustomClassDeleter(tstring* string)
{
	delete string;
	UnregisterClass(_T("InputStringDialog"), ENGINE->GetHInstance());
};

InputStringDialog::InputStringDialog(const tstring& text)
{

	WNDCLASSW dialog{};

	dialog.hbrBackground = (HBRUSH)COLOR_WINDOW;
	dialog.hInstance = ENGINE->GetHInstance();
	dialog.lpszClassName = _T("InputStringDialog");
	dialog.lpfnWndProc = WndProcInputStringDialog;

	RegisterClass(&dialog);

	const tstring className{ _T("InputStringDialog") };

	m_Handle = CreateWindow(
		className.c_str(),
		_T("Dialog"),
		WS_VISIBLE | WS_POPUPWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		220,
		120,
		ENGINE->GetWindowHandle(),
		NULL,
		ENGINE->GetHInstance(),
		NULL);

	// text
	m_StaticHandle = CreateWindow(_T("STATIC"), text.c_str(), WS_VISIBLE | WS_CHILD | SS_CENTER, 20, 20, 160, 40, m_Handle, (HMENU)1, NULL, NULL);

	// edit box
	m_EditHandle = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE, 20, 40, 160, 20, m_Handle, (HMENU)2, NULL, NULL);

	// cancel button
	m_ButtonHandle = CreateWindow(_T("BUTTON"), _T("Cancel"), WS_VISIBLE | WS_CHILD, 20, 60, 60, 20, m_Handle, (HMENU)3, NULL, NULL);

	// ok button
	m_ButtonHandle2 = CreateWindow(_T("BUTTON"), _T("OK"), WS_VISIBLE | WS_CHILD, 120, 60, 60, 20, m_Handle, (HMENU)4, NULL, NULL);

	OnCancel.BindFunction(FlipFlop{}, [this]() {delete this; });
}

InputStringDialog::~InputStringDialog()
{

	SafeDestroyWindow(m_Handle);
	OnCancel.RemoveAllFunctions();
	OnOk.RemoveAllFunctions();


	SafeDestroyWindow(m_StaticHandle);
	SafeDestroyWindow(m_EditHandle);
	SafeDestroyWindow(m_ButtonHandle);
	SafeDestroyWindow(m_ButtonHandle2);

	if (!UnregisterClass(_T("InputStringDialog"), ENGINE->GetHInstance())) {
		LPVOID lpMsgBuf;
		LPVOID lpDisplayBuf;
		DWORD dw = GetLastError();

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);

		assert(false);
	}
}
