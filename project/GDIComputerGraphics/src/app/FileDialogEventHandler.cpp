#include "FileDialogEventHandler.h"
#include "unique_rc.hpp"
#include "windows_com_release.hpp"
#include "windows_com_file_dlg_unadvise.hpp"

#include <cassert>
#include <shlwapi.h>
#include <string>

HRESULT FileDialogEventHandler::CreateInstance(REFIID riid, void** ppv, const std::vector<COMDLG_FILTERSPEC>& saveTypes) noexcept {
	assert(ppv);
	*ppv = nullptr;

	auto pDialogEventHandler = new (std::nothrow) FileDialogEventHandler(saveTypes);
	auto hr = pDialogEventHandler ? S_OK : E_OUTOFMEMORY;

	if (SUCCEEDED(hr))
	{
		hr = pDialogEventHandler->QueryInterface(riid, ppv);
		pDialogEventHandler->Release();
	}
	return hr;
}

IFACEMETHODIMP FileDialogEventHandler::QueryInterface(REFIID riid, void** ppv) {
	static const QITAB qit[] = {
		QITABENT(FileDialogEventHandler, IFileDialogEvents),
		QITABENT(FileDialogEventHandler, IFileDialogControlEvents),
		{ 0 },
		//#pragma warning(suppress:4838)
	};
	return QISearch(this, qit, riid, ppv);
}

IFACEMETHODIMP_(ULONG) FileDialogEventHandler::AddRef() {
	return InterlockedIncrement(&_cRef);
}

IFACEMETHODIMP_(ULONG) FileDialogEventHandler::Release() {
	long cRef = InterlockedDecrement(&_cRef);
	if (0 == cRef)
		delete this;
	return cRef;
}

// IFileDialogEvents methods
// This method gets called when the file-type is changed (combo-box selection changes).
// For sample sake, let's react to this event by changing the properties show.
HRESULT FileDialogEventHandler::OnTypeChange(IFileDialog* pfd) {
	HRESULT hr = S_OK;
	do {
		raii::unique_rc< IFileSaveDialog*, raii::com_object_release_nullptr<IFileSaveDialog*> > pfsd{};

		{
			IFileSaveDialog* p{};
			hr = pfd->QueryInterface(&p);
			if (FAILED(hr)) {
				break;
			}

			pfsd.reset(p);
		}

		UINT uIndex;
		hr = pfsd->GetFileTypeIndex(&uIndex);   // index of current file-type
		if (FAILED(hr)) {
			break;
		}

		//IPropertyDescriptionList* pdl = nullptr;
		raii::unique_rc< IPropertyDescriptionList*, raii::com_object_release_nullptr<IPropertyDescriptionList*> > pdl;

		switch (uIndex) {
		case INDEX_OBJ_FILE: {
			// When .obj is selected, let's ask for some arbitrary property, say Title.
			{
				IPropertyDescriptionList* p{};
				hr = PSGetPropertyDescriptionListFromString(L"prop:System.Title", IID_PPV_ARGS(&p));
				if (FAILED(hr)) {
					break;
				}
				pdl.reset(p);
			}

			// FALSE as second param == do not show default properties.
			hr = pfsd->SetCollectedProperties(pdl.get(), FALSE);

			break;
		}
		}

	} while (false);

	return hr;
}

// This code snippet demonstrates how to work with the common file dialog interface
HRESULT basicFileOpen(const std::vector<COMDLG_FILTERSPEC>& saveTypes, std::wstring& filePath) {
	HRESULT hr = S_OK;
	do {
		// CoCreate the File Open Dialog object.
		raii::unique_rc< IFileDialog*, raii::com_object_release_nullptr<IFileDialog*> > pfd{};

		{
			IFileDialog* p = nullptr;
			hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&p));
			if (FAILED(hr)) {
				break;
			}

			pfd.reset(p);
		}

		raii::unique_rc< IFileDialogEvents*, raii::com_object_release_nullptr<IFileDialogEvents*> > pfde;
		{
			// Create an event handling object, and hook it up to the dialog.
			IFileDialogEvents* p = nullptr;
			hr = FileDialogEventHandler::CreateInstance(IID_PPV_ARGS(&p), saveTypes);
			if (FAILED(hr)) {
				break;
			}

			pfde.reset(p);
		}

		raii::unique_rc<DWORD, raii::com_object_file_dialog_unadvise> pfdAdvise{ 0, raii::com_object_file_dialog_unadvise{nullptr} };
		{
			// Hook up the event handler.
			DWORD dwCookie;
			hr = pfd->Advise(pfde.get(), &dwCookie);
			if (FAILED(hr)) {
				break;
			}

			pfdAdvise = raii::unique_rc<DWORD, raii::com_object_file_dialog_unadvise>{ dwCookie, raii::com_object_file_dialog_unadvise{pfd.get()} };
		}

		// Set the options on the dialog.
		DWORD dwFlags;
		// Before setting, always get the options first in order not to override existing options.
		hr = pfd->GetOptions(&dwFlags);
		if (FAILED(hr)) {
			break;
		}

		// In this case, get shell items only for file system items.
		hr = pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
		if (FAILED(hr)) {
			break;
		}

		// Set the file types to display only. Notice that, this is a 1-based array.
		hr = pfd->SetFileTypes(static_cast<UINT>(saveTypes.size()), saveTypes.data());
		if (FAILED(hr)) {
			break;
		}

		// Set the selected file type index to obj for this example.
		hr = pfd->SetFileTypeIndex(saveTypes.empty() ? 0 : 1);
		if (FAILED(hr)) {
			break;
		}

		// Set the default extension to be ".obj" file.
		hr = pfd->SetDefaultExtension(L"obj");
		if (FAILED(hr)) {
			break;
		}

		// Show the dialog
		hr = pfd->Show(nullptr);
		if (FAILED(hr)) {
			break;
		}

		// Obtain the result, once the user clicks the 'Open' button.
		// The result is an IShellItem object.
		raii::unique_rc< IShellItem*, raii::com_object_release_nullptr<IShellItem*> > psiResult{};
		{
			IShellItem* p;
			hr = pfd->GetResult(&p);

			if (FAILED(hr)) {
				break;
			}

			psiResult.reset(p);
		}

		// We are just going to print out the name of the file for sample sake.
		PWSTR pszFilePath = nullptr;
		hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
		if (SUCCEEDED(hr)) {
			/*TaskDialog(nullptr,
				nullptr,
				L"CommonFileDialogApp",
				pszFilePath,
				nullptr,
				TDCBF_OK_BUTTON,
				TD_INFORMATION_ICON,
				nullptr);*/
			filePath = pszFilePath;
			CoTaskMemFree(pszFilePath);
		}

		// Unhook the event handler in destructor.
	} while (false);

	return hr;
}