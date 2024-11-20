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

// CoordSysGraphView.cpp : implementation of the CCoordSysGraphView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "CoordSysGraph.h"
#endif

#include "CoordSysGraphDoc.h"
#include "CoordSysGraphView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "ApeMath.h"
#include "MemoryDC.hpp"

#include <string_view>

namespace {
    template<typename Unit>
    inline constexpr auto computePadding(Unit length, Unit desiredindent) noexcept {
        return (length > (desiredindent + desiredindent)) ? desiredindent : static_cast<Unit>(0);
    }

    using namespace std::literals;
    constexpr auto coordLabelFormat = "{:g}"sv;

    inline constexpr const auto kConvertBufferSize = 64;

    inline constexpr auto axisNameAndIndexForEdit(UINT id) noexcept {
        using coordsys::AxisName;

        switch (id) {
        case ID_EDIT_COORD_XMIN:
            return std::make_pair(AxisName::Abscissa, CoordSys2D::Axis::PointIndex::Start);
        case ID_EDIT_COORD_YMIN:
            return std::make_pair(AxisName::Ordinate, CoordSys2D::Axis::PointIndex::Start);
        case ID_EDIT_COORD_XMAX:
            return std::make_pair(AxisName::Abscissa, CoordSys2D::Axis::PointIndex::End);
        case ID_EDIT_COORD_YMAX:
            return std::make_pair(AxisName::Ordinate, CoordSys2D::Axis::PointIndex::End);

        default:
        {
            TRACE0("Failed to find branch with id provided\n");
            std::abort();
        }
        }
    }

    // Space maps top left, bottom left, and bottom right points
    inline constexpr typename CoordSys2D::Space::ClientSpace updateClientSpace(const typename CoordSys2D::Space::ClientSpace& oldSpace, CoordSys2D::ClientUnit value, coordsys::AxisName axisName, typename CoordSys2D::Axis::PointIndex axisPointIndex) noexcept {
        using coordsys::AxisName;

        CoordSys2D::Space::ClientSpace newSpace{ oldSpace.pts_ };

        if (CoordSys2D::Axis::PointIndex::Start == axisPointIndex) {
            // Xmin or Ymin
            if (AxisName::Abscissa == axisName) {
                newSpace.pt1().x = newSpace.pt2().x = value;
            }
            else if (AxisName::Ordinate == axisName) {
                newSpace.pt2().y = newSpace.pt3().y = value;
            }
        }
        else {
            if (AxisName::Abscissa == axisName) {
                newSpace.pt3().x = value;
            }
            else if (AxisName::Ordinate == axisName) {
                newSpace.pt1().y = value;
            }
        }

        return newSpace;
    }
}

// CCoordSysGraphView

IMPLEMENT_DYNCREATE(CCoordSysGraphView, CScrollView)

BEGIN_MESSAGE_MAP(CCoordSysGraphView, CScrollView)
    ON_WM_CONTEXTMENU()
    ON_WM_RBUTTONUP()
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
    //    ON_COMMAND(ID_EDIT_COORD_XMIN, &CCoordSysGraphView::OnEditCoordXmin)
END_MESSAGE_MAP()

// CCoordSysGraphView construction/destruction

CCoordSysGraphView::CCoordSysGraphView() noexcept
    : tickBrush_{ static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)) }, tickPen_{ /*static_cast<HPEN>(CreatePen(PS_SOLID, 1, RGB(0, 160, 0)))*/ },
    coord2D_{ CoordSys2D::Space::ClientSpace{{-1, 1}, {-1, -1}, {1, -1}},
        mymtl::identity_matrix<CoordSys2D::TransfUnit, CoordSys2D::Space::kDimensions + 1>(), {},
        std::optional{std::array{
            std::optional{ typename CoordSys2D::AxisAttributes{.pen = tickPen_.get()} },
            std::optional{ typename CoordSys2D::AxisAttributes{.pen = tickPen_.get()} }
        }},
        std::optional{std::array{
            std::optional{ typename CoordSys2D::TickAttributes{0.1F, 5, 5, tickPen_.get(), tickBrush_.get()} },
            std::optional{ typename CoordSys2D::TickAttributes{0.1F, 5, 5, tickPen_.get(), tickBrush_.get()} }
        }},
        std::optional{std::array{
            std::optional{ typename CoordSys2D::LabelAttributes{0.1F, coordsys::LabelFormatter<CoordSys2D::ClientUnit>{coordLabelFormat}, CoordSys2D::ScreenVector{0, 12}, (TA_TOP | TA_NOUPDATECP | TA_CENTER), nullptr, 0, 0} },
            std::optional{ typename CoordSys2D::LabelAttributes{0.1F, coordsys::LabelFormatter<CoordSys2D::ClientUnit>{coordLabelFormat}, CoordSys2D::ScreenVector{8, -16}, (TA_TOP | TA_NOUPDATECP | TA_CENTER), nullptr, 0, 0.1F} }
        }}
    }
{
    // TODO: add construction code here
}

CCoordSysGraphView::~CCoordSysGraphView()
{
}

BOOL CCoordSysGraphView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

    return CScrollView::PreCreateWindow(cs);
}

// CCoordSysGraphView drawing

void CCoordSysGraphView::OnDraw(CDC* pDC)
{
    CCoordSysGraphDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    RECT clientRect;
    pDC->GetClipBox(&clientRect);

    //GetClientRect(&clientRect);
    MemoryDC memDC{ pDC->GetSafeHdc(), clientRect };
    // RGB(100, 100, 100)
    memDC.fillBackgroundSolidColour(pDC->GetBkColor());

    drawScene(memDC.getDC());
    memDC.bitBlt(pDC->GetSafeHdc());
}

void CCoordSysGraphView::OnInitialUpdate() {
    CScrollView::OnInitialUpdate();

    CSize sizeTotal;
    // TODO: calculate the total size of this view
    sizeTotal.cx = sizeTotal.cy = 100;
    SetScrollSizes(MM_TEXT, sizeTotal);

    auto* ribbonProvider = static_cast<IRibbonCtrlProvider*>(static_cast<CMainFrame*>(GetParentFrame()));
    setupRibbonCtrls(*ribbonProvider);
}

void CCoordSysGraphView::OnRButtonUp(UINT /* nFlags */, CPoint point) noexcept {
    ClientToScreen(&point);
    OnContextMenu(this, point);
}

void CCoordSysGraphView::OnContextMenu(CWnd* /* pWnd */, CPoint point) noexcept {
#ifndef SHARED_HANDLERS
    theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCoordSysGraphView diagnostics

#ifdef _DEBUG
void CCoordSysGraphView::AssertValid() const
{
    CScrollView::AssertValid();
}

void CCoordSysGraphView::Dump(CDumpContext& dc) const
{
    CScrollView::Dump(dc);
}

// non-debug version is inline
CCoordSysGraphDoc* CCoordSysGraphView::GetDocument() const noexcept {
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCoordSysGraphDoc)));
    return static_cast<CCoordSysGraphDoc*>(m_pDocument);
}
#endif //_DEBUG


// CCoordSysGraphView message handlers
void CCoordSysGraphView::OnSize(UINT nType, int cx, int cy) noexcept {
    CScrollView::OnSize(nType, cx, cy);

    //RECT r;
    //GetClientRect(&r);

    invalidateScene({ .left = 0, .top = 0, .right = cx, .bottom = cy });
}

BOOL CCoordSysGraphView::OnEraseBkgnd(CDC*) noexcept {
    //Return nonzero in response to WM_ERASEBKGND if it processes the message and erases the background; this indicates that no further erasing is required
    return TRUE;
}

void CCoordSysGraphView::onCommand(CMFCRibbonBar& /*ribbon*/, CMFCRibbonEdit& edit) noexcept
{
    const CString inputEditTextCopy = edit.GetEditText();

    if (inputEditTextCopy.IsEmpty()) {
        setBoundingValueToEditField(edit);

        return;
    }

    // Edit text is not empty, so parse it
    const std::string rawInput{ CT2AEX<kConvertBufferSize>{inputEditTextCopy.GetString()} };

    CoordSys2D::ClientUnit axisEndValue{};
    const auto* const ptrToLast = rawInput.data() + rawInput.size();
    const auto [ptr, parseError] = std::from_chars(rawInput.data(), ptrToLast, axisEndValue);

    // If success set into field and redraw
    if ((parseError == std::errc{}) && (ptrToLast == ptr)) {
        auto [axisName, axisPointIndex] = axisNameAndIndexForEdit(edit.GetID());
        coord2D_.setAxisEnd(axisEndValue, axisName, axisPointIndex, true);
        
        coord2D_.setClientSpace(updateClientSpace(coord2D_.getClientSpace(), axisEndValue, axisName, axisPointIndex), true);

        Invalidate(FALSE);
    }
    else {
        TRACE1("Edit text error parse %d\n", parseError);
        setBoundingValueToEditField(edit);
    }
}

void CCoordSysGraphView::setBoundingValueToEditField(CMFCRibbonEdit& edit) const noexcept {
    const coordsys::LabelFormatter<CoordSys2D::ClientUnit> formatter{ coordLabelFormat };

    auto [axisName, pointIndex] = axisNameAndIndexForEdit(edit.GetID());
    const auto formattedValue = formatter(coord2D_.getAxisEnd(axisName, pointIndex));
    
    edit.SetEditText(CString{ CA2TEX<kConvertBufferSize>{formattedValue.data()} });
}

void CCoordSysGraphView::setupRibbonCtrls(const IRibbonCtrlProvider& provider) const noexcept
{
    setBoundingValueToEditField(provider.getRibbonEdit(ID_EDIT_COORD_XMIN));
    setBoundingValueToEditField(provider.getRibbonEdit(ID_EDIT_COORD_YMIN));
    setBoundingValueToEditField(provider.getRibbonEdit(ID_EDIT_COORD_XMAX));
    setBoundingValueToEditField(provider.getRibbonEdit(ID_EDIT_COORD_YMAX));
}

void CCoordSysGraphView::invalidateScene(RECT clientRect) noexcept {
    const auto cx = static_cast<CoordSys2D::ScreenUnit>(clientRect.right - clientRect.left), cy = static_cast<CoordSys2D::ScreenUnit>(clientRect.bottom - clientRect.top);

    const auto indentX = computePadding(cx, 20);
    const auto indentY = computePadding(cy, 20);

    coord2D_.setScreenSpace(CoordSys2D::Space::ScreenSpace{ {clientRect.left + indentX, clientRect.top + indentY},
        {clientRect.left + indentX, clientRect.top + cy - indentY}, {clientRect.left + cx - indentX, clientRect.top + cy - indentY} }, true);

    InvalidateRect(&clientRect, FALSE);
}

void CCoordSysGraphView::drawScene(HDC paintDC) noexcept {
    coordsys::Rasteriser rasteriser{ paintDC };

    coord2D_.draw(rasteriser);
    //coord2D_.line(paintDC, 0, 0, 0.5F, 1);
    coord2D_.arc(rasteriser, 0.0F, 0.5F, 0.5, -0.25F, 0.25F, 0.375F, 0.25F, 0.375F);
}
