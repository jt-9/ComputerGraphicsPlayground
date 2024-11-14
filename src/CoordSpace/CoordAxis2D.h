#ifndef CartesianAxis2D_h
#define CartesianAxis2D_h

#pragma once

#include "CoordSpaceDefs.h"
#include "ApeMath.h"
#include "Rasteriser.h"
#include "CoordSpace2D.h"

#include <Windows.h> // for HDC and other window types

#include <format>
#include <optional>
#include <cassert>

MY_COORD_SPACE_BEGIN


//#ifdef  UNICODE
//using gstring = std::string;
//const gstring constStrFormat{ "%.2f" };
//#else
//using gstring = std::string;
//const gstring constStrFormat{ "%.2f" };
//#endif


const unsigned char constMaxBufLength = 32;

constexpr mymtl::gfloat minLabelDist = 1000 * mymtl::kEps;


constexpr mymtl::gfloat minTickDist = 100 * mymtl::kEps;

const auto defaultFormatter = [](auto v) noexcept
    {
        return std::format("{}", v);
    };

//------------------------------------------------------------------------------
// Axis class interface
//------------------------------------------------------------------------------
template<typename SU, typename CU, typename Formatter>
class Axis2D
{
public:
    using ClientUnit = CU;
    using ScreenUnit = SU;

    using ScreenUnitVector = mymtl::Vector2<ScreenUnit>;
    using ClientUnitVector = mymtl::Vector2<ClientUnit>;

    struct AxisAttributes
    {
        HPEN pen = nullptr; // default
        HBRUSH brush = nullptr; // default
    };

    struct TickAttributes
    {
        using TickSize = std::uint32_t;

        constexpr TickAttributes(
            ClientUnit tickStep,
            TickSize tickUpperPart,
            TickSize tickLowerPart,
            HPEN tickPen = nullptr,
            HBRUSH tickBrush = nullptr,
            ClientUnit offsetFromOrigin = {},
            ClientUnit offsetFromAxisEnd = {},
            std::uint32_t tickMaxCount = (std::numeric_limits<std::uint32_t>::max)()) noexcept
            : step{ tickStep }, upperPart{ tickUpperPart }, lowerPart{ tickLowerPart },
            pen{ tickPen }, brush{ tickBrush }, originOffset{ offsetFromOrigin }, axisEndOffset{ offsetFromAxisEnd },
            maxCount{ tickMaxCount }
        {
            assert(tickStep > 0 && "Failed requirement tick mark step > 0");
        }

        constexpr TickAttributes(const TickAttributes&) noexcept = default;
        constexpr TickAttributes(TickAttributes&&) noexcept = default;

        constexpr ~TickAttributes() noexcept = default;

        constexpr TickAttributes& operator=(const TickAttributes&) noexcept = default;
        constexpr TickAttributes& operator=(TickAttributes&&) noexcept = default;

        ClientUnit step;
        TickSize upperPart;
        TickSize lowerPart;

        HPEN pen; // nullptr default
        HBRUSH brush; // nullptr default

        ClientUnit originOffset;
        ClientUnit axisEndOffset;
        std::uint32_t maxCount;
    };

    struct LabelAttributes
    {
        template<typename F = Formatter>
            requires std::convertible_to<std::remove_cvref_t<F>, Formatter>
        constexpr LabelAttributes(
            ClientUnit labelStep,
            F&& labelFormatter,
            ScreenUnitVector labelTextOffset,
            unsigned int labelTextAlign,
            HFONT labelFont = nullptr,
            COLORREF labelColour = 0,
            ClientUnit offsetFromOrigin = {},
            ClientUnit offsetFromAxisEnd = {},
            std::uint32_t labelMaxCount = (std::numeric_limits<std::uint32_t>::max)()) noexcept
            : step{ labelStep }, formatter{ std::forward<F>(labelFormatter) }, textOffset{ labelTextOffset }, textAlign{ labelTextAlign },
            font{ labelFont }, colour{ labelColour }, originOffset{ offsetFromOrigin }, axisEndOffset{ offsetFromAxisEnd },
            maxCount{ labelMaxCount }
        {
            assert(labelStep > 0 && "Failed requirement label step > 0");
        }

        constexpr LabelAttributes(const LabelAttributes&) noexcept = default;
        constexpr LabelAttributes& operator=(const LabelAttributes&) noexcept = default;

        constexpr ~LabelAttributes() noexcept = default;

        constexpr LabelAttributes(LabelAttributes&&) noexcept = default;
        constexpr LabelAttributes& operator=(LabelAttributes&&) noexcept = default;

        ClientUnit step;
        Formatter formatter;
        ScreenUnitVector textOffset;
        unsigned int textAlign;

        HFONT font; // default
        COLORREF colour; // label color

        ClientUnit originOffset;
        ClientUnit axisEndOffset;
        std::uint32_t maxCount;

        //long			textAngle; // Angle
        //unsigned int	textAlign;
    };

    enum UsingElem : std::uint16_t { use_axis = 1, use_label = 2, use_tick = 4 };

    enum Name
    {
        Abscissa = 0, Ordinate
    };

    static constexpr unsigned short kAxisEndsNumber = 2;
    static constexpr decltype(kAxisEndsNumber) kAxisStartIndex = 0;
    static constexpr decltype(kAxisEndsNumber) kAxisEndIndex = 1;

    // Adjusts axis parameters with default values
    constexpr Axis2D(const std::array<ClientUnitVector, kAxisEndsNumber>& axisEndPoints, const std::array<ClientUnit, kAxisEndsNumber>& axisEnds, ClientUnit origin = {},
        const std::optional<AxisAttributes>& axisAttributes = std::nullopt,
        const std::optional<TickAttributes>& tickAttributes = std::nullopt,
        const std::optional<LabelAttributes>& labelAttributes = std::nullopt) noexcept;

    constexpr Axis2D(const Axis2D&) noexcept = delete;
    constexpr Axis2D& operator=(const Axis2D&) noexcept = delete;

    constexpr Axis2D(Axis2D&&) noexcept = default;
    constexpr Axis2D& operator=(Axis2D&&) noexcept = default;

    constexpr ~Axis2D() = default;

    MYMTL_NO_DISCARD constexpr const auto& getAxisAttrib() const noexcept;
    MYMTL_NO_DISCARD constexpr const auto& getTickAttrib() const noexcept;
    MYMTL_NO_DISCARD constexpr const auto& getLabelAttrib() const noexcept;

    //int getUsedElements() const;
    //int deselUsedElem( UsingElem deselElem );

    constexpr void setStartPoint(const ClientUnitVector& startPoint) noexcept;
    constexpr void setEndPoint(const ClientUnitVector& endPoint) noexcept;

    //	const ClientUnitVector& endPoint) noexcept;

    //void setAxisName( const gstring& newName );
    //const gstring& axisName() const;

    // Sets up axis data with specified values
    //DWORD Create( const std::string &name, bool bForward, const LPAXIS_ATTRIBUTES lpaa = 0, const LPTICK_ATTRIBUTES lpta = 0, const LPLABEL_ATTRIBUTES lpla = 0 );

    template<typename TU>
    constexpr void draw(Rasteriser rasteriser, const CoordSpace2D<SU, CU, TU>& space) const noexcept;

private:
    MYMTL_NO_DISCARD constexpr bool shouldDrawAxis() const noexcept;

    constexpr void drawAxis(Rasteriser rasteriser, const ScreenUnitVector& startPoint, const ScreenUnitVector& endPoint, const AxisAttributes& aa) const noexcept;

    MYMTL_NO_DISCARD constexpr bool shouldDrawTickMarks() const noexcept;

    template<typename TU>
    constexpr void drawTickMarks(Rasteriser rasteriser, const CoordSpace2D<SU, CU, TU>& space, const ScreenUnitVector& startPoint, const ScreenUnitVector& endPoint, const TickAttributes& ta) const noexcept;

    template<typename TU>
    constexpr void drawTickMarkAt(Rasteriser rasteriser, const TickAttributes& ta, const mymtl::Vector2<TU>& axisUnitNormal, const ScreenUnitVector& tickScreenPoint) const noexcept;

    MYMTL_NO_DISCARD constexpr bool shouldDisplayLabels() const noexcept;

    template<typename TU>
    constexpr void displayLabels(Rasteriser rasteriser, const ScreenUnitVector& startPoint, const ScreenUnitVector& endPoint, const LabelAttributes& la) const noexcept;

    template<typename TU>
    constexpr void displayLabelAt(Rasteriser rasteriser, const LabelAttributes& la, ClientUnit labelPosition,
        const mymtl::Vector2<TU>& axisDirUnitVector, const mymtl::Vector2<TU>& axisUnitNormal, const ScreenUnitVector& labelAxisScreenPoint) const noexcept;

public:
    std::array<ClientUnitVector, kAxisEndsNumber> endPoints_;
    std::array<ClientUnit, kAxisEndsNumber> ends_;
    ClientUnit origin_;

private:
    std::optional<AxisAttributes> axisAttr_;
    std::optional<TickAttributes> tickAttr_;
    std::optional<LabelAttributes> labelAttr_;
};

MY_COORD_SPACE_END

#include "CoordAxis2D-inl.h"

#endif // CartesianAxis2D_h
