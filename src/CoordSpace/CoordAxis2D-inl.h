#ifndef CartesianAxis2D_inl
#define CartesianAxis2D_inl

#pragma once

#include "unique_rc.hpp"
#include "windows_dc_restore.hpp"
#include "CoordAxis2D.h"

MY_COORD_SPACE_BEGIN


//------------------------------------------------------------------------------
// Axis2D class implementation
//------------------------------------------------------------------------------
template<typename SU, typename CU, typename Formatter>
MYMTL_INLINE constexpr Axis2D<SU, CU, Formatter>::Axis2D(const std::array<ClientUnitVector, kAxisEndsNumber>& axisEndPoints,
	const std::array<ClientUnit, kAxisEndsNumber>& axisEnds, ClientUnit origin,
	const std::optional<AxisAttributes>& axisAttributes, const std::optional<TickAttributes>& tickAttributes, const std::optional<LabelAttributes>& labelAttributes) noexcept
	: endPoints_{ axisEndPoints }, ends_{ axisEnds }, origin_{ origin }, axisAttr_{ axisAttributes }, labelAttr_{ labelAttributes }, tickAttr_{ tickAttributes }
{
}

//template<typename SU, typename CU, typename Formatter>
//inline Axis2D<SU, CU, Formatter>::Axis2D(int axisStyle, const gstring& axisName, ClientUnit tickStep, ClientUnit labelStep) noexcept
//	: showAxisStyle_{ 0 }, axisAttrib_{ std::nullopt }, labelAttrib_{ std::nullopt }, tickAttrib_{ std::nullopt }
//{
//	// Setting up axis attributes: font for axis name, pen for line, and setting axis name...
//	if (axisStyle & use_axis) {
//		AxisAttributes aAttrib{};
//
//		LOGPEN logpen{ PS_SOLID, POINT{1,1}, RGB(0, 0, 0) };
//
//		aAttrib.hPen = ::CreatePenIndirect(&logpen);
//		if (!aAttrib.hPen) {
//			return;
//		}
//
//		LOGFONT logfont;
//		logfont.lfHeight = 14;
//		logfont.lfWidth = 0;
//		logfont.lfEscapement = 0;
//		logfont.lfOrientation = 0;
//		logfont.lfWeight = FW_NORMAL;
//		logfont.lfItalic = 0;
//		logfont.lfUnderline = 0;
//		logfont.lfStrikeOut = 0;
//		logfont.lfCharSet = DEFAULT_CHARSET;
//		logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
//		logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
//		logfont.lfQuality = PROOF_QUALITY;
//		logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
//		::strcpy_s(logfont.lfFaceName, LF_FACESIZE, "Times New Roman");
//
//		aAttrib.hFont = ::CreateFontIndirect(&logfont);
//		if (!aAttrib.hFont) {
//			::DeleteObject(static_cast<HGDIOBJ>(aAttrib.hPen));
//			return;
//		}
//
//		aAttrib.axisName = axisName;
//		aAttrib.dwColor = RGB(0, 0, 0);
//
//		// If everything is fine, update showAxisStyle_
//		axisAttrib_ = aAttrib;
//		showAxisStyle_ |= use_axis;
//	}
//
//	// Setting up tick attributes...
//	if ((axisStyle & use_tick) && (tickStep > minTickDist)) {
//		TickAttributes<ClientUnit> tAttrib{};
//
//		LOGPEN logpen{ PS_SOLID, POINT{1,1}, RGB(0, 0, 0) };
//		/*logpen.lopnStyle = PS_SOLID;
//		logpen.lopnWidth.x = 1;
//		logpen.lopnWidth.y = 1;
//		logpen.lopnColor = RGB(0, 0, 0);*/
//
//		tAttrib.hPen = ::CreatePenIndirect(&logpen);
//		if (!tAttrib.hPen) {
//			return;
//		}
//
//		tAttrib.fStep = tickStep;
//		tAttrib.nHalfLength = 4;
//
//		tickAttrib_ = tAttrib;
//		showAxisStyle_ |= use_tick;
//	}
//
//	// Adjusting label attributes...
//	if ((axisStyle & use_label) && (labelStep > minLabelDist)) {
//		LabelAttributes<ClientUnit> lAttrib{};
//
//		LOGFONT logfont;
//		logfont.lfHeight = -11;
//		logfont.lfWidth = 0;
//		logfont.lfEscapement = 0;
//		logfont.lfOrientation = 0;
//		logfont.lfWeight = FW_NORMAL;
//		logfont.lfItalic = 0;
//		logfont.lfUnderline = 0;
//		logfont.lfStrikeOut = 0;
//		logfont.lfCharSet = DEFAULT_CHARSET;
//		logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
//		logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
//		logfont.lfQuality = PROOF_QUALITY;
//		logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
//		::strcpy_s(logfont.lfFaceName, LF_FACESIZE, "Times New Roman");
//
//		lAttrib.hFont = ::CreateFontIndirect(&logfont);
//		if (!lAttrib.hFont) {
//			return;
//		}
//
//		lAttrib.fStep = labelStep;
//		//labelAttrib_->nOrient = textOrientation;
//		lAttrib.dwColor = RGB(0, 0, 0);
//		lAttrib.nIndent = 8;
//
//		labelAttrib_ = lAttrib;
//		showAxisStyle_ |= use_label;
//	}
//}


//template<typename SU, typename CU, typename Formatter>
//inline bool Axis2D<SU, CU, Formatter>::release(int usedElems) {
//	if ((usedElems & use_axis) && axisAttrib_) {
//		if (!::DeleteObject((HGDIOBJ)axisAttrib_->hPen)
//			|| !::DeleteObject((HGDIOBJ)axisAttrib_->hFont)) {
//			return false;
//		}
//
//		usedElems &= ~use_axis;
//		axisAttrib_ = std::nullopt;
//	}
//
//	if ((usedElems & use_label) && labelAttrib_) {
//		if (!::DeleteObject((HGDIOBJ)labelAttrib_->hFont)) {
//			return false;
//		}
//
//		usedElems &= ~use_label;
//		labelAttrib_ = std::nullopt;
//	}
//
//	if ((usedElems & use_tick) && tickAttrib_) {
//		if (!::DeleteObject((HGDIOBJ)tickAttrib_->hPen)) {
//			return false;
//		}
//
//		usedElems &= ~use_tick;
//		tickAttrib_ = std::nullopt;
//	}
//
//	return true;
//}


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

//template<typename SU, typename CU, typename Formatter>
//MYMTL_INLINE constexpr void Axis2D<SU, CU, Formatter>::setEndPoints(const Vector2C& startPoint, const Vector2C& endPoint) noexcept {
//	startPoint_ = startPoint;
//	endPoint_ = endPoint;
//}

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
MYMTL_INLINE constexpr void Axis2D<SU, CU, Formatter>::draw(HDC hdc, Rasteriser rasteriser, const CoordSpace2D<SU, CU, TU>& space) const noexcept {
	const raii::unique_rc<HDC, raii::gdi_restore_dc_nullptr> hdcStash{ hdc, raii::gdi_restore_dc_nullptr{SaveDC(hdc)} };

	const auto from = space.clientToScreenPoint(endPoints_[kAxisStartIndex]);
	const auto to = space.clientToScreenPoint(endPoints_[kAxisEndIndex]);

	if (shouldDrawAxis()) {
		drawAxis(hdc, rasteriser, from, to, *axisAttr_);
	}

	if (shouldDrawTickMarks()) {
		drawTickMarks(hdc, rasteriser, space, from, to, *tickAttr_);
	}

	if (shouldDisplayLabels()) {
		displayLabels<TU>(hdc, rasteriser, from, to, *labelAttr_);
	}
}

template<typename SU, typename CU, typename Formatter>
MYMTL_INLINE constexpr bool Axis2D<SU, CU, Formatter>::shouldDrawAxis() const noexcept {
	return axisAttr_.has_value();
}

template<typename SU, typename CU, typename Formatter>
MYMTL_INLINE constexpr void Axis2D<SU, CU, Formatter>::drawAxis(HDC hdc, Rasteriser rasteriser, const ScreenUnitVector& startPoint, const ScreenUnitVector& endPoint, const AxisAttributes& aa) const noexcept {
	const auto brushResource = (aa.brush) ? std::optional{ rasteriser.selectObjectResource(hdc, aa.brush) } : std::nullopt;
	const auto penResource = (aa.pen) ? std::optional{ rasteriser.selectObjectResource(hdc, aa.pen) } : std::nullopt;

	rasteriser.line(hdc, startPoint, endPoint);
}

template<typename SU, typename CU, typename Formatter>
MYMTL_INLINE constexpr bool Axis2D<SU, CU, Formatter>::shouldDrawTickMarks() const noexcept {
	return tickAttr_.has_value() && (tickAttr_->upperPart != 0 || tickAttr_->lowerPart != 0);
}

template<typename SU, typename CU, typename Formatter>
template<typename TU>
MYMTL_INLINE constexpr void Axis2D<SU, CU, Formatter>::drawTickMarks(HDC hdc, Rasteriser rasteriser, const CoordSpace2D<SU, CU, TU>& space,
	const ScreenUnitVector& startPoint, const ScreenUnitVector& endPoint, const TickAttributes& ta) const noexcept {
	const ClientUnit clampedOrigin = std::clamp(origin_, ends_[kAxisStartIndex], ends_[kAxisEndIndex]);

	const auto axisDirVectorScreenUnits = (endPoint - startPoint) / static_cast<TU>(ends_[kAxisEndIndex] - ends_[kAxisStartIndex]);

	const auto originScreenPoint = (startPoint + axisDirVectorScreenUnits * static_cast<TU>(clampedOrigin - ends_[kAxisStartIndex]));

	const auto brushResource = (ta.brush) ? std::optional{ rasteriser.selectObjectResource(hdc, ta.brush) } : std::nullopt;
	const auto penResource = (ta.pen) ? std::optional{ rasteriser.selectObjectResource(hdc, ta.pen) } : std::nullopt;


	const auto axisNormalScreenUnits = mymtl::vector_normalize_ert<TU>(space.clientToScreenVector(mymtl::vector_normal(endPoints_[kAxisEndIndex] - endPoints_[kAxisStartIndex])));
	// Draw ticks from axis min to origin - offset
	if (ends_[kAxisStartIndex] + ta.axisEndOffset < clampedOrigin - ta.originOffset && ta.maxCount > 0) {
		const auto computeTickPosition = [origin = clampedOrigin - ta.originOffset, step = ta.step](auto n) noexcept { return origin - n * step; };

		std::remove_cv_t<decltype(ta.maxCount)> tickCount = 0;
		auto tickPosition = computeTickPosition(tickCount);
		while (ends_[kAxisStartIndex] + ta.axisEndOffset <= tickPosition && tickCount < ta.maxCount) {
			const ScreenUnitVector tickScreenPoint = mymtl::to_vector<typename ScreenUnitVector::value_type, ScreenUnitVector::size()>(originScreenPoint + (tickPosition - clampedOrigin) * axisDirVectorScreenUnits);
			drawTickMarkAt(hdc, rasteriser, ta, axisNormalScreenUnits, tickScreenPoint);

			tickCount++;
			tickPosition = computeTickPosition(tickCount);
		}
	}

	// Draw ticks form origin + offset to axis max
	if (ends_[kAxisEndIndex] - ta.axisEndOffset > clampedOrigin + ta.originOffset && ta.maxCount > 0) {
		const auto computeTickPosition = [origin = clampedOrigin + ta.originOffset, step = ta.step](auto n) noexcept { return origin + n * step; };

		std::remove_cv_t<decltype(ta.maxCount)> tickCount = 0;
		auto tickPosition = computeTickPosition(tickCount);
		while (ends_[kAxisEndIndex] - ta.axisEndOffset >= tickPosition && tickCount < ta.maxCount) {
			const ScreenUnitVector tickScreenPoint = mymtl::to_vector<typename ScreenUnitVector::value_type, ScreenUnitVector::size()>(originScreenPoint + (tickPosition - clampedOrigin) * axisDirVectorScreenUnits);
			drawTickMarkAt(hdc, rasteriser, ta, axisNormalScreenUnits, tickScreenPoint);

			tickCount++;
			tickPosition = computeTickPosition(tickCount);
		}
	}
}

template<typename SU, typename CU, typename Formatter>
template<typename TU>
MYMTL_INLINE constexpr void Axis2D<SU, CU, Formatter>::drawTickMarkAt(HDC hdc, Rasteriser rasteriser, const TickAttributes& ta, const mymtl::Vector2<TU>& axisUnitNormal, const ScreenUnitVector& tickScreenPoint) const noexcept {
	//rasteriser.ellipse(hdc, tickScreenPoint.x-ta.upperPart, tickScreenPoint.y-ta.upperPart, tickScreenPoint.x+ta.lowerPart, tickScreenPoint.y+ta.lowerPart);
	const ScreenUnitVector upperPart = mymtl::to_vector<typename ScreenUnitVector::value_type, ScreenUnitVector::size()>(ta.upperPart * axisUnitNormal), lowerPart = mymtl::to_vector<ScreenUnit, axisUnitNormal.size()>(ta.lowerPart * axisUnitNormal);

	rasteriser.line(hdc, tickScreenPoint - lowerPart, tickScreenPoint + upperPart);
}


template<typename SU, typename CU, typename Formatter>
inline constexpr bool Axis2D<SU, CU, Formatter>::shouldDisplayLabels() const noexcept
{
	return labelAttr_.has_value();
}

template<typename SU, typename CU, typename Formatter>
template<typename TU>
MYMTL_INLINE constexpr void Axis2D<SU, CU, Formatter>::displayLabels(HDC hdc, Rasteriser rasteriser,
	const ScreenUnitVector& startPoint, const ScreenUnitVector& endPoint, const LabelAttributes& la) const noexcept {
	const ClientUnit clampedOrigin = std::clamp(origin_, ends_[kAxisStartIndex], ends_[kAxisEndIndex]);

	const auto axisDirectionScreenUnits = endPoint - startPoint;
	const auto axisDirectionScreenUnitsPerClientUnit = axisDirectionScreenUnits / static_cast<TU>(ends_[kAxisEndIndex] - ends_[kAxisStartIndex]);
	const auto originScreenPoint = (startPoint + axisDirectionScreenUnitsPerClientUnit * static_cast<TU>(clampedOrigin - ends_[kAxisStartIndex]));

	const auto fontResource = (la.font) ? std::optional{ rasteriser.selectObjectResource(hdc, la.font) } : std::nullopt;
	const auto prevTextAlignment = rasteriser.setTextAlign(hdc, la.textAlign);

	const auto axisDirUnitVectorScreenUnits = mymtl::vector_normalize_ert<TU>(axisDirectionScreenUnits);
	const auto axisUnitNormalScreenUnits = mymtl::vector_normal(axisDirUnitVectorScreenUnits);

	// Draw labels from axis min to origin - offset
	if (ends_[kAxisStartIndex] + la.axisEndOffset < clampedOrigin - la.originOffset && la.maxCount > 0) {
		const auto computeLabelPosition = [origin = clampedOrigin - la.originOffset, step = la.step](auto n) noexcept { return origin - n * step; };

		std::remove_cv_t<decltype(la.maxCount)> labelCount = 0;
		auto labelPosition = computeLabelPosition(labelCount);
		while (ends_[kAxisStartIndex] + la.axisEndOffset <= labelPosition && labelCount < la.maxCount) {
			const ScreenUnitVector labelAxisScreenPoint = mymtl::to_vector<typename ScreenUnitVector::value_type, ScreenUnitVector::size()>(originScreenPoint + (labelPosition - clampedOrigin) * axisDirectionScreenUnitsPerClientUnit);
			displayLabelAt(hdc, rasteriser, la, labelPosition, axisDirUnitVectorScreenUnits, axisUnitNormalScreenUnits, labelAxisScreenPoint);

			labelCount++;
			labelPosition = computeLabelPosition(labelCount);
		}
	}

	// Draw ticks form origin + offset to axis max
	if (ends_[kAxisEndIndex] - la.axisEndOffset > clampedOrigin + la.originOffset && la.maxCount > 0) {
		const auto computeLabelPosition = [origin = clampedOrigin + la.originOffset, step = la.step](auto n) noexcept { return origin + n * step; };

		std::remove_cv_t<decltype(la.maxCount)> labelCount = 0;
		auto labelPosition = computeLabelPosition(labelCount);
		while (ends_[kAxisEndIndex] - la.axisEndOffset >= labelPosition && labelCount < la.maxCount) {
			const ScreenUnitVector labelAxisScreenPoint = mymtl::to_vector<typename ScreenUnitVector::value_type, ScreenUnitVector::size()>(originScreenPoint + (labelPosition - clampedOrigin) * axisDirectionScreenUnitsPerClientUnit);
			displayLabelAt(hdc, rasteriser, la, labelPosition, axisDirUnitVectorScreenUnits, axisUnitNormalScreenUnits, labelAxisScreenPoint);

			labelCount++;
			labelPosition = computeLabelPosition(labelCount);
		}
	}

	rasteriser.setTextAlign(hdc, prevTextAlignment);
}

template<typename SU, typename CU, typename Formatter>
template<typename TU>
MYMTL_INLINE constexpr void Axis2D<SU, CU, Formatter>::displayLabelAt(HDC hdc, Rasteriser rasteriser, const LabelAttributes& la, ClientUnit labelPosition, 
	const mymtl::Vector2<TU>& axisDirUnitVector, const mymtl::Vector2<TU>& axisUnitNormal, const ScreenUnitVector& labelAxisScreenPoint) const noexcept {

	const ScreenUnitVector referencePoint = labelAxisScreenPoint + mymtl::to_vector<typename ScreenUnitVector::value_type, ScreenUnitVector::size()>(axisDirUnitVector * la.textOffset.x + axisUnitNormal * la.textOffset.y);
	rasteriser.textOut(hdc, referencePoint, la.formatter(labelPosition));
}

MY_COORD_SPACE_END

#endif // CartesianAxis2D_inl