// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface
// (the "Fluent UI") and is provided only as referential material to supplement the
// Microsoft Foundation Classes Reference and related electronic documentation
// included with the MFC C++ library software.
// License terms to copy, use or distribute the Fluent UI are available separately.
// To learn more about our Fluent UI licensing program, please visit
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// CoordSysGraphView.h : interface of the CCoordSysGraphView class
//

#pragma once

#include "CartesianCoordSys2D.h"
#include "LabelFormatter.hpp"
#include "windows_gdi_deleter.hpp"
#include "unique_rc.hpp"

class CCoordSysGraphView : public CScrollView
{
protected: // create from serialization only
	CCoordSysGraphView() noexcept;
	DECLARE_DYNCREATE(CCoordSysGraphView)

	// Attributes
public:
	CCoordSysGraphDoc* GetDocument() const noexcept;

	// Operations
public:

	// Overrides
public:
	void OnDraw(CDC* pDC) override;  // overridden to draw this view
	BOOL PreCreateWindow(CREATESTRUCT& cs) override;
protected:
	void OnInitialUpdate() override; // called first time after construct

	// Implementation
public:
	~CCoordSysGraphView() override;
#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif

public:
	using CoordSys2D = coordsys::CartesianCoordSys2D<int, float, float, coordsys::LabelFormatter<float>>;
private:
	raii::unique_rc<HBRUSH, raii::gdi_delete_object_nullptr<HBRUSH>> tickBrush_;
	raii::unique_rc<HPEN, raii::gdi_delete_object_nullptr<HPEN>> tickPen_;
	//coordsys::LabelFormatter<CoordSys2D::ClientUnit> labelFormatter_;
	CoordSys2D coord2D_;

	// Generated message map functions
protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point) noexcept;
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point) noexcept;
	afx_msg void OnSize(UINT nType, int cx, int cy) noexcept;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC) noexcept;

	DECLARE_MESSAGE_MAP()

private:
	void drawScene(HDC paintDC) noexcept;
	void invalidateScene(RECT clientRect) noexcept;
};

#ifndef _DEBUG  // debug version in CoordSysGraphView.cpp
inline CCoordSysGraphDoc* CCoordSysGraphView::GetDocument() const noexcept
{
	return static_cast<CCoordSysGraphDoc*>(m_pDocument);
}
#endif

