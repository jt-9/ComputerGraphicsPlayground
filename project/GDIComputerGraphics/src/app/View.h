#pragma once

#include "CoordSpaceCommon.h"
#include "CGModel.h"

#include <windows.h>
#include <UIRibbon.h>
#include <stdint.h>
#include <tuple>

class MainView
{	
public:
    //using ModelCoordinateType = float;
    using VisualModel = mycg::CGModel<float>;
    using ZBuffer = std::vector<int>;

    MainView(HWND window) noexcept;
    ~MainView() noexcept;

    MainView(MainView const&) = delete;
    MainView& operator = (MainView const&) = delete;

    HRESULT onViewChanged(UINT viewId,
        UI_VIEWTYPE typeId,
        IUnknown* pView,
        UI_VIEWVERB verb,
        INT uReasonCode) noexcept;

    void onDraw(HDC hdc) noexcept;
    void onSizeChanged(uint32_t width, uint32_t height) noexcept;
    void invalidate() noexcept;
    void setModel(VisualModel&& model, bool setClientSpaceToBoundingBox);
    void setModel(const VisualModel& model, bool setClientSpaceToBoundingBox);
    void clearScene();

private:
    void releaseRibbonView() noexcept;
    void drawScene(HDC paintDC) noexcept;
    void invalidateScene() noexcept;

    void resizeZBufferIfNeeded(uint32_t width, uint32_t height) noexcept;
private:
    HWND frameWnd;
    IUIRibbon* ribbon;
    unsigned int ribbonHeight;
    CoordSystem2D x0y;

    std::unique_ptr<VisualModel> displayedModel;
    std::tuple< ZBuffer, uint32_t, uint32_t> modelZBuffer;
    //std::optional<mymtl::BoundingBox3<VisualModel::CoordinateType>> 
};