#ifndef CGModel_h
#define CGModel_h

#pragma once

#include "Vector.h"
#include "MyCGDefines.h"

#include <vector>

MYCG_NS_BEGIN

template<typename Coordinate>
class CGModel {
public:
	using CoordinateType = Coordinate;
	using VertexType = mymtl::Vector3<CoordinateType>;
	using FaceType = std::vector<int>;

	using VertexArray = std::vector<VertexType>;
	using FaceArray = std::vector<FaceType>;
private:
	VertexArray verts_;
	FaceArray faces_;
public:
	CGModel() = default;
	inline CGModel(const VertexArray& vs, const FaceArray& fs)
		: verts_{ vs }, faces_{ fs }
	{}

	CGModel(const CGModel& src) = default;
	CGModel(CGModel&& src) = default;
	~CGModel() = default;

	CGModel& operator = (const CGModel& src) = default;
	CGModel& operator = (CGModel&& src) = default;
	
	inline constexpr auto vertexAt(size_t i) const {
		return verts_.at(i);
	}

	inline constexpr const auto& faceAt(size_t i) const {
		return faces_.at(i);
	}

	inline constexpr auto vertexArraySSize() const {
		return std::ssize(verts_);
	}

	inline constexpr auto faceArraySSize() const {
		return std::ssize(faces_);
	}

	inline constexpr const auto& vertices() const {
		return verts_;
	}

	inline constexpr const auto& faces() const {
		return faces_;
	}
};

MYCG_NS_END

#endif // CGModel_h
