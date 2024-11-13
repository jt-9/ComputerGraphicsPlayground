// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.


#include "CommandHandler.h"
#include "RibbonLayout.h"
#include "RibbonFramework.h"
#include "FileDialogEventHandler.h"

#include <filesystem>

// Static method to create an instance of the object.
HRESULT CCommandHandler::CreateInstance(IUICommandHandler **ppCommandHandler)
{
    if (!ppCommandHandler)
    {
        return E_POINTER;
    }

    *ppCommandHandler = nullptr;

    HRESULT hr = S_OK;
   
    CCommandHandler* pCommandHandler = new CCommandHandler();

    if (pCommandHandler != nullptr)
    {
        *ppCommandHandler = static_cast<IUICommandHandler *>(pCommandHandler);
    }
    else
    {
        hr = E_OUTOFMEMORY;
    }

    return hr;
}

// IUnknown method implementations.
STDMETHODIMP_(ULONG) CCommandHandler::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) CCommandHandler::Release()
{
    LONG cRef = InterlockedDecrement(&m_cRef);
    if (cRef == 0)
    {
        delete this;
    }

    return cRef;
}

STDMETHODIMP CCommandHandler::QueryInterface(REFIID iid, void** ppv)
{
    if (iid == __uuidof(IUnknown))
    {
        *ppv = static_cast<IUnknown*>(this);
    }
    else if (iid == __uuidof(IUICommandHandler))
    {
        *ppv = static_cast<IUICommandHandler*>(this);
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
//  FUNCTION: UpdateProperty()
//
//  PURPOSE: Called by the Ribbon framework when a command property (PKEY) needs to be updated.
//
//  COMMENTS:
//
//    This function is used to provide new command property values, such as labels, icons, or
//    tooltip information, when requested by the Ribbon framework.  
//    
//    In this SimpleRibbon sample, the method is not implemented.  
//
STDMETHODIMP CCommandHandler::UpdateProperty(
    UINT nCmdID,
    REFPROPERTYKEY key,
    const PROPVARIANT* ppropvarCurrentValue,
    PROPVARIANT* ppropvarNewValue)
{
    UNREFERENCED_PARAMETER(nCmdID);
    UNREFERENCED_PARAMETER(key);
    UNREFERENCED_PARAMETER(ppropvarCurrentValue);
    UNREFERENCED_PARAMETER(ppropvarNewValue);

    return E_NOTIMPL;
}

//
//  FUNCTION: Execute()
//
//  PURPOSE: Called by the Ribbon framework when a command is executed by the user.  For example, when
//           a button is pressed.
//
STDMETHODIMP CCommandHandler::Execute(
    UINT nCmdID,
    UI_EXECUTIONVERB verb,
    const PROPERTYKEY* key,
    const PROPVARIANT* ppropvarValue,
    IUISimplePropertySet* pCommandExecutionProperties)
{
    UNREFERENCED_PARAMETER(pCommandExecutionProperties);
    UNREFERENCED_PARAMETER(ppropvarValue);
    UNREFERENCED_PARAMETER(key);
    UNREFERENCED_PARAMETER(verb);
    
    switch (nCmdID) {
        case cmdButton_LoadModel: {
            std::wstring filePath;
            auto hr = basicFileOpen({ 
                {L"Graphic obj (*.obj)",       L"*.obj"},
                {L"All Documents (*.*)",       L"*.*"} }, filePath);

            if (auto app = getApplication(); SUCCEEDED(hr) && app) {
                // LoadModel for wide area
                std::filesystem::path objFilePath{ filePath };
                app->loadModel(objFilePath);
            }

            break;
        }
        
        case cmdButton_Clear: {
            if (auto app = getApplication(); app) {
                app->clearScene();
            }

            break;
        }
    }

    return S_OK;
}
