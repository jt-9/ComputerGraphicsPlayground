#ifndef WinRasterisationPolicies_h
#define WinRasterisationPolicies_h

// Defines policy which is used to draw pixels, lines, curves. CoordSpace delegates actual drawing to the policy

#include "CoordSpaceDefs.h"
#include "Vector.h"
#include "unique_rc.hpp"
#include "windows_gdi_select_object.hpp"

#include <windows.h>
#include <concepts>
#include <optional>
#include <string_view>

MY_COORD_SPACE_BEGIN

struct WinGDIRasteriser {
	MYMTL_INLINE auto moveTo(HDC hdc, int x, int y) const noexcept {
		return 0 != MoveToEx(hdc, x, y, nullptr);
	}

	template<typename U>
		requires std::convertible_to<U, int>
	MYMTL_INLINE constexpr auto moveTo(HDC hdc, const mymtl::Vector2<U>& p) const noexcept {
		return moveTo(hdc, static_cast<int>(p.x), static_cast<int>(p.y));
	}

	MYMTL_INLINE auto lineTo(HDC hdc, int x, int y) const noexcept {
		return 0 != LineTo(hdc, x, y);
	}

	template<typename U>
		requires std::convertible_to<U, int>
	MYMTL_INLINE constexpr auto lineTo(HDC hdc, const mymtl::Vector2<U>& p) const noexcept {
		return lineTo(hdc, static_cast<int>(p.x), static_cast<int>(p.y));
	}

	MYMTL_INLINE auto line(HDC hdc, int fromX, int fromY, int toX, int toY) const noexcept {
		return moveTo(hdc, fromX, fromY) && lineTo(hdc, toX, toY);
	}

	template<typename U>
		requires std::convertible_to<U, int>
	MYMTL_INLINE constexpr auto line(HDC hdc, const mymtl::Vector2<U>& from, const mymtl::Vector2<U>& to) const noexcept {
		return moveTo(hdc, from) && lineTo(hdc, to);
	}

	MYMTL_INLINE auto setPixel(HDC hdc, int x, int y, unsigned long colour) const noexcept {
		return SetPixel(hdc, x, y, colour);
	}

	template<typename U>
	MYMTL_INLINE constexpr auto getCurrentPosition(HDC hdc, mymtl::Vector2<U>& pos) const noexcept {
		if (POINT pt; 0 != GetCurrentPositionEx(hdc, &pt)) {
			pos.x = static_cast<U>(pt.x);
			pos.y = static_cast<U>(pt.y);

			return true;
		}
		else {
			return false;
		}
	}

	MYMTL_INLINE auto ellipse(HDC hdc, int left, int top, int right, int bottom) const noexcept {
		return 0 != Ellipse(hdc, left, top, right, bottom);
	}

	MYMTL_INLINE auto textOut(HDC hdc, int x, int y, const char* str, std::uint16_t length) const noexcept {
		return 0 != TextOutA(hdc, x, y, str, length);
	}

	MYMTL_INLINE auto textOut(HDC hdc, int x, int y, std::string_view str) const noexcept {
		return textOut(hdc, x, y, str.data(), static_cast<std::uint16_t>(str.size()));
	}

	template<typename U>
		requires std::convertible_to<U, int>
	MYMTL_INLINE auto textOut(HDC hdc, const mymtl::Vector2<U>& point, std::string_view str) const noexcept {
		return textOut(hdc, point.x, point.y, str);
	}

	MYMTL_INLINE auto textOut(HDC hdc, int x, int y, const wchar_t* str, int length) const noexcept {
		return 0 != TextOutW(hdc, x, y, str, length);
	}

	MYMTL_INLINE auto textOut(HDC hdc, int x, int y, std::wstring_view str) const noexcept {
		return textOut(hdc, x, y, str.data(), static_cast<std::uint16_t>(str.size()));
	}

	template<typename U>
		requires std::convertible_to<U, int>
	MYMTL_INLINE auto textOut(HDC hdc, const mymtl::Vector2<U>& point, std::wstring_view str) const noexcept {
		return textOut(hdc, point.x, point.y, str);
	}

	[[nodiscard]] MYMTL_INLINE unsigned int getTextAlign(HDC hdc) const noexcept {
		return GetTextAlign(hdc);
	}

	MYMTL_INLINE unsigned int setTextAlign(HDC hdc, unsigned int align) const noexcept {
		return SetTextAlign(hdc, align);
	}

	MYMTL_INLINE constexpr auto selectObjectResource(HDC hdc, HGDIOBJ o) const noexcept {
		return raii::unique_rc<HGDIOBJ, raii::gdi_select_object_nullptr<HGDIOBJ>>{ SelectObject(hdc, o), raii::gdi_select_object_nullptr<HGDIOBJ>{hdc} };
	}

	MYMTL_INLINE auto selectObject(HDC hdc, HGDIOBJ o) const noexcept {
		return SelectObject(hdc, o);
	}
};

MY_COORD_SPACE_END

#endif //!WinRasterisationPolicies_h