// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

#include "Application.h"
#include "CommandHandler.h"
#include "ModelReader.h"
#include "View.h"

#include <UIRibbon.h>
#include <UIRibbonPropertyHelpers.h>

#include <chrono>

// Static method to create an instance of the object.
HRESULT Application::CreateInstance(HWND frameWnd, Application*& pApplication)
{   
    pApplication = new Application(frameWnd);

    return (pApplication) ? S_OK : E_OUTOFMEMORY;
}

Application::Application(HWND frameWnd) noexcept
    : m_cRef{ 1 }, m_pCommandHandler{ nullptr }, mainView{ std::make_unique<MainView>(frameWnd) }
{}

Application::~Application() noexcept {
    if (m_pCommandHandler)
    {
        m_pCommandHandler->Release();
        m_pCommandHandler = nullptr;
    }
}

// IUnknown method implementations.
STDMETHODIMP_(ULONG) Application::AddRef() {
    return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) Application::Release() {
    LONG cRef = InterlockedDecrement(&m_cRef);
    if (cRef == 0)
    {
        delete this;
    }

    return cRef;
}

STDMETHODIMP Application::QueryInterface(REFIID iid, void** ppv) {
    if (iid == __uuidof(IUnknown))
    {
        *ppv = static_cast<IUnknown*>(this);
    }
    else if (iid == __uuidof(IUIApplication))
    {
        *ppv = static_cast<IUIApplication*>(this);
    }
    else 
    {
        *ppv = nullptr;
        return E_NOINTERFACE;
    }

    AddRef();
    return S_OK;
}

//
//  FUNCTION: OnCreateUICommand(UINT, UI_COMMANDTYPE, IUICommandHandler)
//
//  PURPOSE: Called by the Ribbon framework for each command specified in markup, to allow
//           the host application to bind a command handler to that command.
//
//  COMMENTS:
//
//    In this SimpleRibbon sample, the same command handler is returned for all commands
//    specified in the SimpleRibbon.xml file.
//    
//    To view the OnCreateUICommand callbacks, uncomment the _cwprintf call.
//
//
STDMETHODIMP Application::OnCreateUICommand(
    UINT nCmdID,
    UI_COMMANDTYPE typeID,
    IUICommandHandler** ppCommandHandler) {
    UNREFERENCED_PARAMETER(typeID);
    UNREFERENCED_PARAMETER(nCmdID);

    if (nullptr == m_pCommandHandler)
    {
        HRESULT hr = CCommandHandler::CreateInstance(&m_pCommandHandler);
        if (FAILED(hr))
        {
            return hr;
        }
    }

    return m_pCommandHandler->QueryInterface(IID_PPV_ARGS(ppCommandHandler));
}

//
//  FUNCTION: OnViewChanged(UINT, UI_VIEWTYPE, IUnknown*, UI_VIEWVERB, INT)
//
//  PURPOSE: Called when the state of a View (Ribbon is a view) changes, for example, created, destroyed, or resized.
//
//
STDMETHODIMP Application::OnViewChanged(
    UINT viewId,
    UI_VIEWTYPE typeId,
    IUnknown* pView,
    UI_VIEWVERB verb,
    INT uReasonCode) {
    return mainView->onViewChanged(viewId, typeId, pView, verb, uReasonCode);
}


//
//  FUNCTION: OnDestroyUICommand(UINT, UI_COMMANDTYPE, IUICommandHandler*)
//
//  PURPOSE: Called by the Ribbon framework for each command at the time of ribbon destruction.
//
STDMETHODIMP Application::OnDestroyUICommand(
    UINT32 nCmdID,
    UI_COMMANDTYPE typeID,
    IUICommandHandler* commandHandler) {
    UNREFERENCED_PARAMETER(commandHandler);
    UNREFERENCED_PARAMETER(typeID);
    UNREFERENCED_PARAMETER(nCmdID);

    return E_NOTIMPL;
}

MainView* Application::getMainView() const noexcept {
    return mainView.get();
}

void Application::loadModel(const std::filesystem::path& filePath) const {
    try {
        auto future = mycg::readModelFromObjInputStreamAsync<MainView::VisualModel::CoordinateType>(filePath, std::launch::async);
        if (future.valid()) {
            auto status = future.wait_for(std::chrono::seconds{ 20 });
            
            if (status == std::future_status::ready) {
                mainView->setModel(future.get(), true);
                mainView->invalidate();
            }
        }
    }
    catch (const std::exception& e) {
        UNREFERENCED_PARAMETER(e);
    }
}

void Application::clearScene() {
    mainView->clearScene();
    mainView->invalidate();
}
