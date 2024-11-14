#ifndef CartesianCoordSys2D_h
#define CartesianCoordSys2D_h

#include "ApeMath.h"
#include "CoordSpace2D.h"
#include "Rasteriser.h"
#include "CoordAxis2D.h"

#include <array>
#include <optional>

MY_COORD_SPACE_BEGIN

template<typename SU, typename CU, typename TU, typename Formatter>
class CartesianCoordSys2D {
public:
    using Space = CoordSpace2D<SU, CU, TU>;
    using ClientUnit = Space::ClientUnit;
    using ScreenUnit = Space::ScreenUnit;
    using TransfUnit = Space::TransfUnit;

    using Axis = Axis2D<ScreenUnit, ClientUnit, Formatter>;
    using Axes = std::array<Axis, Space::kDimensions>;
    using ClientVector = mymtl::VectorN<ClientUnit, Space::kDimensions>;
    using ScreenVector = mymtl::VectorN<ScreenUnit, Space::kDimensions>;

    using AxisAttributes = typename Axis::AxisAttributes;
    using TickAttributes = typename Axis::TickAttributes;

    using LabelAttributes = typename Axis::LabelAttributes;

    struct BoundingBox {
        ClientVector min;
        ClientVector max;

        constexpr auto size() const noexcept {
            return min.size();
        }

        constexpr bool is_empty() const noexcept {
            return size() == 0;
        }
    };

public:
    constexpr CartesianCoordSys2D(const Space::ClientSpace& clientSpace, const Space::ScreenSpace& screenSpace, const ClientVector& origin = { 0 },
        const std::optional<std::array<std::optional<AxisAttributes>, Space::kDimensions>>& aa = std::nullopt, 
        const std::optional<std::array<std::optional<TickAttributes>, Space::kDimensions>>& ta = std::nullopt,
        const std::optional<std::array<std::optional<LabelAttributes>, Space::kDimensions>>& la = std::nullopt) noexcept;

    constexpr CartesianCoordSys2D(const Space::ClientSpace& clientSpace, const Space::TransformationMatrix& m, const ClientVector& origin = { 0 },
        const std::optional<std::array<std::optional<AxisAttributes>, Space::kDimensions>>& aa = std::nullopt,
        const std::optional<std::array<std::optional<TickAttributes>, Space::kDimensions>>& ta = std::nullopt,
        const std::optional<std::array<std::optional<LabelAttributes>, Space::kDimensions>>& la = std::nullopt) noexcept;

    constexpr CartesianCoordSys2D(const Space::ClientSpace& clientSpace, const Space::ScreenSpace& screenSpace, const BoundingBox& boundBox, const ClientVector& origin = { 0 },
        const std::optional<std::array<std::optional<AxisAttributes>, Space::kDimensions>>& aa = std::nullopt,
        const std::optional<std::array<std::optional<TickAttributes>, Space::kDimensions>>& ta = std::nullopt,
        const std::optional<std::array<std::optional<LabelAttributes>, Space::kDimensions>>& la = std::nullopt) noexcept;

    constexpr CartesianCoordSys2D(const Space::ClientSpace& clientSpace, const Space::TransformationMatrix& m, const BoundingBox& boundBox, const ClientVector& origin = { 0 },
        const std::optional<std::array<std::optional<AxisAttributes>, Space::kDimensions>>& aa = std::nullopt,
        const std::optional<std::array<std::optional<TickAttributes>, Space::kDimensions>>& ta = std::nullopt,
        const std::optional<std::array<std::optional<LabelAttributes>, Space::kDimensions>>& la = std::nullopt) noexcept;
    constexpr ~CartesianCoordSys2D() noexcept = default;

    constexpr CartesianCoordSys2D(CartesianCoordSys2D&& src) noexcept = default;
    constexpr CartesianCoordSys2D& operator=(CartesianCoordSys2D&& src) noexcept = default;

    CartesianCoordSys2D(const CartesianCoordSys2D& src) noexcept = delete;
    CartesianCoordSys2D& operator=(const CartesianCoordSys2D& rhs) noexcept = delete;

    constexpr auto& setClientSpace(const Space::ClientSpace& clientSpace, bool recalculateTransformation) noexcept;
    MY_COORD_SPACE_ATTR_NO_DISCARD constexpr const Space::ClientSpace& getClientSpace() const noexcept;

    constexpr auto& setScreenSpace(const Space::ScreenSpace& screenSpace, bool recalculateTransformation) noexcept;
    MY_COORD_SPACE_ATTR_NO_DISCARD constexpr const Space::ScreenSpace& getScreenSpace() const noexcept;

    constexpr auto& setOrigin(const ClientVector& origin) noexcept;
    MY_COORD_SPACE_ATTR_NO_DISCARD constexpr ClientVector getOrigin() noexcept;

    constexpr auto& setAxisPoint(const ClientVector& endPoint, typename Axis::Name axisName, std::uint8_t axisPointIndex) noexcept;

    constexpr void recalculateTransformation() noexcept;

    constexpr auto moveTo(Rasteriser r, ClientUnit x, ClientUnit y) const noexcept;
    constexpr auto lineTo(Rasteriser r, ClientUnit x, ClientUnit y) const noexcept;
    constexpr auto line(Rasteriser r, ClientUnit fromX, ClientUnit fromY, ClientUnit toX, ClientUnit toY) const noexcept;
    constexpr auto setPixel(Rasteriser r, ClientUnit x, ClientUnit y, unsigned long colour) const noexcept;
    constexpr auto ellipse(Rasteriser r, ClientUnit left, ClientUnit top, ClientUnit right, ClientUnit bottom) const noexcept;

    constexpr void draw(Rasteriser r) const noexcept;

    [[nodiscard]] static constexpr ClientVector clampVector(const ClientVector& v, const BoundingBox& bb) noexcept;

private:
    [[nodiscard]] static constexpr BoundingBox fromClientSpace(const typename Space::ClientSpace& clientSpace) noexcept {
        BoundingBox bb{ .min = clientSpace.pt1(), .max = clientSpace.pt1() };

        for (const auto& p : clientSpace.pts_) {
            for (std::remove_cv_t<decltype(bb.min.size())> i = 0; i < bb.min.size(); i++) {
                bb.min[i] = (std::min)(bb.min[i], p[i]);
                bb.max[i] = (std::max)(bb.max[i], p[i]);
            }
        }

        return bb;
    }

    [[nodiscard]] static constexpr auto mapAxisNameToIndex(typename Axis::Name axisName) noexcept {
        assert((axisName >= 0) && (axisName < Space::kDimensions));
        return static_cast<unsigned short>(axisName);
    }

    template<typename T, std::size_t N>
    [[nodiscard]] static constexpr std::optional<T> getAttributesForAxis(const std::optional<std::array<std::optional<T>, N>>& optAttributes, typename Axis::Name axisName) noexcept {
        return optAttributes.has_value() ? optAttributes->at(mapAxisNameToIndex(axisName)) : std::nullopt;
    }

    [[nodiscard]] static constexpr std::array<ClientUnit, Axis::kAxisEndsNumber> computeAxisEnds(const BoundingBox& boundBox, typename Axis::Name axisName) noexcept {
        const auto axisIndex = mapAxisNameToIndex(axisName);
        return std::array{ boundBox.min[axisIndex], boundBox.max[axisIndex] };
    }

    [[nodiscard]] static constexpr ClientUnit computeAxisOrigin(const ClientVector& origin, typename Axis::Name axisName) noexcept {
        const auto axisIndex = mapAxisNameToIndex(axisName);

        return origin[axisIndex];
    }

    [[nodiscard]] static constexpr std::array<ClientVector, Axis::kAxisEndsNumber> computeAxisEndPoints(const BoundingBox& boundBox,
        const ClientVector& origin, typename Axis::Name axisName) noexcept {

        const auto axisIndex = mapAxisNameToIndex(axisName);
        const auto axisStartPointIndex = std::to_underlying(Axis::PointIndex::Start), 
            axisEndPointIndex = std::to_underlying(Axis::PointIndex::End);

        const auto clampedOrigin = clampVector(origin, boundBox);
        std::array endPoints{ clampedOrigin, clampedOrigin };

        endPoints[axisStartPointIndex][axisIndex] = boundBox.min[axisIndex];
        endPoints[axisEndPointIndex][axisIndex] = boundBox.max[axisIndex];

        return endPoints;
    }

private:
    Space space_;
    Axes axes_;
};


MY_COORD_SPACE_END

#include "CartesianCoordSys2D-inl.h"

#endif // !CartesianCoordSys2D_h

