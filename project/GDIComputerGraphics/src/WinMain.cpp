// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

// SimpleRibbon.cpp : Defines the entry point for the application.
//

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files.
#include <windows.h>

#include "resource.h"
#include "RibbonFramework.h"

#include <array>

namespace {
    // Forward declarations of functions included in this code module:
    //
    //  FUNCTION: MyRegisterClass()
    //
    //  PURPOSE: Registers the window class.
    //
    //  COMMENTS:
    //
    //    This function and its usage are only necessary if you want this code
    //    to be compatible with Win32 systems prior to the 'RegisterClassEx'
    //    function that was added to Windows 95. It is important to call this function
    //    so that the application has 'well formed' small icons associated
    //    with it.
    //
    //
    inline auto MyRegisterClass(HINSTANCE hInstance, LPCTSTR windowClass, WNDPROC wndProc ) noexcept {
        WNDCLASSEX wcex{
            .cbSize = sizeof(WNDCLASSEX),
            .style = 0,   // Don't use CS_HREDRAW or CS_VREDRAW with a Ribbon
            .lpfnWndProc = wndProc,
            .cbClsExtra = 0,
            .cbWndExtra = 0,
            .hInstance = hInstance,
            .hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SIMPLERIBBON)),
            .hCursor = LoadCursor(hInstance, IDC_ARROW),
            .hbrBackground = nullptr,
            .lpszMenuName = nullptr,
            .lpszClassName = windowClass,
            .hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL))
        };

        return RegisterClassEx(&wcex);
    }

    inline auto MyUnregisterClass(HINSTANCE hInstance, LPCTSTR wszWindowClass) noexcept {
        return UnregisterClass(wszWindowClass, hInstance);
    }

    //
    //   FUNCTION: InitInstance(HINSTANCE, int)
    //
    //   PURPOSE: Saves instance handle and creates main window.
    //
    //   COMMENTS:
    //
    //        In this function, an instance handle is saved in a global variable and
    //        create and display the main program window.
    //
    //
    inline auto InitInstance(HINSTANCE hInstance, LPCTSTR wszWindowClass, LPCTSTR wszTitle, int nCmdShow) noexcept {
        // hInst = hInstance; // Store instance handle in our global variable.

        const auto hWnd = CreateWindow(wszWindowClass, wszTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
            CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

        if (!hWnd) {
            //auto stringError = TEXT("Error creating window, code: ") + std::to_wstring(GetLastError());
            //OutputDebugString(stringError.data());

            return false;
        }

        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);

        return true;
    }

}

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    HRESULT hr = CoInitialize(nullptr);
    if (FAILED(hr)) {
        return FALSE;
    }

    const int MAX_LENGTH_LOADSTRING = 160;
    std::array<TCHAR, MAX_LENGTH_LOADSTRING> appTitle, windowClassName;

    // Initialize global strings.
    LoadString(hInstance, IDS_APP_TITLE, appTitle.data(), static_cast<int>(appTitle.size()));
    LoadString(hInstance, IDC_SIMPLERIBBON, windowClassName.data(), static_cast<int>(windowClassName.size()));

    const auto classAtom = MyRegisterClass(hInstance, windowClassName.data(), WndProc);
    if (!classAtom) {
        return FALSE;
    }

    // Perform application initialization.
    if (!InitInstance(hInstance, windowClassName.data(), appTitle.data(), nCmdShow)) {
        MyUnregisterClass(hInstance, windowClassName.data());
        return FALSE;
    }

    // Main message loop.
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    CoUninitialize();
    MyUnregisterClass(hInstance, windowClassName.data());

    return (int) msg.wParam;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu.
//  WM_PAINT    - Paint the main window.
//  WM_DESTROY  - post a quit message and return.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    //int wmId, wmEvent;    
    PAINTSTRUCT ps;

    LRESULT messageResult = 0;

    switch (message) {
        case WM_CREATE: {
            // Initializes the Ribbon framework.
            auto initSuccess = holder.Create(hWnd);
            if (!initSuccess)
            {
                return -1;
            }
            break;
        }
        case WM_PAINT: {
            auto hdc = BeginPaint(hWnd, &ps);
        
            holder.getApplication()->getMainView()->onDraw(hdc);
        
            EndPaint(hWnd, &ps);
            break;
        }
        case WM_SIZE: {
        
            //auto width = LOWORD(lParam);
            //auto height = HIWORD(lParam);
            holder.getApplication()->getMainView()->invalidate();

            break;
        }
        //case WM_EXITSIZEMOVE: {
        //    holder.getApplication()->getMainView()->invalidate();

        //    break;
        //}
        case WM_DESTROY: {
            // Tears down the Ribbon framework.
            holder.Destroy();
            PostQuitMessage(0);
            break;
        }
        default:
            messageResult = DefWindowProc(hWnd, message, wParam, lParam);
    }

    return messageResult;
}
