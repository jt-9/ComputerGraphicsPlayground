#ifndef MyCGRasterisationPolicy_h
#define MyCGRasterisationPolicy_h

#pragma once

#include "MyCGDefines.h"
#include "Vector.h"

#include <windows.h>
#include <concepts>

MYCG_NS_BEGIN

template<typename SU> requires std::convertible_to<SU, int>
class MyCGIRasterisationPolicy {
public:
	inline auto moveTo(HDC hdc, SU x, SU y, LPPOINT prevPt) const noexcept {
		return 0 != ::MoveToEx(hdc, static_cast<int>(x), static_cast<int>(y), prevPt);
	}

	inline auto lineTo(HDC hdc, SU x, SU y) const noexcept {
		if (mymtl::Vector2<SU> position; getCurrentPosition(hdc, position)) {
			auto colour = ::GetDCPenColor(hdc);
			return (colour != CLR_INVALID) && bresenhamLine2(hdc, static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(x), static_cast<int>(y), colour);
		}
		else {
			return false;
		}
	}

	inline auto line(HDC hdc, SU fromX, SU fromY, SU toX, SU toY) const noexcept {
		auto colour = ::GetDCPenColor(hdc);
		return (colour != CLR_INVALID) && bresenhamLine2(hdc, static_cast<int>(fromX), static_cast<int>(fromY), static_cast<int>(toX), static_cast<int>(toY), colour);
	}

	inline auto getCurrentPosition(HDC hdc, mymtl::Vector2<SU>& position) const noexcept {
		if (POINT pt; 0 != ::GetCurrentPositionEx(hdc, &pt)) {
			position.x = static_cast<SU>(pt.x);
			position.y = static_cast<SU>(pt.y);

			return true;
		}
		else {
			return false;
		}
	}

	inline auto setPixel(HDC hdc, SU x, SU y, unsigned long colour) const noexcept {
		return ::SetPixel(hdc, static_cast<int>(x), static_cast<int>(y), colour);
	}

private:
	inline auto bresenhamLine(HDC hdc, int x0, int y0, int x1, int y1, unsigned long colour) const {
		assert(hdc);

		auto isSteep = false;
		if (abs(x1 - x0) < abs(y1 - y0)) {
			std::swap(x0, y0);
			std::swap(x1, y1);

			isSteep = true;
		}

		if (x0 > x1) {
			std::swap(x0, x1);
			std::swap(y0, y1);
		}

		const auto dx = x1 - x0;
		const auto dy = y1 - y0;
		const auto deltaErrorY = 2 * dy;
		const auto incY = (y1 > y0) ? 1 : -1;
		if (isSteep) {
			for (auto x = x0, y = y0, error = 0; x <= x1; x++) {
				setPixel(hdc, y, x, colour);

				error += deltaErrorY;
				if (error > dx) {
					y += incY;
					error -= 2 * dx;
				}
			}
		}
		else {
			for (auto x = x0, y = y0, error = 0; x <= x1; x++) {
				setPixel(hdc, x, y, colour);

				error += deltaErrorY;
				if (error > dx) {
					y += incY;
					error -= 2 * dx;
				}
			}
		}

		return true;
	}

	inline bool bresenhamLine2(HDC hdc, int x0, int y0, int x1, int y1, unsigned long colour) const {
		assert(hdc);

		const auto dx = abs(x1 - x0);
		const auto dy = abs(y1 - y0);
		const auto incX = x1 >= x0 ? 1 : -1;
		const auto incY = y1 >= y0 ? 1 : -1;

		if (dy <= dx) {
			int d = (dy << 1) - dx;
			int d1 = dy << 1;
			int d2 = (dy - dx) << 1;

			setPixel(hdc, x0, y0, colour);

			for (int x = x0 + incX, y = y0, i = 1; i <= dx; i++, x += incX) {
				if (d > 0) {
					d += d2;
					y += incY;
				}
				else {
					d += d1;
				}

				setPixel(hdc, x, y, colour);
			}
		}
		else {
			int d = (dx << 1) - dy;
			int d1 = dx << 1;
			int d2 = (dx - dy) << 1;

			setPixel(hdc, x0, y0, colour);

			for (int y = y0 + incY, x = x0, i = 1; i <= dy; i++, y += incY) {
				if (d > 0) {
					d += d2;
					x += incX;
				}
				else
					d += d1;

				setPixel(hdc, x, y, colour);
			}
		}

		return true;
	}
};

MYCG_NS_END

#endif //MyCGRasterisationPolicy_h


