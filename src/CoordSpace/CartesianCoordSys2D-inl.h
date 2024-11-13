#ifndef CartesianCoordSys2D_inl
#define CartesianCoordSys2D_inl

#include <cassert>
#include <cmath>
#include <algorithm>

MY_COORD_SPACE_BEGIN


template<typename SU, typename CU, typename TU, typename Formatter>
MYMTL_INLINE constexpr CartesianCoordSys2D<SU, CU, TU, Formatter>::CartesianCoordSys2D(const Space::ClientSpace& clientSpace, const Space::ScreenSpace& screenSpace, const ClientUnitVector& origin, 
	const std::optional<std::array<std::optional<AxisAttributes>, Space::kDimensions>>& aa,
	const std::optional<std::array<std::optional<TickAttributes>, Space::kDimensions>>& ta,
	const std::optional<std::array<std::optional<LabelAttributes>, Space::kDimensions>>& la) noexcept
	: CartesianCoordSys2D(clientSpace, screenSpace, fromClientSpace(clientSpace), origin, aa, ta, la)
{

}

template<typename SU, typename CU, typename TU, typename Formatter>
MYMTL_INLINE constexpr CartesianCoordSys2D<SU, CU, TU, Formatter>::CartesianCoordSys2D(const Space::ClientSpace& clientSpace, const Space::TransformationMatrix& m, const ClientUnitVector& origin,
	const std::optional<std::array<std::optional<AxisAttributes>, Space::kDimensions>>& aa,
	const std::optional<std::array<std::optional<TickAttributes>, Space::kDimensions>>& ta,
	const std::optional<std::array<std::optional<LabelAttributes>, Space::kDimensions>>& la) noexcept
	: CartesianCoordSys2D(clientSpace, m, fromClientSpace(clientSpace), origin, aa, ta, la)
{

}

template<typename SU, typename CU, typename TU, typename Formatter>
MYMTL_INLINE constexpr CartesianCoordSys2D<SU, CU, TU, Formatter>::CartesianCoordSys2D(const Space::ClientSpace& clientSpace, const Space::ScreenSpace& screenSpace,
	const BoundingBox& boundBox, const ClientUnitVector& origin,
	const std::optional<std::array<std::optional<AxisAttributes>, Space::kDimensions>>& aa,
	const std::optional<std::array<std::optional<TickAttributes>, Space::kDimensions>>& ta,
	const std::optional<std::array<std::optional<LabelAttributes>, Space::kDimensions>>& la) noexcept
	: space_{ clientSpace, screenSpace }, axes_{
		Axis{
			computeAxisEndPoints(boundBox, origin, Axis::Abscissa), computeAxisEnds(boundBox, Axis::Abscissa), computeAxisOrigin(origin, Axis::Abscissa), 
			getAttributesForAxis(aa, Axis::Abscissa), getAttributesForAxis(ta, Axis::Abscissa), getAttributesForAxis(la, Axis::Abscissa)
		},
		Axis{
			computeAxisEndPoints(boundBox, origin, Axis::Ordinate), computeAxisEnds(boundBox, Axis::Ordinate), computeAxisOrigin(origin, Axis::Ordinate), 
			getAttributesForAxis(aa, Axis::Ordinate), getAttributesForAxis(ta, Axis::Ordinate), getAttributesForAxis(la, Axis::Ordinate)
		}
	}
{

}

template<typename SU, typename CU, typename TU, typename Formatter>
MYMTL_INLINE constexpr CartesianCoordSys2D<SU, CU, TU, Formatter>::CartesianCoordSys2D(const Space::ClientSpace& clientSpace, const Space::TransformationMatrix& m,
	const BoundingBox& boundBox, const ClientUnitVector& origin,
	const std::optional<std::array<std::optional<AxisAttributes>, Space::kDimensions>>& aa,
	const std::optional<std::array<std::optional<TickAttributes>, Space::kDimensions>>& ta,
	const std::optional<std::array<std::optional<LabelAttributes>, Space::kDimensions>>& la) noexcept
	: space_{ clientSpace, m }, axes_{
		Axis{
			computeAxisEndPoints(boundBox, origin, Axis::Abscissa), computeAxisEnds(boundBox, Axis::Abscissa), computeAxisOrigin(origin, Axis::Abscissa), 
			getAttributesForAxis(aa, Axis::Abscissa), getAttributesForAxis(ta, Axis::Abscissa), getAttributesForAxis(la, Axis::Abscissa)
		},
		Axis{
			computeAxisEndPoints(boundBox, origin, Axis::Ordinate), computeAxisEnds(boundBox, Axis::Ordinate), computeAxisOrigin(origin, Axis::Ordinate), 
			getAttributesForAxis(aa, Axis::Ordinate), getAttributesForAxis(ta, Axis::Ordinate), getAttributesForAxis(la, Axis::Ordinate)
		}
	}
{

}

template<typename SU, typename CU, typename TU, typename Formatter>
MYMTL_INLINE constexpr auto& CartesianCoordSys2D<SU, CU, TU, Formatter>::setClientSpace(const Space::ClientSpace& clientSpace, bool recalculateTransformation) noexcept {
	space_.setClientSpace(clientSpace, recalculateTransformation);

	return *this;
}

template<typename SU, typename CU, typename TU, typename Formatter>
MYMTL_INLINE constexpr const typename CartesianCoordSys2D<SU, CU, TU, Formatter>::Space::ClientSpace& CartesianCoordSys2D<SU, CU, TU, Formatter>::getClientSpace() const noexcept {
	return space_.getClientSpace();
}

template<typename SU, typename CU, typename TU, typename Formatter>
MYMTL_INLINE constexpr auto& CartesianCoordSys2D<SU, CU, TU, Formatter>::setScreenSpace(const Space::ScreenSpace& screenSpace, bool recalculateTransformation) noexcept {
	space_.setScreenSpace(screenSpace, recalculateTransformation);

	return *this;
}

template<typename SU, typename CU, typename TU, typename Formatter>
MYMTL_INLINE constexpr const typename CartesianCoordSys2D<SU, CU, TU, Formatter>::Space::ScreenSpace& CartesianCoordSys2D<SU, CU, TU, Formatter>::getScreenSpace() const noexcept {
	return space_.getScreenSpace();
}

template<typename SU, typename CU, typename TU, typename Formatter>
MYMTL_INLINE constexpr auto& CartesianCoordSys2D<SU, CU, TU, Formatter>::setOrigin(const ClientUnitVector& origin) noexcept {
	for (std::size_t i = 0; i < axes_.size(); i++) {
		axes_[i].origin_ = origin[i];
	}

	return *this;
}

template<typename SU, typename CU, typename TU, typename Formatter>
MY_COORD_SPACE_ATTR_NO_DISCARD constexpr typename CartesianCoordSys2D<SU, CU, TU, Formatter>::ClientUnitVector CartesianCoordSys2D<SU, CU, TU, Formatter>::getOrigin() noexcept {
	ClientUnitVector origin;

	for (std::size_t i = 0; i < axes_.size(); i++) {
		origin[i] = axes_[i].origin_;
	}

	return origin;
}

template<typename SU, typename CU, typename TU, typename Formatter>
MYMTL_INLINE constexpr void CartesianCoordSys2D<SU, CU, TU, Formatter>::recalculateTransformation() noexcept {
	space_.recalculateTransformation();
}

template<typename SU, typename CU, typename TU, typename Formatter>
[[nodiscard]] MYMTL_INLINE constexpr typename CartesianCoordSys2D<SU, CU, TU, Formatter>::ClientUnitVector CartesianCoordSys2D<SU, CU, TU, Formatter>::clampVector(
	const typename CartesianCoordSys2D<SU, CU, TU, Formatter>::ClientUnitVector& v,
	const typename CartesianCoordSys2D<SU, CU, TU, Formatter>::BoundingBox& bb) noexcept {

	assert(!bb.is_empty() && "Bounding box cannot be 0-dimensional");

	CartesianCoordSys2D<SU, CU, TU, Formatter>::ClientUnitVector r;
	for (unsigned char i = 0; i < bb.size(); i++) {
		r[i] = std::clamp(v[i], bb.min[i], bb.max[i]);
	}

	return r;
}

template<typename SU, typename CU, typename TU, typename Formatter>
MYMTL_INLINE constexpr auto CartesianCoordSys2D<SU, CU, TU, Formatter>::moveTo(HDC hdc, ClientUnit x, ClientUnit y) const noexcept {
	const auto sv = space_.clientToScreenPoint(x, y);

	return rasteriser_.moveTo(hdc, static_cast<SU>(sv.x), static_cast<SU>(sv.y));
}

template<typename SU, typename CU, typename TU, typename Formatter>
MYMTL_INLINE constexpr auto CartesianCoordSys2D<SU, CU, TU, Formatter>::lineTo(HDC hdc, ClientUnit x, ClientUnit y) const noexcept {
	const auto sv = space_.clientToScreenPoint(x, y);

	return rasteriser_.lineTo(hdc, static_cast<SU>(sv.x), static_cast<SU>(sv.y));
}

template<typename SU, typename CU, typename TU, typename Formatter>
MYMTL_INLINE constexpr auto CartesianCoordSys2D<SU, CU, TU, Formatter>::line(HDC hdc, ClientUnit fromX, ClientUnit fromY, ClientUnit toX, ClientUnit toY) const noexcept {
	const auto from = space_.clientToScreenPoint(fromX, fromY);
	const auto to = space_.clientToScreenPoint(toX, toY);

	return rasteriser_.line(hdc, static_cast<SU>(from.x), static_cast<SU>(from.y), static_cast<SU>(to.x), static_cast<SU>(to.y));
}

template<typename SU, typename CU, typename TU, typename Formatter>
MYMTL_INLINE constexpr auto CartesianCoordSys2D<SU, CU, TU, Formatter>::setPixel(HDC hdc, ClientUnit x, ClientUnit y, unsigned long colour) const noexcept {
	const auto sv = space_.clientToScreenPoint(x, y);

	return rasteriser_.setPixel(hdc, static_cast<SU>(sv.x), static_cast<SU>(sv.y), colour);
}

template<typename SU, typename CU, typename TU, typename Formatter>
MYMTL_INLINE constexpr auto CartesianCoordSys2D<SU, CU, TU, Formatter>::ellipse(HDC hdc, ClientUnit left, ClientUnit top, ClientUnit right, ClientUnit bottom) const noexcept {
	const auto tl = space_.clientToScreenPoint(left, top);
	const auto rb = space_.clientToScreenPoint(right, bottom);

	return rasteriser_.ellipse(hdc, static_cast<SU>(tl.x), static_cast<SU>(tl.y), static_cast<SU>(rb.x), static_cast<SU>(rb.y));
}

template<typename SU, typename CU, typename TU, typename Formatter>
MYMTL_INLINE constexpr void CartesianCoordSys2D<SU, CU, TU, Formatter>::draw(HDC hdc) const noexcept {
	for (const auto& axis : axes_) {
		axis.draw(hdc, rasteriser_, space_);
	}
}


MY_COORD_SPACE_END

#endif // !CartesianCoordSys2D_inl