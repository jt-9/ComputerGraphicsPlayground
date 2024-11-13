#ifndef CartesianSpace2D_h
#define CartesianSpace2D_h


//#include "MTLValsCore.h"
#include "ApeMath.h"
#include "Rect.h"
#include "CoordSpaceDefs.h"


#include <cassert>

#include <memory>
#include <optional>
#include <array>

//#include <windows.h>


MY_COORD_SPACE_BEGIN


template<typename Unit>
struct CoordSpaceBasis2D {
    CoordSpaceBasis2D() noexcept = default;
    ~CoordSpaceBasis2D() noexcept = default;

    constexpr CoordSpaceBasis2D(const mymtl::Vector2<Unit>& pt1, const mymtl::Vector2<Unit>& pt2, const mymtl::Vector2<Unit>& pt3) noexcept;
    constexpr CoordSpaceBasis2D(const std::array<mymtl::Vector2<Unit>, 3>& pts) noexcept;

    CoordSpaceBasis2D(const CoordSpaceBasis2D& src) noexcept = default;
    CoordSpaceBasis2D& operator = (const CoordSpaceBasis2D& rhs) noexcept = default;

    CoordSpaceBasis2D(CoordSpaceBasis2D&& pts) noexcept = default;
    CoordSpaceBasis2D& operator = (CoordSpaceBasis2D&& rhs) noexcept = default;

    MY_COORD_SPACE_ATTR_NO_DISCARD inline constexpr mymtl::Vector2<Unit>& pt1() noexcept {
        assert(pts_.size() > 0);
        return pts_[0];
    }

    MY_COORD_SPACE_ATTR_NO_DISCARD inline constexpr const mymtl::Vector2<Unit>& pt1() const noexcept {
        assert(pts_.size() > 0);
        return pts_[0];
    }

    MY_COORD_SPACE_ATTR_NO_DISCARD inline constexpr mymtl::Vector2<Unit>& pt2() noexcept {
        return pts_[1];
    }

    MY_COORD_SPACE_ATTR_NO_DISCARD inline constexpr const mymtl::Vector2<Unit>& pt2() const noexcept {
        return pts_[1];
    }

    MY_COORD_SPACE_ATTR_NO_DISCARD inline constexpr mymtl::Vector2<Unit>& pt3() noexcept {
        assert(pts_.size() > 2);
        return pts_[2];
    }

    MY_COORD_SPACE_ATTR_NO_DISCARD inline constexpr const mymtl::Vector2<Unit>& pt3() const noexcept {
        assert(pts_.size() > 2);
        return pts_[2];
    }

    MY_COORD_SPACE_ATTR_NO_DISCARD inline constexpr auto pointsNumber() const noexcept {
        return pts_.size();
    }

    std::array<mymtl::Vector2<Unit>, 3> pts_;
};


//------------------------------------------------------------------------------
// CoordSpace2D class interface
//------------------------------------------------------------------------------

template<typename SU, typename CU, typename TU>
class CoordSpace2D
{
public:
    using ClientUnit = CU;
    using ScreenUnit = SU;
    using TransfUnit = TU;

    using ScreenRect = mymtl::Rect2<ScreenUnit>;
    using ClientRect = mymtl::Rect2<ClientUnit>;

    using ClientSpace = CoordSpaceBasis2D<ClientUnit>;
    using ScreenSpace = CoordSpaceBasis2D<ScreenUnit>;

    //static const unsigned int constAxisCount = 2;
    //enum class AxisID: uint8_t { AN_ABSCISSA = 1, AN_ORDINATE };

    using TransformationMatrix = mymtl::Matrix3<TransfUnit>;
    //using CoordAxis = Axis2D<ScreenUnit, ClientUnit>;

    using ScreenPoint = mymtl::Vector2<ScreenUnit>;
    using ClientPoint = mymtl::Vector2<ClientUnit>;

    static constexpr std::int16_t kDimensions = 2;

public:
    // standard constructor
    constexpr CoordSpace2D(const ClientSpace& clientSpace, const ScreenSpace& screenSpace) noexcept;

    constexpr CoordSpace2D(const ClientSpace& clientSpace, const TransformationMatrix& m) noexcept;

    CoordSpace2D(const CoordSpace2D& src) = delete;
    CoordSpace2D& operator =(const CoordSpace2D& rhs) = delete;

    // replaces current matrix with the given
    constexpr auto& loadMatrix( const TransformationMatrix& m, bool updateTransformation);

    // replaces current matrix with the identity
    constexpr auto& loadIdentity() noexcept;
    
    // Determines the product of the given matrix and the current matrix
    // (transformation is about the local origin of the object)
    constexpr auto& multMatrix( const TransformationMatrix& m, bool recalculateTransformation) noexcept;

    // Determines the product of the current matrix and the given matrix
    // (transformation is about the current world origin)
    constexpr auto& multMatrixRight(const TransformationMatrix& m, bool recalculateTransformation) noexcept;

    //void rotate( gfloat phi );
    //void rotateLocal( gfloat phi );

    // Returns current transformation matrix
    MY_COORD_SPACE_ATTR_NO_DISCARD constexpr const auto& getTramsformationMatrix() const noexcept;
    
    template<typename X, typename Y, typename Z>
    constexpr auto clientToScreen(X x, Y y, Z z) const;

    template<typename U, typename Z>
    constexpr auto clientToScreen(const mymtl::Vector2<U>& v, Z z) const;

    template<typename X, typename Y>
    constexpr mymtl::Vector2<ScreenUnit> clientToScreenPoint(X x, Y y) const;

    template<typename U>
    constexpr mymtl::Vector2<ScreenUnit> clientToScreenPoint(const mymtl::Vector2<U>& v) const;

    template<typename X, typename Y>
    constexpr mymtl::Vector2<ScreenUnit> clientToScreenVector(X x, Y y) const;

    template<typename U>
    constexpr mymtl::Vector2<ScreenUnit> clientToScreenVector(const mymtl::Vector2<U>& v) const;

    template<typename X, typename Y, typename Z>
    constexpr auto screenToClient(X x, Y y, Z z) const;

    template<typename U, typename Z>
    constexpr auto screenToClient(const mymtl::Vector2<U>& v, Z z) const;
        
    constexpr auto& setClientSpace(const ClientSpace &clientSpace, bool recalculateTransformation) noexcept;
    MY_COORD_SPACE_ATTR_NO_DISCARD constexpr const ClientSpace& getClientSpace() const noexcept;

    constexpr auto& setScreenSpace(const ScreenSpace &screenSpace, bool recalculateTransformation) noexcept;
    MY_COORD_SPACE_ATTR_NO_DISCARD constexpr const ScreenSpace& getScreenSpace() const noexcept;

    constexpr void recalculateTransformation() noexcept;

private:

    // Calculates world to screen
    constexpr void computeTransformationMatrix(TransformationMatrix& m) noexcept;
    
    MY_COORD_SPACE_ATTR_NO_DISCARD constexpr ClientRect screenToClientRect(const ScreenRect& r) const;
    MY_COORD_SPACE_ATTR_NO_DISCARD constexpr ScreenRect clientToScreenRect(const ClientRect& r) const;

    template< typename X, typename Y, typename Z >
        requires mymtl::NumericType<X>&& mymtl::NumericType<Y>&& mymtl::NumericType<Z>
    MY_COORD_SPACE_ATTR_NO_DISCARD constexpr auto xyzTransformInternal(X x, Y y, Z z, const TransformationMatrix& m) const noexcept;

    template< typename R, typename X, typename Y, typename Z >
        requires mymtl::NumericType<X>&& mymtl::NumericType<Y>&& mymtl::NumericType<Z>
    constexpr void xyzTransformInternal(mymtl::Vector3<R>& r, X x, Y y, Z z, const TransformationMatrix& m) const noexcept;

    template<typename InUnit, typename OutUnit>
    constexpr void computeCoordSpace(const TransformationMatrix& m, const CoordSpaceBasis2D<InUnit>& from, CoordSpaceBasis2D<OutUnit>& to) const noexcept;

    // Updates axes' coordinates
    //void updateAxesAttrib();
    
    //void updateLabelAttrib( AxisID axisID );
    
    // Depicts ticks of the specified axis's part
//	void drawPartTicks( HDC hdc, const GVector2 *pTickPoints, gfloat axisPart, gfloat wTickStep, const GVector2& axisUnit );
    
    //gfloat calcLabelOrientation( const GVector2& axisStartPt, const GVector2& axisEndPt ) const;

    //// font height is ignored for now
    //GVector2& calcLabelParams(GVector2& indent, unsigned int &textAlign,
    //					unsigned short indentLength,  const Axis2D::AxisInfo& aui, const GVector2 &dir ) const;
    //
    //void drawPartLabels( HDC hdc, const GVector2& labelPoint, gfloat wFirstValue, gfloat axisPart, gfloat wLabelStep,
    //					/*const mymtl::GVector2D &labelIndent,*/ const GVector2& axisUnit );
//	bool Line ( HDC hdc, double Xstart, double Ystart, double Xend, double Yend );
//	bool Line( HDC hdc, WORLDPOINT Start, WORLDPOINT End );
//	
//	void SetDireciton( AxisID uiAxisID, bool bForward = true ); // use AN_ABSCISSA for X and AN_ORDINATE accordingly for Y
//	
//	COLORREF SetPoint(HDC hdc, WORLDPOINT wpoint, COLORREF crColor); // Same as SetPixel but using world coordinates
//	COLORREF SetPoint( HDC hdc, double X, double Y, COLORREF crColor );

protected: // data

    ClientSpace clientSpace_;
    ScreenSpace screenSpace_;

    // Current transformation matrix
    TransformationMatrix transfMat_; // = leftMultMat_ * worldToScreenMat_ * rightMultMat_
    TransformationMatrix transfMatInv_; // Inverse matrix for transfMat_
};

MY_COORD_SPACE_END

#include "CoordSpace2D-inl.h"

#endif // !CartesianSpace2D_h
