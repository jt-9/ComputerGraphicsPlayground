// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

#pragma once


#include "View.h"

#include <memory>
#include <filesystem>
//
//  CLASS: CApplication : IUIApplication
//
//  PURPOSE: Implements interface IUIApplication that defines methods
//           required to manage Framework events.
//
//  COMMENTS:
//
//    CApplication implements the IUIApplication interface which is required for any ribbon application.
//    IUIApplication contains callbacks made by the ribbon framework to the application
//    during various updates like command creation/destruction and view state changes.
//

class Application
    : public IUIApplication // Applications must implement IUIApplication.
{
public:

    // Static method to create an instance of the object.
    static HRESULT CreateInstance(HWND frameWnd, Application* &pApplication);

    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID iid, void** ppv);
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();

    // IUIApplication methods
    STDMETHOD(OnCreateUICommand)(UINT nCmdID,
        UI_COMMANDTYPE typeID,
        IUICommandHandler** ppCommandHandler);

    STDMETHOD(OnViewChanged)(UINT viewId,
        UI_VIEWTYPE typeId,
        IUnknown* pView,
        UI_VIEWVERB verb,
        INT uReasonCode);

    STDMETHOD(OnDestroyUICommand)(UINT32 commandId, 
        UI_COMMANDTYPE typeID,
        IUICommandHandler* commandHandler);

    MainView* getMainView() const noexcept;

    void loadModel(const std::filesystem::path& filePath) const;
    void clearScene();
private:
    Application(HWND frameWnd) noexcept;
    ~Application() noexcept;

    LONG m_cRef;                            // Reference count.
    IUICommandHandler * m_pCommandHandler;  // Generic Command Handler
    std::unique_ptr<MainView> mainView;
};
