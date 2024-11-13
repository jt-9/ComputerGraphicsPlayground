#ifndef WinRasterisationPolicies_h
#define WinRasterisationPolicies_h

// Defines policy which is used to draw pixels, lines, curves. CoordSpace delegates actual drawing to the policy

#include "CoordSpaceDefs.h"
#include "Vector.h"

#include "windows_gdi_select_object.hpp"
#include "windows_dc_restore.hpp"
#include "unique_rc.hpp"

#include <Windows.h>
#include <concepts>
#include <optional>
#include <string_view>

MY_COORD_SPACE_BEGIN

struct WinGDIRasteriser {
	MYMTL_INLINE explicit constexpr WinGDIRasteriser(HDC hdc) noexcept
		: hdc_{ hdc }
	{}

	constexpr WinGDIRasteriser(const WinGDIRasteriser&) noexcept = default;
	constexpr WinGDIRasteriser& operator = (const WinGDIRasteriser&) noexcept = default;

	constexpr WinGDIRasteriser(WinGDIRasteriser&&) noexcept = default;
	constexpr WinGDIRasteriser& operator = (WinGDIRasteriser&&) noexcept = default;

	constexpr ~WinGDIRasteriser() noexcept = default;

	MYMTL_INLINE auto moveTo(int x, int y) const noexcept {
		return 0 != MoveToEx(hdc_, x, y, nullptr);
	}

	template<typename U>
		requires std::convertible_to<U, int>
	MYMTL_INLINE constexpr auto moveTo(const mymtl::Vector2<U>& p) const noexcept {
		return moveTo(static_cast<int>(p.x), static_cast<int>(p.y));
	}

	MYMTL_INLINE auto lineTo(int x, int y) const noexcept {
		return 0 != LineTo(hdc_, x, y);
	}

	template<typename U>
		requires std::convertible_to<U, int>
	MYMTL_INLINE constexpr auto lineTo(const mymtl::Vector2<U>& p) const noexcept {
		return lineTo(static_cast<int>(p.x), static_cast<int>(p.y));
	}

	MYMTL_INLINE auto line(int fromX, int fromY, int toX, int toY) const noexcept {
		return moveTo(fromX, fromY) && lineTo(toX, toY);
	}

	template<typename U>
		requires std::convertible_to<U, int>
	MYMTL_INLINE constexpr auto line(const mymtl::Vector2<U>& from, const mymtl::Vector2<U>& to) const noexcept {
		return moveTo(from) && lineTo(to);
	}

	MYMTL_INLINE auto setPixel(int x, int y, unsigned long colour) const noexcept {
		return SetPixel(hdc_, x, y, colour);
	}

	template<typename U>
	MYMTL_INLINE constexpr auto getCurrentPosition(mymtl::Vector2<U>& pos) const noexcept {
		if (POINT pt; 0 != GetCurrentPositionEx(hdc_, &pt)) {
			pos.x = static_cast<U>(pt.x);
			pos.y = static_cast<U>(pt.y);

			return true;
		}
		else {
			return false;
		}
	}

	MYMTL_INLINE auto ellipse(int left, int top, int right, int bottom) const noexcept {
		return 0 != Ellipse(hdc_, left, top, right, bottom);
	}

	MYMTL_INLINE auto textOut(int x, int y, const char* str, std::uint16_t length) const noexcept {
		return 0 != TextOutA(hdc_, x, y, str, length);
	}

	MYMTL_INLINE auto textOut(int x, int y, std::string_view str) const noexcept {
		return textOut(x, y, str.data(), static_cast<std::uint16_t>(str.size()));
	}

	template<typename U>
		requires std::convertible_to<U, int>
	MYMTL_INLINE auto textOut(const mymtl::Vector2<U>& point, std::string_view str) const noexcept {
		return textOut(point.x, point.y, str);
	}

	MYMTL_INLINE auto textOut(int x, int y, const wchar_t* str, int length) const noexcept {
		return 0 != TextOutW(hdc_, x, y, str, length);
	}

	MYMTL_INLINE auto textOut(int x, int y, std::wstring_view str) const noexcept {
		return textOut(x, y, str.data(), static_cast<std::uint16_t>(str.size()));
	}

	template<typename U>
		requires std::convertible_to<U, int>
	MYMTL_INLINE auto textOut(const mymtl::Vector2<U>& point, std::wstring_view str) const noexcept {
		return textOut(point.x, point.y, str);
	}

	[[nodiscard]] MYMTL_INLINE unsigned int getTextAlign() const noexcept {
		return GetTextAlign(hdc_);
	}

	MYMTL_INLINE unsigned int setTextAlign(unsigned int align) const noexcept {
		return SetTextAlign(hdc_, align);
	}

	MYMTL_INLINE constexpr auto selectObjectResource(HGDIOBJ o) const noexcept {
		return raii::unique_rc<HGDIOBJ, raii::gdi_select_object_nullptr<HGDIOBJ>>{ SelectObject(hdc_, o), raii::gdi_select_object_nullptr<HGDIOBJ>{hdc_} };
	}

	MYMTL_INLINE auto selectObject(HGDIOBJ o) const noexcept {
		return SelectObject(hdc_, o);
	}

	MYMTL_INLINE constexpr auto saveStateRAII() const noexcept {
		return raii::unique_rc<HDC, raii::gdi_restore_dc_nullptr>{ hdc_, raii::gdi_restore_dc_nullptr{ SaveDC(hdc_) } };
	}
private:
	HDC hdc_; // non owning, caller is responsible for passing correct HDC
};

MY_COORD_SPACE_END

#endif //!WinRasterisationPolicies_h