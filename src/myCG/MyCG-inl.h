#ifndef MyCG_inl
#define MyCG_inl

#pragma once

#include "MyCG.h"
#include "Vector.h"

#include <windows.h>
#include <utility>
#include <cassert>
#include <utility>

MYCG_NS_BEGIN

namespace {
	inline bool isDegenerateYTriangle(int y0, int y1, int y2) {
		return (y0 == y1) && (y0 == y2);
	}

	inline void fillDegenerateTriangle(HDC hdc, int x0, int x1, int x2, int y, unsigned long colour) {
		const auto minX = (std::min)(x0, (std::min)(x1, x2));
		const auto maxX = (std::max)(x0, (std::max)(x1, x2));

		for (auto x = minX; x <= maxX; x++) {
			::SetPixel(hdc, x, y, colour);
		}
	}
}

inline void fillTriangle(HDC hdc, int x0, int y0, int x1, int y1, int x2, int y2, unsigned long colour) noexcept {
	assert(hdc);

	if (isDegenerateYTriangle(y0, y1, y2)) {
		fillDegenerateTriangle(hdc, x0, x1, x2, y0, colour);
		return;
	}

	// Sort by y
	if (y0 > y1) {
		std::swap(y0, y1);
		std::swap(x0, x1);
	}

	if (y0 > y2) {
		std::swap(y0, y2);
		std::swap(x0, x2);
	}

	if (y1 > y2) {
		std::swap(y1, y2);
		std::swap(x1, x2);
	}

	const auto triangleHeight = static_cast<double>(y2) - y0 + 1;

	{
		double const lowerSegmentTriangleHeight = static_cast<double>(y1) - y0 + 1;
		for (auto y = y0; y < y1; y++) {
			const auto alpha = (y - y0) / triangleHeight;
			const auto beta = (y - y0) / lowerSegmentTriangleHeight;

			auto startX = static_cast<std::remove_cvref_t<decltype(x0)>>(x0 + (x2 - x0) * alpha);
			auto endX = static_cast<std::remove_cvref_t<decltype(x0)>>(x0 + (x1 - x0) * beta);

			if (startX > endX) {
				std::swap(startX, endX);
			}

			for (auto x = startX; x <= endX; x++) {
				::SetPixel(hdc, x, y, colour);
			}
		}
	}

	{
		double const upperSegmentTriangleHeight = static_cast<double>(y2) - y1 + 1;
		for (auto y = y1; y <= y2; y++) {
			const auto alpha = (static_cast<std::remove_cvref_t<decltype(triangleHeight)>>(y) - y0) / triangleHeight;
			const auto beta = (static_cast<std::remove_cvref_t<decltype(upperSegmentTriangleHeight)>>(y) - y1) / upperSegmentTriangleHeight;

			auto startX = static_cast<std::remove_cvref_t<decltype(x0)>>(x0 + (x2 - x0) * alpha);
			auto endX = static_cast<std::remove_cvref_t<decltype(x1)>>(x1 + (x2 - x1) * beta);

			if (startX > endX) {
				std::swap(startX, endX);
			}

			for (auto x = startX; x <= endX; x++) {
				::SetPixel(hdc, x, y, colour);
			}
		}
	}
}

inline void fillTriangle2(HDC hdc, int x0, int y0, int x1, int y1, int x2, int y2, unsigned long colour) noexcept {
	assert(hdc);

	if (isDegenerateYTriangle(y0, y1, y2)) {
		fillDegenerateTriangle(hdc, x0, x1, x2, y0, colour);
		return;
	}

	// Sort by y
	if (y0 > y1) {
		std::swap(y0, y1);
		std::swap(x0, x1);
	}

	if (y0 > y2) {
		std::swap(y0, y2);
		std::swap(x0, x2);
	}

	if (y1 > y2) {
		std::swap(y1, y2);
		std::swap(x1, x2);
	}

	auto originalPen = static_cast<HPEN>(SelectObject(hdc, GetStockObject(DC_PEN)));
	if (!originalPen) {
		return;
	}
	SetDCPenColor(hdc, colour);

	const auto triangleHeight = static_cast<double>(y2 - y0 + 1);

	{
		double const lowerSegmentTriangleHeight = static_cast<double>(y1 - y0 + 1);
		for (auto y = y0; y < y1; y++) {
			const auto alpha = (y - y0) / triangleHeight;
			const auto beta = (y - y0) / lowerSegmentTriangleHeight;

			auto startX = static_cast<std::remove_cvref_t<decltype(x0)>>(x0 + (x2 - x0) * alpha);
			auto endX = static_cast<std::remove_cvref_t<decltype(x0)>>(x0 + (x1 - x0) * beta);

			if (startX > endX) {
				std::swap(startX, endX);
			}

			//for (auto x = startX; x <= endX; x++) {
			//	xy.setPixel(hdc, x, y, colour);
			//}

			::MoveToEx(hdc, startX, y, nullptr);
			::LineTo(hdc, endX, y);
		}
	}

	{
		double const upperSegmentTriangleHeight = static_cast<double>(y2 - y1 + 1);
		for (auto y = y1; y <= y2; y++) {
			const auto alpha = (y - y0) / triangleHeight;
			const auto beta = (y - y1) / upperSegmentTriangleHeight;

			auto startX = static_cast<std::remove_cvref_t<decltype(x0)>>(x0 + (x2 - x0) * alpha);
			auto endX = static_cast<std::remove_cvref_t<decltype(x1)>>(x1 + (x2 - x1) * beta);

			if (startX > endX) {
				std::swap(startX, endX);
			}

			//for (auto x = startX; x <= endX; x++) {
			//	xy.setPixel(hdc, x, y, colour);
			//}

			::MoveToEx(hdc, startX, y, nullptr);
			::LineTo(hdc, endX, y);
		}
	}

	SelectObject(hdc, originalPen);
}

inline void fillTriangle(HDC hdc, int x0, int y0, int z0, int x1, int y1, int z1, int x2, int y2, int z2, unsigned long colour, std::vector<int>& zbuffer, uint32_t zbufferWidth) noexcept {
	assert(hdc);

	if (isDegenerateYTriangle(y0, y1, y2)) {
		fillDegenerateTriangle(hdc, x0, x1, x2, y0, colour);
		return;
	}

	// Sort by y
	if (y0 > y1) {
		std::swap(y0, y1);
		std::swap(x0, x1);
	}

	if (y0 > y2) {
		std::swap(y0, y2);
		std::swap(x0, x2);
	}

	if (y1 > y2) {
		std::swap(y1, y2);
		std::swap(x1, x2);
	}

	const auto triangleHeight = static_cast<double>(y2) - y0 + 1;

	{
		double const lowerSegmentTriangleHeight = static_cast<double>(y1) - y0 + 1;
		for (auto y = y0; y < y1; y++) {
			const auto alpha = (y - y0) / triangleHeight;
			const auto beta = (y - y0) / lowerSegmentTriangleHeight;

			auto startX = static_cast<std::remove_cvref_t<decltype(x0)>>(x0 + (x2 - x0) * alpha);
			auto endX = static_cast<std::remove_cvref_t<decltype(x0)>>(x0 + (x1 - x0) * beta);

			if (startX > endX) {
				std::swap(startX, endX);
			}

			//float phi = B.x == A.x ? 1. : (float)(j - A.x) / (float)(B.x - A.x);
			//Vec3i P = A + (B - A) * phi;

			const auto sweepLineXLength = (endX != startX) ? static_cast<double>(endX) - startX : 1.0;
			for (auto x = startX; x <= endX; x++) {
				const auto gamma = (x - startX) / sweepLineXLength;
				const auto z = static_cast<std::remove_cvref_t<decltype(zbuffer[0])>>(z0 + (z1 - z0) * gamma);

				const auto zIndex = x + y * zbufferWidth;
				if (zbuffer[zIndex] < z) {
					zbuffer[zIndex] = z;
					::SetPixel(hdc, x, y, colour);
				}
			}
		}
	}

	{
		double const upperSegmentTriangleHeight = static_cast<double>(y2) - y1 + 1;
		for (auto y = y1; y <= y2; y++) {
			const auto alpha = (static_cast<std::remove_cvref_t<decltype(triangleHeight)>>(y) - y0) / triangleHeight;
			const auto beta = (static_cast<std::remove_cvref_t<decltype(upperSegmentTriangleHeight)>>(y) - y1) / upperSegmentTriangleHeight;

			auto startX = static_cast<std::remove_cvref_t<decltype(x0)>>(x0 + (x2 - x0) * alpha);
			auto endX = static_cast<std::remove_cvref_t<decltype(x1)>>(x1 + (x2 - x1) * beta);

			if (startX > endX) {
				std::swap(startX, endX);
			}

			const auto sweepLineXLength = (endX != startX) ? static_cast<double>(endX) - startX : 1.0;
			for (auto x = startX; x <= endX; x++) {
				const auto gamma = (x - startX) / sweepLineXLength;
				const auto z = static_cast<std::remove_cvref_t<decltype(zbuffer[0])>>(z1 + (z2 - z1) * gamma);

				const auto zIndex = x + y * zbufferWidth;
				if (zbuffer[zIndex] < z) {
					zbuffer[zIndex] = z;
					::SetPixel(hdc, x, y, colour);
				}
			}
		}
	}
}

template<typename VertexType>
inline void computeBoundingBox(const std::vector<mymtl::Vector2<VertexType>>& vertices, mymtl::BoundingBox2<VertexType>& box) noexcept {
	if (vertices.empty()) {
		::memset(&box.sb, 0, sizeof(box.sb));
		return;
	}

	{
		const auto firstElement = vertices.front();
		box.sb.left_ = box.sb.right_ = firstElement.x;
		box.sb.top_ = box.sb.bottom_ = firstElement.y;
	}
	for (const auto& v : vertices) {
		if (box.sb.left_ > v.x) {
			box.sb.left_ = v.x;
		}
		else if (box.sb.right_ < v.x) {
			box.sb.right_ = v.x;
		}

		if (box.sb.top_ < v.y) {
			box.sb.top_ = v.y;
		}
		else if (box.sb.bottom_ > v.y) {
			box.sb.bottom_ = v.y;
		}
	}
}

template<typename VertexType>
inline void computeBoundingBox(const std::vector<mymtl::Vector3<VertexType>>& vertices, mymtl::BoundingBox3<VertexType>& box) noexcept {
	if (vertices.empty()) {
		::memset(&box.sb, 0, sizeof(box.sb));
		return;
	}

	{
		const auto firstElement = vertices.front();

		box.sb.left_ = box.sb.right_ = firstElement.x;
		box.sb.top_ = box.sb.bottom_ = firstElement.y;
		box.sb.far_ = box.sb.near_ = firstElement.z;
	}
	for (const auto& v : vertices) {
		if (box.sb.left_ > v.x) {
			box.sb.left_ = v.x;
		} else if (box.sb.right_ < v.x) {
			box.sb.right_ = v.x;
		}

		if (box.sb.top_ < v.y) {
			box.sb.top_ = v.y;
		}
		else if (box.sb.bottom_ > v.y) {
			box.sb.bottom_ = v.y;
		}

		if (box.sb.near_ > v.z) {
			box.sb.near_ = v.z;
		}
		else if (box.sb.far_ < v.z) {
			box.sb.far_ = v.z;
		}
	}
}

MYCG_NS_END

#endif