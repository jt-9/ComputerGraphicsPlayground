#ifndef MyCG_h
#define MyCG_h

#pragma once

#include "MyCGDefines.h"
#include "CGModel.h"
#include "Vector.h"
#include "BoundingBox.h"

#include <vector>

MYCG_NS_BEGIN

// Uses GDI SetPixel to draw a swipping line
void fillTriangle(HDC hdc, int x0, int y0, int x1, int y1, int x2, int y2, unsigned long colour) noexcept;

// Uses GDI MoveToEx, LineTo to draw a swipping line
void fillTriangle2(HDC hdc, int x0, int y0, int x1, int y1, int x2, int y2, unsigned long colour) noexcept;

void fillTriangle(HDC hdc, int x0, int y0, int z0, int x1, int y1, int z1, int x2, int y2, int z2, unsigned long colour, std::vector<int>& zbuffer, uint32_t zbufferWidth) noexcept;

template<typename VertexType>
void computeBoundingBox(const std::vector<mymtl::Vector2<VertexType>>& vertices, mymtl::BoundingBox2<VertexType>& box) noexcept;

template<typename VertexType>
void computeBoundingBox(const std::vector<mymtl::Vector3<VertexType>>& vertices, mymtl::BoundingBox3<VertexType>& box) noexcept;

MYCG_NS_END

#include "MyCG-inl.h"

#endif //MyCG_h
