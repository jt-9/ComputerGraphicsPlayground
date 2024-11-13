#ifndef MYGDI_MEMORY_DC_HPP
#define MYGDI_MEMORY_DC_HPP

#pragma once

#include "windows_dc_deleter.hpp"
#include "windows_gdi_deleter.hpp"
#include "unique_rc.hpp"

#include <array>
#include <cmath>
#include <cassert>


class MemoryDC
{
private:
    [[nodiscard]] inline static constexpr auto rectToCornerPoints(RECT const& rect) noexcept {
        return std::array{ POINT{ rect.left, rect.top }, POINT{ rect.right, rect.bottom } };
    }

    [[nodiscard]] inline static auto areDeviceCapsSupported(HDC hdc, int index, int capabilitiesMask) noexcept {
        return (GetDeviceCaps(hdc, index) & capabilitiesMask) != 0;
    }

    [[nodiscard]] inline static HBITMAP createMemoryBitmap(HDC hdc, RECT const& rect) noexcept {
        auto rectCorners = rectToCornerPoints(rect);
        LPtoDP(hdc, rectCorners.data(), static_cast<int>(rectCorners.size()));

        return CreateCompatibleBitmap(hdc, std::abs(rectCorners[1].x - rectCorners[0].x), std::abs(rectCorners[1].y - rectCorners[0].y));
    }

public:
    MemoryDC(MemoryDC&&) = default;
    MemoryDC& operator = (MemoryDC&&) = default;

    MemoryDC(MemoryDC const&) = delete;
    MemoryDC& operator = (MemoryDC const&) = delete;


    [[nodiscard]] constexpr inline HDC getDC() const noexcept {
        return memoryDC.get();
    }

    [[nodiscard]] constexpr inline HBITMAP getBitmap() const noexcept {
        return memoryBitmap.get();
    }

    inline MemoryDC(HDC hdc, RECT const& rect) noexcept
        : memoryBitmap{ createMemoryBitmap(hdc, rect) }, defaultBitmap{ nullptr }, memoryDC{ CreateCompatibleDC(hdc) }, drawRect{ rect } {

        assert(areDeviceCapsSupported(hdc, RASTERCAPS, RC_BITBLT) && "Device context must support raster operation bit blit");

        defaultBitmap = static_cast<HBITMAP>(SelectObject(memoryDC.get(), memoryBitmap.get()));
        if (!defaultBitmap) {
            // No need to continue setting up if failed to select memory bitmap into device context
            return;
        }

        SetMapMode(memoryDC.get(), GetMapMode(hdc));
        SetGraphicsMode(memoryDC.get(), GetGraphicsMode(hdc));

        {
            XFORM xForm;
            if (GetWorldTransform(hdc, &xForm)) {
                SetWorldTransform(memoryDC.get(), &xForm);
            }
        }

        {
            SIZE windowParamSize;
            GetWindowExtEx(hdc, &windowParamSize);
            SetWindowExtEx(memoryDC.get(), windowParamSize.cx, windowParamSize.cy, nullptr);

            GetViewportExtEx(hdc, &windowParamSize);
            SetViewportExtEx(memoryDC.get(), windowParamSize.cx, windowParamSize.cy, nullptr);
        }

        SetWindowOrgEx(memoryDC.get(), drawRect.left, drawRect.top, nullptr);
    }

    // Destructors are called in reverse order, so memory device context will be deleted first, then bitmap
    constexpr ~MemoryDC() = default;

    inline bool fillBackground(HBRUSH brush) const noexcept {
        return FillRect(memoryDC.get(), &drawRect, brush) != 0;
    }

    inline bool fillBackgroundSolidColour(COLORREF colour) const noexcept
    {
        using GDIBrushHandle = raii::unique_rc<HBRUSH, raii::gdi_delete_object_nullptr<HBRUSH>>;
        const GDIBrushHandle solidBrush{ CreateSolidBrush(colour) };
        return fillBackground(solidBrush.get());
    }

    inline bool bitBlt(HDC dstHDC, DWORD rasterOp = SRCCOPY) const noexcept
    {
        return memoryDC &&
            BitBlt(dstHDC, drawRect.left, drawRect.top, std::abs(drawRect.right - drawRect.left), std::abs(drawRect.bottom - drawRect.top), memoryDC.get(), drawRect.left, drawRect.top, rasterOp) != 0;
    }

    inline constexpr void detachBitmap() noexcept {
        if (memoryDC && defaultBitmap) {
            SelectObject(memoryDC.get(), defaultBitmap);
            defaultBitmap = nullptr;
        }
    }

private:
    raii::unique_rc<HBITMAP, raii::gdi_delete_object_nullptr<HBITMAP>> memoryBitmap;
    HBITMAP defaultBitmap;
    raii::unique_rc<HDC, raii::gdi_delete_dc_nullptr> memoryDC;
    RECT drawRect;
};

#endif // !MYGDI_MEMORY_DC_HPP