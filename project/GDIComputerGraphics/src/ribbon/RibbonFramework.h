// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

//
// RibbonFrameword.h/cpp implements the initialization and tear down of ribbon framework.
//

#pragma once

#include "Application.h"
#include <UIRibbon.h>

class RibbonApplicationHolder final {
public:
	// Methods to facilitate the initialization and destruction of the Ribbon framework.
	bool Create(HWND hWnd);
	void Destroy();

	RibbonApplicationHolder() noexcept;
	~RibbonApplicationHolder() noexcept;

	RibbonApplicationHolder(RibbonApplicationHolder&&) noexcept;
	RibbonApplicationHolder& operator = (RibbonApplicationHolder&&) noexcept;

	RibbonApplicationHolder(RibbonApplicationHolder const&) = delete;
	RibbonApplicationHolder& operator = (RibbonApplicationHolder const&) = delete;

	inline Application* getApplication() const {
		return application;
	}

private:
	void DestroyFramework();
	void DestroyApplication();

private:
	IUIFramework* iuiFramework;  // Reference to the Ribbon framework.
	Application* application;  // Reference to the Application object.
};

extern RibbonApplicationHolder holder;

inline auto getApplication() noexcept {
	return holder.getApplication();
}