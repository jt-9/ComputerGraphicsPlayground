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

#pragma once


class IOutputListActionListener {
public:
    virtual void onOutputListCopy(CListBox& outputList) noexcept = 0;
    virtual void onOutputListClear(CListBox& outputList) noexcept = 0;
    virtual void onVisibilityChange(CListBox& outputList, BOOL bShow) noexcept = 0;
};

/////////////////////////////////////////////////////////////////////////////
// COutputList window

class COutputList : public CListBox
{
// Construction
public:
    COutputList(IOutputListActionListener* outputListListener) noexcept;
    ~COutputList() noexcept override;

    COutputList(COutputList&&) noexcept;
    COutputList& operator = (COutputList&&) noexcept;

    COutputList(const COutputList&) = delete;
    COutputList& operator = (const COutputList&) = delete;

// Implementation
public:

protected:
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point) noexcept;
    afx_msg void OnEditCopy() noexcept;
    afx_msg void OnEditClear() noexcept;
    afx_msg void OnViewOutput() noexcept;

    DECLARE_MESSAGE_MAP()

private:
    IOutputListActionListener* outputListListener_;
};

class COutputWnd : public CDockablePane, private IOutputListActionListener
{
// Construction
public:
    COutputWnd() noexcept;

    void UpdateFonts();

// Attributes
protected:
    CMFCTabCtrl	m_wndTabs;

    COutputList m_wndOutputBuild;
    COutputList m_wndOutputDebug;

protected:
    void FillBuildWindow();
    void FillDebugWindow();

    void AdjustHorzScroll(CListBox& wndListBox);

// Implementation
public:
    ~COutputWnd() noexcept override;

private:
    void onOutputListCopy(CListBox& outputList) noexcept override;
    void onOutputListClear(CListBox& outputList) noexcept override;
    void onVisibilityChange(CListBox& outputList, BOOL bShow) noexcept override;

protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);

    DECLARE_MESSAGE_MAP()
};

