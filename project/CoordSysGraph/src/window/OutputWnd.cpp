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

#include "pch.h"
#include "framework.h"

#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"

#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// COutputList

COutputList::COutputList(IOutputListActionListener* outputListListener) noexcept
    : outputListListener_{ outputListListener }
{
}

COutputList::~COutputList() noexcept
{
}

COutputList::COutputList(COutputList&& src) noexcept
    : outputListListener_{ src.outputListListener_ }
{
    src.outputListListener_ = nullptr;
}

COutputList& COutputList::operator = (COutputList&& src) noexcept
{
    outputListListener_ = src.outputListListener_;
    src.outputListListener_ = nullptr;

    return *this;
}

BEGIN_MESSAGE_MAP(COutputList, CListBox)
    ON_WM_CONTEXTMENU()
    ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
    ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
    ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
    ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList message handlers

void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point) noexcept
{
    CMenu menu;
    menu.LoadMenu(IDR_OUTPUT_POPUP);

    CMenu* pSumMenu = menu.GetSubMenu(0);

    if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CFrameWndEx)))
    {
        CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

        if (!pPopupMenu->Create(this, point.x, point.y, pSumMenu->m_hMenu, FALSE, TRUE)) {
            delete pPopupMenu;

            return;
        }

        ((CFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
        UpdateDialogControls(this, TRUE);
    }

    SetFocus();
}

void COutputList::OnEditCopy() noexcept
{
    if (outputListListener_) {
        outputListListener_->onOutputListCopy(*this);
    }
}

void COutputList::OnEditClear() noexcept
{
    if (outputListListener_) {
        outputListListener_->onOutputListClear(*this);
    }
}

void COutputList::OnViewOutput() noexcept
{
    if (outputListListener_) {
        outputListListener_->onVisibilityChange(*this, FALSE);
    }
}


/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWnd::COutputWnd() noexcept
    : m_wndOutputBuild{ this }, m_wndOutputDebug{ this }
{
}

COutputWnd::~COutputWnd() noexcept
{
}

BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
    ON_WM_CREATE()
    ON_WM_SIZE()
END_MESSAGE_MAP()

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDockablePane::OnCreate(lpCreateStruct) == -1)
        return -1;

    CRect rectDummy;
    rectDummy.SetRectEmpty();

    // Create tabs window:
    if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
    {
        TRACE0("Failed to create output tab window\n");
        return -1;      // fail to create
    }

    // Create output panes:
    const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

    if (!m_wndOutputBuild.Create(dwStyle, rectDummy, &m_wndTabs, 2) ||
        !m_wndOutputDebug.Create(dwStyle, rectDummy, &m_wndTabs, 3))
    {
        TRACE0("Failed to create output windows\n");
        return -1;      // fail to create
    }

    UpdateFonts();

    CString strTabName;

    // Attach list windows to tab:
    auto bNameValid = strTabName.LoadString(IDS_BUILD_TAB);
    ASSERT(bNameValid);
    m_wndTabs.AddTab(&m_wndOutputBuild, strTabName, (UINT)0);
    bNameValid = strTabName.LoadString(IDS_DEBUG_TAB);
    ASSERT(bNameValid);
    m_wndTabs.AddTab(&m_wndOutputDebug, strTabName, (UINT)1);

    // Fill output tabs with some dummy text (nothing magic here)
    FillBuildWindow();
    FillDebugWindow();

    return 0;
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
    CDockablePane::OnSize(nType, cx, cy);

    // Tab control should cover the whole client area:
    m_wndTabs.SetWindowPos(nullptr, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputWnd::AdjustHorzScroll(CListBox& wndListBox)
{
    CClientDC dc{ this };
    CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

    int cxExtentMax = 0;

    for (int i = 0; i < wndListBox.GetCount(); i++)
    {
        CString strItem;
        wndListBox.GetText(i, strItem);

        cxExtentMax = (std::max)(cxExtentMax, (int)dc.GetTextExtent(strItem).cx);
    }

    wndListBox.SetHorizontalExtent(cxExtentMax);
    dc.SelectObject(pOldFont);
}

void COutputWnd::FillBuildWindow()
{
    m_wndOutputBuild.AddString(_T("Build output is being displayed here."));
    m_wndOutputBuild.AddString(_T("The output is being displayed in rows of a list view"));
    m_wndOutputBuild.AddString(_T("but you can change the way it is displayed as you wish..."));
}

void COutputWnd::FillDebugWindow()
{
    m_wndOutputDebug.AddString(_T("Debug output is being displayed here."));
    m_wndOutputDebug.AddString(_T("The output is being displayed in rows of a list view"));
    m_wndOutputDebug.AddString(_T("but you can change the way it is displayed as you wish..."));
}

void COutputWnd::UpdateFonts()
{
    m_wndOutputBuild.SetFont(&afxGlobalData.fontRegular);
    m_wndOutputDebug.SetFont(&afxGlobalData.fontRegular);
}

void COutputWnd::onOutputListCopy(CListBox& /*outputList*/) noexcept
{
    MessageBox(_T("Copy output"));
}

void COutputWnd::onOutputListClear(CListBox& /*outputList*/) noexcept
{
    MessageBox(_T("Clear output"));
}

void COutputWnd::onVisibilityChange(CListBox& /*outputList*/, BOOL bShow) noexcept
{
    CFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CFrameWndEx, GetTopLevelFrame());

    if (pMainFrame != nullptr)
    {
        pMainFrame->SetFocus();
        pMainFrame->ShowPane(this, bShow, FALSE, FALSE);
        pMainFrame->RecalcLayout();
    }
}
