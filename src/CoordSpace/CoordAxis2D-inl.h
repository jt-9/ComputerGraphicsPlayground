#ifndef CartesianAxis2D_inl
#define CartesianAxis2D_inl

#pragma once

#include <utility>
#include "CoordAxis2D.h"

MY_COORD_SPACE_BEGIN


//------------------------------------------------------------------------------
// Axis2D class implementation
//------------------------------------------------------------------------------
template<typename SU, typename CU, typename Formatter>
MYMTL_INLINE constexpr Axis2D<SU, CU, Formatter>::Axis2D(const std::array<ClientVector, kAxisEndsNumber>& axisEndPoints,
	const std::array<ClientUnit, kAxisEndsNumber>& axisEnds, ClientUnit origin,
	const std::optional<AxisAttributes>& axisAttributes, const std::optional<TickAttributes>& tickAttributes, const std::optional<LabelAttributes>& labelAttributes) noexcept
	: endPoints_{ axisEndPoints }, ends_{ axisEnds }, origin_{ origin }, axisAttr_{ axisAttributes }, labelAttr_{ labelAttributes }, tickAttr_{ tickAttributes }
{
}

template<typename SU, typename CU, typename Formatter>
MYMTL_INLINE constexpr const auto& Axis2D<SU, CU, Formatter>::getAxisAttrib() const noexcept {
	return axisAttr_;
}

template<typename SU, typename CU, typename Formatter>
MYMTL_INLINE constexpr const auto& Axis2D<SU, CU, Formatter>::getTickAttrib() const noexcept {
	return tickAttr_;
}

template<typename SU, typename CU, typename Formatter>
MYMTL_INLINE constexpr const auto& Axis2D<SU, CU, Formatter>::getLabelAttrib() const noexcept {
	return labelAttr_;
}

template<typename SU, typename CU, typename Formatter>
MYMTL_INLINE constexpr void Axis2D<SU, CU, Formatter>::setEndPoint(const ClientVector& endPoint, PointIndex axisPointIndex) noexcept {
	assert(std::to_underlying(axisPointIndex) < kAxisEndsNumber && "axisPointIndex is out of bounds");
	endPoints_[std::to_underlying(axisPointIndex)] = endPoint;
}

template<typename SU, typename CU, typename Formatter>
MYMTL_INLINE constexpr const typename Axis2D<SU, CU, Formatter>::ClientVector& Axis2D<SU, CU, Formatter>::getEndPoint(PointIndex axisPointIndex) const noexcept {
	assert(std::to_underlying(axisPointIndex) < kAxisEndsNumber && "axisPointIndex is out of bounds");
	return endPoints_[std::to_underlying(axisPointIndex)];
}

template<typename SU, typename CU, typename Formatter>
MYMTL_INLINE constexpr void Axis2D<SU, CU, Formatter>::setEndPoints(const std::array<ClientVector, kAxisEndsNumber>& endPoints) noexcept {
	endPoints_ = endPoints;
}

template<typename SU, typename CU, typename Formatter>
MYMTL_INLINE constexpr const auto& Axis2D<SU, CU, Formatter>::getEndPoints() const noexcept {
	return endPoints_;
}

template<typename SU, typename CU, typename Formatter>
MYMTL_INLINE constexpr void Axis2D<SU, CU, Formatter>::setEndValue(ClientUnit endValue, PointIndex axisPointIndex) noexcept {
	assert(std::to_underlying(axisPointIndex) < kAxisEndsNumber && "axisPointIndex is out of bounds");
	ends_[std::to_underlying(axisPointIndex)] = endValue;
}

template<typename SU, typename CU, typename Formatter>
MYMTL_INLINE constexpr typename Axis2D<SU, CU, Formatter>::ClientUnit Axis2D<SU, CU, Formatter>::getEndValue(PointIndex axisPointIndex) const noexcept {
	assert(std::to_underlying(axisPointIndex) < kAxisEndsNumber && "axisPointIndex is out of bounds");
	return ends_[std::to_underlying(axisPointIndex)];
}

template<typename SU, typename CU, typename Formatter>
MYMTL_INLINE constexpr void Axis2D<SU, CU, Formatter>::setEndValues(const std::array<ClientUnit, kAxisEndsNumber>& endValues) noexcept {
	ends_ = endValues;
}

template<typename SU, typename CU, typename Formatter>
MYMTL_NO_DISCARD constexpr const auto& Axis2D<SU, CU, Formatter>::getEndValues() const noexcept {
	return ends_;
}

template<typename SU, typename CU, typename Formatter>
MYMTL_INLINE constexpr void Axis2D<SU, CU, Formatter>::setOrigin(ClientUnit origin) noexcept {
	origin_ = origin;
}

template<typename SU, typename CU, typename Formatter>
MYMTL_INLINE constexpr typename Axis2D<SU, CU, Formatter>::ClientUnit Axis2D<SU, CU, Formatter>::getOrigin() const noexcept {
	return origin_;
}

//template<typename SU, typename CU, typename Formatter>
//MYMTL_INLINE int Axis2D<SU, CU, Formatter>::getUsedElements() const {
//	return showAxisStyle_;
//}

//MYMTL_INLINE int Axis2D::deselUsedElem( UsingElem deselElem )
//{
//	showAxisStyle_ &= ~deselElem;
//	return showAxisStyle_;
//}

template<typename SU, typename CU, typename Formatter>
template<typename TU>
MYMTL_INLINE constexpr void Axis2D<SU, CU, Formatter>::draw(Rasteriser rasteriser, const CoordSpace2D<SU, CU, TU>& space) const noexcept {
	const auto hdcStash = rasteriser.saveStateRAII();

	const auto from = space.clientToScreenPoint(getEndPoint(PointIndex::Start));
	const auto to = space.clientToScreenPoint(getEndPoint(PointIndex::End));

	if (shouldDrawAxis()) {
		drawAxis(rasteriser, from, to, *axisAttr_);
	}

	if (shouldDrawTickMarks()) {
		drawTickMarks(rasteriser, space, from, to, *tickAttr_);
	}

	if (shouldDisplayLabels()) {
		displayLabels<TU>(rasteriser, from, to, *labelAttr_);
	}
}

template<typename SU, typename CU, typename Formatter>
MYMTL_INLINE constexpr bool Axis2D<SU, CU, Formatter>::shouldDrawAxis() const noexcept {
	return axisAttr_.has_value();
}

template<typename SU, typename CU, typename Formatter>
MYMTL_INLINE constexpr void Axis2D<SU, CU, Formatter>::drawAxis(Rasteriser rasteriser, const ScreenVector& startPoint, const ScreenVector& endPoint, const AxisAttributes& aa) const noexcept {
	const auto brushResource = (aa.brush) ? std::optional{ rasteriser.selectObjectResource(aa.brush) } : std::nullopt;
	const auto penResource = (aa.pen) ? std::optional{ rasteriser.selectObjectResource(aa.pen) } : std::nullopt;

	rasteriser.line(startPoint, endPoint);
}

template<typename SU, typename CU, typename Formatter>
MYMTL_INLINE constexpr bool Axis2D<SU, CU, Formatter>::shouldDrawTickMarks() const noexcept {
	return tickAttr_.has_value() && (tickAttr_->upperPart != 0 || tickAttr_->lowerPart != 0);
}

template<typename SU, typename CU, typename Formatter>
template<typename TU>
MYMTL_INLINE constexpr void Axis2D<SU, CU, Formatter>::drawTickMarks(Rasteriser rasteriser, const CoordSpace2D<SU, CU, TU>& space,
	const ScreenVector& startPoint, const ScreenVector& endPoint, const TickAttributes& ta) const noexcept {
	const ClientUnit clampedOrigin = std::clamp(origin_, getEndValue(PointIndex::Start), getEndValue(PointIndex::End));

	const auto axisDirVectorScreenUnits = (endPoint - startPoint) / static_cast<TU>(getEndValue(PointIndex::End) - getEndValue(PointIndex::Start));

	const auto originScreenPoint = startPoint + axisDirVectorScreenUnits * static_cast<TU>(clampedOrigin - getEndValue(PointIndex::Start));

	const auto brushResource = (ta.brush) ? std::optional{ rasteriser.selectObjectResource(ta.brush) } : std::nullopt;
	const auto penResource = (ta.pen) ? std::optional{ rasteriser.selectObjectResource(ta.pen) } : std::nullopt;


	const auto axisNormalScreenUnits = mymtl::vector_normalize_ert<TU>(space.clientToScreenVector(mymtl::vector_normal(getEndPoint(PointIndex::End) - getEndPoint(PointIndex::Start))));
	// Draw ticks from axis min to origin - offset
	if (getEndValue(PointIndex::Start) + ta.axisEndOffset < clampedOrigin - ta.originOffset && ta.maxCount > 0) {
		const auto computeTickPosition = [origin = clampedOrigin - ta.originOffset, step = ta.step](auto n) noexcept { return origin - n * step; };

		std::remove_cv_t<decltype(ta.maxCount)> tickCount = 0;
		auto tickPosition = computeTickPosition(tickCount);
		while (getEndValue(PointIndex::Start) + ta.axisEndOffset <= tickPosition && tickCount < ta.maxCount) {
			const ScreenVector tickScreenPoint = mymtl::to_vector<typename ScreenVector::value_type, ScreenVector::size()>(originScreenPoint + (tickPosition - clampedOrigin) * axisDirVectorScreenUnits);
			drawTickMarkAt(rasteriser, ta, axisNormalScreenUnits, tickScreenPoint);

			tickCount++;
			tickPosition = computeTickPosition(tickCount);
		}
	}

	// Draw ticks form origin + offset to axis max
	if (getEndValue(PointIndex::End) - ta.axisEndOffset > clampedOrigin + ta.originOffset && ta.maxCount > 0) {
		const auto computeTickPosition = [origin = clampedOrigin + ta.originOffset, step = ta.step](auto n) noexcept { return origin + n * step; };

		std::remove_cv_t<decltype(ta.maxCount)> tickCount = 0;
		auto tickPosition = computeTickPosition(tickCount);
		while (getEndValue(PointIndex::End) - ta.axisEndOffset >= tickPosition && tickCount < ta.maxCount) {
			const ScreenVector tickScreenPoint = mymtl::to_vector<typename ScreenVector::value_type, ScreenVector::size()>(originScreenPoint + (tickPosition - clampedOrigin) * axisDirVectorScreenUnits);
			drawTickMarkAt(rasteriser, ta, axisNormalScreenUnits, tickScreenPoint);

			tickCount++;
			tickPosition = computeTickPosition(tickCount);
		}
	}
}

template<typename SU, typename CU, typename Formatter>
template<typename TU>
MYMTL_INLINE constexpr void Axis2D<SU, CU, Formatter>::drawTickMarkAt(Rasteriser rasteriser, const TickAttributes& ta, const mymtl::Vector2<TU>& axisUnitNormal, const ScreenVector& tickScreenPoint) const noexcept {
	//rasteriser.ellipse(tickScreenPoint.x-ta.upperPart, tickScreenPoint.y-ta.upperPart, tickScreenPoint.x+ta.lowerPart, tickScreenPoint.y+ta.lowerPart);
	const ScreenVector upperPart = mymtl::to_vector<typename ScreenVector::value_type, ScreenVector::size()>(ta.upperPart * axisUnitNormal), lowerPart = mymtl::to_vector<ScreenUnit, axisUnitNormal.size()>(ta.lowerPart * axisUnitNormal);

	rasteriser.line(tickScreenPoint - lowerPart, tickScreenPoint + upperPart);
}


template<typename SU, typename CU, typename Formatter>
inline constexpr bool Axis2D<SU, CU, Formatter>::shouldDisplayLabels() const noexcept
{
	return labelAttr_.has_value();
}

template<typename SU, typename CU, typename Formatter>
template<typename TU>
MYMTL_INLINE constexpr void Axis2D<SU, CU, Formatter>::displayLabels(Rasteriser rasteriser,
	const ScreenVector& startPoint, const ScreenVector& endPoint, const LabelAttributes& la) const noexcept {
	const ClientUnit clampedOrigin = std::clamp(origin_, getEndValue(PointIndex::Start), getEndValue(PointIndex::End));

	const auto axisDirectionScreenUnits = endPoint - startPoint;
	const auto axisDirectionScreenUnitsPerClientUnit = axisDirectionScreenUnits / static_cast<TU>(getEndValue(PointIndex::End) - getEndValue(PointIndex::Start));
	const auto originScreenPoint = startPoint + axisDirectionScreenUnitsPerClientUnit * static_cast<TU>(clampedOrigin - getEndValue(PointIndex::Start));

	const auto fontResource = (la.font) ? std::optional{ rasteriser.selectObjectResource(la.font) } : std::nullopt;
	const auto prevTextAlignment = rasteriser.setTextAlign(la.textAlign);

	const auto axisDirUnitVectorScreenUnits = mymtl::vector_normalize_ert<TU>(axisDirectionScreenUnits);
	const auto axisUnitNormalScreenUnits = mymtl::vector_normal(axisDirUnitVectorScreenUnits);

	// Draw labels from axis min to origin - offset
	if (getEndValue(PointIndex::Start) + la.axisEndOffset < clampedOrigin - la.originOffset && la.maxCount > 0) {
		const auto computeLabelPosition = [origin = clampedOrigin - la.originOffset, step = la.step](auto n) noexcept { return origin - n * step; };

		std::remove_cv_t<decltype(la.maxCount)> labelCount = 0;
		auto labelPosition = computeLabelPosition(labelCount);
		while (getEndValue(PointIndex::Start) + la.axisEndOffset <= labelPosition && labelCount < la.maxCount) {
			const ScreenVector labelAxisScreenPoint = mymtl::to_vector<typename ScreenVector::value_type, ScreenVector::size()>(originScreenPoint + (labelPosition - clampedOrigin) * axisDirectionScreenUnitsPerClientUnit);
			displayLabelAt(rasteriser, la, labelPosition, axisDirUnitVectorScreenUnits, axisUnitNormalScreenUnits, labelAxisScreenPoint);

			labelCount++;
			labelPosition = computeLabelPosition(labelCount);
		}
	}

	// Draw ticks form origin + offset to axis max
	if (getEndValue(PointIndex::End) - la.axisEndOffset > clampedOrigin + la.originOffset && la.maxCount > 0) {
		const auto computeLabelPosition = [origin = clampedOrigin + la.originOffset, step = la.step](auto n) noexcept { return origin + n * step; };

		std::remove_cv_t<decltype(la.maxCount)> labelCount = 0;
		auto labelPosition = computeLabelPosition(labelCount);
		while (getEndValue(PointIndex::End) - la.axisEndOffset >= labelPosition && labelCount < la.maxCount) {
			const ScreenVector labelAxisScreenPoint = mymtl::to_vector<typename ScreenVector::value_type, ScreenVector::size()>(originScreenPoint + (labelPosition - clampedOrigin) * axisDirectionScreenUnitsPerClientUnit);
			displayLabelAt(rasteriser, la, labelPosition, axisDirUnitVectorScreenUnits, axisUnitNormalScreenUnits, labelAxisScreenPoint);

			labelCount++;
			labelPosition = computeLabelPosition(labelCount);
		}
	}

	rasteriser.setTextAlign(prevTextAlignment);
}

template<typename SU, typename CU, typename Formatter>
template<typename TU>
MYMTL_INLINE constexpr void Axis2D<SU, CU, Formatter>::displayLabelAt(Rasteriser rasteriser, const LabelAttributes& la, ClientUnit labelPosition,
	const mymtl::Vector2<TU>& axisDirUnitVector, const mymtl::Vector2<TU>& axisUnitNormal, const ScreenVector& labelAxisScreenPoint) const noexcept {

	const ScreenVector referencePoint = labelAxisScreenPoint + mymtl::to_vector<typename ScreenVector::value_type, ScreenVector::size()>(axisDirUnitVector * la.textOffset.x + axisUnitNormal * la.textOffset.y);
	rasteriser.textOut(referencePoint, la.formatter(labelPosition));
}

MY_COORD_SPACE_END

#endif // CartesianAxis2D_inl