#pragma once

#include <windows.h>      // For common windows data types and function headers
#define STRICT_TYPED_ITEMIDS
#include <shlobj.h>
#include <objbase.h>      // For COM headers
#include <shobjidl.h>     // for IFileDialogEvents and IFileDialogControlEvents
#include <shlwapi.h>
#include <knownfolders.h> // for KnownFolder APIs/datatypes/function headers
#include <propvarutil.h>  // for PROPVAR-related functions
#include <propkey.h>      // for the Property key APIs/datatypes
#include <propidl.h>      // for the Property System APIs
#include <strsafe.h>      // for StringCchPrintfW
#include <shtypes.h>      // for COMDLG_FILTERSPEC
#include <new>
#include <string>
#include <Shlwapi.h>
#include <propsys.h>

#pragma comment(linker, "\"/manifestdependency:type='Win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

/* File Dialog Event Handler *****************************************************************************************************/

class CDialogEventHandler : public IFileDialogEvents,
    public IFileDialogControlEvents
{
public:
    // IUnknown methods
    IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv)
    {
        static const QITAB qit[] = {
            QITABENT(CDialogEventHandler, IFileDialogEvents),
            QITABENT(CDialogEventHandler, IFileDialogControlEvents),
            { 0 },
#pragma warning(suppress:4838)
        };
        return QISearch(this, qit, riid, ppv);
    }

    IFACEMETHODIMP_(ULONG) AddRef()
    {
        return InterlockedIncrement(&_cRef);
    }

    IFACEMETHODIMP_(ULONG) Release()
    {
        long cRef = InterlockedDecrement(&_cRef);
        if (!cRef)
            delete this;
        return cRef;
    }

    // IFileDialogEvents methods
    IFACEMETHODIMP OnFileOk(IFileDialog*) { return S_OK; };
    IFACEMETHODIMP OnFolderChange(IFileDialog*) { return S_OK; };
    IFACEMETHODIMP OnFolderChanging(IFileDialog*, IShellItem*) { return S_OK; };
    IFACEMETHODIMP OnHelp(IFileDialog*) { return S_OK; };
    IFACEMETHODIMP OnSelectionChange(IFileDialog*) { return S_OK; };
    IFACEMETHODIMP OnShareViolation(IFileDialog*, IShellItem*, FDE_SHAREVIOLATION_RESPONSE*) { return S_OK; };
    IFACEMETHODIMP OnTypeChange(IFileDialog* pfd) { return S_OK; };
    IFACEMETHODIMP OnOverwrite(IFileDialog*, IShellItem*, FDE_OVERWRITE_RESPONSE*) { return S_OK; };

    // IFileDialogControlEvents methods
    IFACEMETHODIMP OnItemSelected(IFileDialogCustomize* pfdc, DWORD dwIDCtl, DWORD dwIDItem) { return S_OK; };
    IFACEMETHODIMP OnButtonClicked(IFileDialogCustomize*, DWORD) { return S_OK; };
    IFACEMETHODIMP OnCheckButtonToggled(IFileDialogCustomize*, DWORD, BOOL) { return S_OK; };
    IFACEMETHODIMP OnControlActivating(IFileDialogCustomize*, DWORD) { return S_OK; };

    CDialogEventHandler() : _cRef(1) { };
private:
    ~CDialogEventHandler() { };
    long _cRef;
};

// Instance creation helper
HRESULT CDialogEventHandler_CreateInstance(REFIID riid, void** ppv)
{
    *ppv = NULL;
    CDialogEventHandler* pDialogEventHandler = new (std::nothrow) CDialogEventHandler();
    HRESULT hr = pDialogEventHandler ? S_OK : E_OUTOFMEMORY;
    if (SUCCEEDED(hr))
    {
        hr = pDialogEventHandler->QueryInterface(riid, ppv);
        pDialogEventHandler->Release();
    }
    return hr;
}

/* Utility Functions *************************************************************************************************************/

// A helper function that converts UNICODE data to ANSI and writes it to the given file.
// We write in ANSI format to make it easier to open the output file in Notepad.
HRESULT _WriteDataToFile(HANDLE hFile, PCWSTR pszDataIn)
{
    // First figure out our required buffer size.
    DWORD cbData = WideCharToMultiByte(CP_ACP, 0, pszDataIn, -1, NULL, 0, NULL, NULL);
    HRESULT hr = (cbData == 0) ? HRESULT_FROM_WIN32(GetLastError()) : S_OK;
    if (SUCCEEDED(hr))
    {
        // Now allocate a buffer of the required size, and call WideCharToMultiByte again to do the actual conversion.
        char* pszData = new (std::nothrow) CHAR[cbData];
        hr = pszData ? S_OK : E_OUTOFMEMORY;
        if (SUCCEEDED(hr))
        {
            hr = WideCharToMultiByte(CP_ACP, 0, pszDataIn, -1, pszData, cbData, NULL, NULL)
                ? S_OK
                : HRESULT_FROM_WIN32(GetLastError());
            if (SUCCEEDED(hr))
            {
                DWORD dwBytesWritten = 0;
                hr = WriteFile(hFile, pszData, cbData - 1, &dwBytesWritten, NULL)
                    ? S_OK
                    : HRESULT_FROM_WIN32(GetLastError());
            }
            delete[] pszData;
        }
    }
    return hr;
}

// Helper function to write property/value into a custom file format.
//
// We are inventing a dummy format here:
// [APPDATA]
// xxxxxx
// [ENDAPPDATA]
// [PROPERTY]foo=bar[ENDPROPERTY]
// [PROPERTY]foo2=bar2[ENDPROPERTY]
HRESULT _WritePropertyToCustomFile(PCWSTR pszFileName, PCWSTR pszPropertyName, PCWSTR pszValue)
{
    HANDLE hFile = CreateFileW(pszFileName,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ,
        NULL,
        OPEN_ALWAYS, // We will write only if the file exists.
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    HRESULT hr = (hFile == INVALID_HANDLE_VALUE) ? HRESULT_FROM_WIN32(GetLastError()) : S_OK;
    if (SUCCEEDED(hr))
    {
        const WCHAR           wszPropertyStartTag[] = L"[PROPERTY]";
        const WCHAR           wszPropertyEndTag[] = L"[ENDPROPERTY]\r\n";
        const DWORD           cchPropertyStartTag = (DWORD)wcslen(wszPropertyStartTag);
        const static DWORD    cchPropertyEndTag = (DWORD)wcslen(wszPropertyEndTag);
        DWORD const cchPropertyLine = cchPropertyStartTag +
            cchPropertyEndTag +
            (DWORD)wcslen(pszPropertyName) +
            (DWORD)wcslen(pszValue) +
            2; // 1 for '=' + 1 for NULL terminator.
        PWSTR pszPropertyLine = new (std::nothrow) WCHAR[cchPropertyLine];
        hr = pszPropertyLine ? S_OK : E_OUTOFMEMORY;
        if (SUCCEEDED(hr))
        {
            hr = StringCchPrintfW(pszPropertyLine,
                cchPropertyLine,
                L"%s%s=%s%s",
                wszPropertyStartTag,
                pszPropertyName,
                pszValue,
                wszPropertyEndTag);
            if (SUCCEEDED(hr))
            {
                hr = SetFilePointer(hFile, 0, NULL, FILE_END) ? S_OK : HRESULT_FROM_WIN32(GetLastError());
                if (SUCCEEDED(hr))
                {
                    hr = _WriteDataToFile(hFile, pszPropertyLine);
                }
            }
            delete[] pszPropertyLine;
        }
        CloseHandle(hFile);
    }

    return hr;
}

// Helper function to write dummy content to a custom file format.
//
// We are inventing a dummy format here:
// [APPDATA]
// xxxxxx
// [ENDAPPDATA]
// [PROPERTY]foo=bar[ENDPROPERTY]
// [PROPERTY]foo2=bar2[ENDPROPERTY]
HRESULT _WriteDataToCustomFile(PCWSTR pszFileName)
{
    HANDLE hFile = CreateFileW(pszFileName,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ,
        NULL,
        CREATE_ALWAYS,  // Let's always create this file.
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    HRESULT hr = (hFile == INVALID_HANDLE_VALUE) ? HRESULT_FROM_WIN32(GetLastError()) : S_OK;
    if (SUCCEEDED(hr))
    {
        WCHAR wszDummyContent[] = L"[MYAPPDATA]\r\nThis is an example of how to use the IFileSaveDialog interface.\r\n[ENDMYAPPDATA]\r\n";

        hr = _WriteDataToFile(hFile, wszDummyContent);
        CloseHandle(hFile);
    }
    return hr;
}

/* Common File Dialog Snippets ***************************************************************************************************/

// This code snippet demonstrates how to work with the common file dialog interface
HRESULT BasicFileOpen(std::wstring& path, const COMDLG_FILTERSPEC& fileType = {L"All Documents (*.*)", L"*.*"})
{
    // CoCreate the File Open Dialog object.
    IFileDialog* pfd = NULL;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
    if (SUCCEEDED(hr))
    {
        // Create an event handling object, and hook it up to the dialog.
        IFileDialogEvents* pfde = NULL;
        hr = CDialogEventHandler_CreateInstance(IID_PPV_ARGS(&pfde));
        if (SUCCEEDED(hr))
        {
            // Hook up the event handler.
            DWORD dwCookie;
            hr = pfd->Advise(pfde, &dwCookie);
            if (SUCCEEDED(hr))
            {
                // Set the options on the dialog.
                DWORD dwFlags;

                // Before setting, always get the options first in order not to override existing options.
                hr = pfd->GetOptions(&dwFlags);
                if (SUCCEEDED(hr))
                {
                    // In this case, get shell items only for file system items.
                    hr = pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
                    if (SUCCEEDED(hr))
                    {
                        // Set the file types to display only. Notice that, this is a 1-based array.
                        hr = pfd->SetFileTypes(1, &fileType);
                        if (SUCCEEDED(hr))
                        {
                            
                            hr = pfd->SetFileTypeIndex(1);
                            if (SUCCEEDED(hr))
                            {
                                // Set the default extension to be ".doc" file.
                                hr = pfd->SetDefaultExtension(fileType.pszSpec);
                                if (SUCCEEDED(hr))
                                {
                                    // Show the dialog
                                    hr = pfd->Show(NULL);
                                    if (SUCCEEDED(hr))
                                    {
                                        // Obtain the result, once the user clicks the 'Open' button.
                                        // The result is an IShellItem object.
                                        IShellItem* psiResult;
                                        hr = pfd->GetResult(&psiResult);
                                        if (SUCCEEDED(hr))
                                        {
                                            PWSTR pszFilePath = NULL;
                                            hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
                                            if (SUCCEEDED(hr))
                                            {
                                                path = std::wstring((wchar_t*)pszFilePath);
                                            }
                                            psiResult->Release();
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                // Unhook the event handler.
                pfd->Unadvise(dwCookie);
            }
            pfde->Release();
        }
        pfd->Release();
    }
    return hr;
}

// TODO
//https://github.com/microsoft/Windows-classic-samples/blob/main/Samples/Win7Samples/winui/shell/appplatform/commonfiledialog/CommonFileDialogApp.cpp

// This code snippet demonstrates how to add default metadata in the Common File Dialog.
// Look at CDialogEventHandler::OnTypeChange to see to change the order/list of properties
// displayed in the Common File Dialog.
//HRESULT BasicFileSave(IShellItem& file, const COMDLG_FILTERSPEC& fileType = { L"All Documents (*.*)", L"*.*" })
//{
//    // CoCreate the File Open Dialog object.
//    IFileSaveDialog* pfsd = NULL;
//    HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfsd));
//    if (SUCCEEDED(hr))
//    {
//        // Create an event handling object, and hook it up to the dialog.
//        IFileDialogEvents* pfde = NULL;
//        DWORD               dwCookie = 0;
//        hr = CDialogEventHandler_CreateInstance(IID_PPV_ARGS(&pfde));
//        if (SUCCEEDED(hr))
//        {
//            // Hook up the event handler.
//            hr = pfsd->Advise(pfde, &dwCookie);
//            if (SUCCEEDED(hr))
//            {
//                // Set the file types to display.
//                hr = pfsd->SetFileTypes(1, &fileType);
//                if (SUCCEEDED(hr))
//                {
//                    hr = pfsd->SetFileTypeIndex(1);
//                    if (SUCCEEDED(hr))
//                    {
//                        hr = pfsd->SetDefaultExtension(fileType.pszSpec);
//                        if (SUCCEEDED(hr))
//                        {
//                            IPropertyStore* pps = NULL;
//
//                            // The InMemory Property Store is a Property Store that is
//                            // kept in the memory instead of persisted in a file stream.
//                            hr = PSCreateMemoryPropertyStore(IID_PPV_ARGS(&pps));
//                            if (SUCCEEDED(hr))
//                            {
//                                PROPVARIANT propvarValue = {};
//                                hr = InitPropVariantFromString(L"SampleKeywordsValue", &propvarValue);
//                                if (SUCCEEDED(hr))
//                                {
//                                    // Set the value to the property store of the item.
//                                    hr = pps->SetValue(PKEY_Keywords, propvarValue);
//                                    if (SUCCEEDED(hr))
//                                    {
//                                        // Commit does the actual writing back to the in memory store.
//                                        hr = pps->Commit();
//                                        if (SUCCEEDED(hr))
//                                        {
//                                            // Hand these properties to the File Dialog.
//                                            hr = pfsd->SetCollectedProperties(NULL, TRUE);
//                                            if (SUCCEEDED(hr))
//                                            {
//                                                hr = pfsd->SetProperties(pps);
//                                            }
//                                        }
//                                    }
//                                    PropVariantClear(&propvarValue);
//                                }
//                                pps->Release();
//                            }
//                        }
//                    }
//                }
//
//                if (FAILED(hr))
//                {
//                    // Unadvise here in case we encounter failures before we get a chance to show the dialog.
//                    pfsd->Unadvise(dwCookie);
//                }
//            }
//            pfde->Release();
//        }
//
//        if (SUCCEEDED(hr))
//        {
//            // Now show the dialog.
//            hr = pfsd->Show(NULL);
//            if (SUCCEEDED(hr))
//            {
//                IShellItem* psiResult{};
//                hr = pfsd->GetFolder(&psiResult);
//                if (SUCCEEDED(hr))
//                {
//                    LPWSTR fileName{};
//                    hr = pfsd->GetFileName(&fileName);
//                    if (SUCCEEDED(hr))
//                    {
//                        LPWSTR filePath{};
//                        hr = psiResult->GetDisplayName(SIGDN_URL, &filePath);
//                        if (SUCCEEDED(hr))
//                        {
//                            std::wstring wpath{ (wchar_t*)filePath };
//                            std::wstring wname{ (wchar_t*)fileName };
//                            path = wpath + L"/" + wname;
//                        }
//                    }
//                    psiResult->Release();
//                }
//            }
//            // Unhook the event handler.
//            pfsd->Unadvise(dwCookie);
//        }
//        pfsd->Release();
//    }
//    return hr;
//}
