// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

#include "RibbonFramework.h"
#include "Application.h"

RibbonApplicationHolder holder{};

RibbonApplicationHolder::RibbonApplicationHolder() noexcept
    : iuiFramework{ nullptr }, application{ nullptr }
{}

RibbonApplicationHolder::~RibbonApplicationHolder() noexcept {
    Destroy();
}

RibbonApplicationHolder::RibbonApplicationHolder(RibbonApplicationHolder&& src) noexcept 
    : iuiFramework{ src.iuiFramework }, application{ src.application }
{
    src.iuiFramework = nullptr;
    src.application = nullptr;
}

RibbonApplicationHolder& RibbonApplicationHolder::operator = (RibbonApplicationHolder&& rhs) noexcept {
    if (this != &rhs) {
        Destroy();

        iuiFramework = rhs.iuiFramework;
        application = rhs.application;

        rhs.iuiFramework = nullptr;
        rhs.application = nullptr;
    }

    return *this;
}

//
//  FUNCTION: Create(HWND)
//
//  PURPOSE:  Initialize the Ribbon framework and bind a Ribbon to the application.
//
//  COMMENTS:
//
//    To get a Ribbon to display, the Ribbon framework must be initialized. 
//    This involves three important steps:
//      1) Instantiating the Ribbon framework object (CLSID_UIRibbonFramework).
//      2) Passing the host HWND and IUIApplication object to the framework.
//      3) Loading the binary markup compiled by UICC.exe.
//
//
bool RibbonApplicationHolder::Create(HWND hWnd)
{
    // Here we instantiate the Ribbon framework object.
    HRESULT hr = CoCreateInstance(CLSID_UIRibbonFramework, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&iuiFramework));
    if (FAILED(hr))
    {
        return false;
    }   

    // Next, we create the application object (IUIApplication) and call the framework Initialize method, 
    // passing the application object and the host HWND that the Ribbon will attach itself to.
    hr = Application::CreateInstance(hWnd, application);
    if (FAILED(hr))
    {
        Destroy();
        return false;
    } 

    hr = iuiFramework->Initialize(hWnd, application);
    if (FAILED(hr))
    {
        Destroy();

        return false;
    }

    // Finally, we load the binary markup.  This will initiate callbacks to the IUIApplication object 
    // that was provided to the framework earlier, allowing command handlers to be bound to individual
    // commands.
    hr = iuiFramework->LoadUI(GetModuleHandle(nullptr), TEXT("RIBBONLAYOUT_RIBBON"));
    if (FAILED(hr))
    {
        Destroy();
        return false;
    }

    return true;
}

//
//  FUNCTION: Destroy()
//
//  PURPOSE:  Tears down the Ribbon framework.
//
void RibbonApplicationHolder::Destroy() {
    DestroyFramework();
    DestroyApplication();
}

void RibbonApplicationHolder::DestroyFramework() {
    if (iuiFramework) {
        iuiFramework->Destroy();
        iuiFramework->Release();
        iuiFramework = nullptr;
    }
}

void RibbonApplicationHolder::DestroyApplication() {
    if (application) {
        application->Release();
        application = nullptr;
    }
}
