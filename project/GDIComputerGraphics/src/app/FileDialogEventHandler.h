#ifndef FileDialogEventHandler_h
#define FileDialogEventHandler_h

#pragma once

#include <windows.h>
#define STRICT_TYPED_ITEMIDS
//#include <shlobj.h>
//#include <objbase.h>      // For COM headers
#include <shobjidl.h>     // for IFileDialogEvents and IFileDialogControlEvents
#include <knownfolders.h> // for KnownFolder APIs/datatypes/function headers
#include <propvarutil.h>  // for PROPVAR-related functions
#include <propkey.h>      // for the Property key APIs/datatypes
#include <propidl.h>      // for the Property System APIs
#include <shtypes.h>      // for COMDLG_FILTERSPEC

#include <vector>
#include <string>

class FileDialogEventHandler: public IFileDialogEvents, public IFileDialogControlEvents {
public:
    /*const constexpr COMDLG_FILTERSPEC c_rgSaveTypes[] =
    {
        {L"Graphic obj (*.obj)",       L"*.doc"},
        {L"All Documents (*.*)",       L"*.*"}
    };*/

    // Indices of file types
    static const int INDEX_OBJ_FILE = 1;

public:
    // Instance creation helper
    static HRESULT CreateInstance(REFIID riid, void** ppv, const std::vector<COMDLG_FILTERSPEC>& saveTypes) noexcept;


    IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv);
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();

    // IFileDialogEvents methods
    IFACEMETHODIMP OnFileOk(IFileDialog*) { return S_OK; };
    IFACEMETHODIMP OnFolderChange(IFileDialog*) { return S_OK; };
    IFACEMETHODIMP OnFolderChanging(IFileDialog*, IShellItem*) { return S_OK; };
    IFACEMETHODIMP OnHelp(IFileDialog*) { return S_OK; };
    IFACEMETHODIMP OnSelectionChange(IFileDialog*) { return S_OK; };
    IFACEMETHODIMP OnShareViolation(IFileDialog*, IShellItem*, FDE_SHAREVIOLATION_RESPONSE*) { return S_OK; };
    IFACEMETHODIMP OnTypeChange(IFileDialog* pfd);
    IFACEMETHODIMP OnOverwrite(IFileDialog*, IShellItem*, FDE_OVERWRITE_RESPONSE*) { return S_OK; };

    // IFileDialogControlEvents methods
    IFACEMETHODIMP OnItemSelected(IFileDialogCustomize*, DWORD, DWORD) { return S_OK; };
    IFACEMETHODIMP OnButtonClicked(IFileDialogCustomize*, DWORD) { return S_OK; };
    IFACEMETHODIMP OnCheckButtonToggled(IFileDialogCustomize*, DWORD, BOOL) { return S_OK; };
    IFACEMETHODIMP OnControlActivating(IFileDialogCustomize*, DWORD) { return S_OK; };

private:
    FileDialogEventHandler(const std::vector<COMDLG_FILTERSPEC>& saveTypes) noexcept
        : _cRef{ 1 }, _c_rgSaveTypes{ saveTypes }
    {};

    ~FileDialogEventHandler() noexcept
    {};

    long _cRef;
    std::vector<COMDLG_FILTERSPEC> _c_rgSaveTypes;
};

HRESULT basicFileOpen(const std::vector<COMDLG_FILTERSPEC>& saveTypes, std::wstring& filePath);

#endif //FileDialogEventHandler_h

