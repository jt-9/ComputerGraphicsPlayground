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

// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "OutputWnd.h"
#include "CoordSysRibbonHelper.hpp"

class CMainFrame : public CFrameWndEx
{
    
protected: // create from serialization only
    CMainFrame() noexcept;
    DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
    BOOL PreCreateWindow(CREATESTRUCT& cs) override;

// Implementation
public:
    ~CMainFrame() override;
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
    CMFCRibbonBar     m_wndRibbonBar;
    CMFCRibbonApplicationButton m_MainButton;
    CMFCToolBarImages m_PanelImages;
    CMFCRibbonStatusBar  m_wndStatusBar;
    COutputWnd        m_wndOutput;
    CoordSys2DRibbonEditControls m_ctrlRibbonEdit;

// Generated message map functions
protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnApplicationLook(UINT id);
    afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
    afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
    DECLARE_MESSAGE_MAP()

    BOOL CreateDockingWindows() noexcept;
    void SetDockingWindowIcons(BOOL bHiColorIcons) noexcept;
private:
    [[nodiscard]] CoordSys2DRibbonEditControls initRibbonEditControls() const noexcept;

public:
    afx_msg void OnEditCoordXmin();
    afx_msg void OnEditCoordYmin();
    afx_msg void OnEditCoordXmax();
    afx_msg void OnEditCoordYmax();
};


