#ifndef CartesianSpace2D_inl
#define CartesianSpace2D_inl

#include "CoordSpace2D.h"
#include <utility>
#include <algorithm>

MY_COORD_SPACE_BEGIN


//------------------------------------------------------------------------------
// Rect class implementation
//------------------------------------------------------------------------------

//template< typename T, template< typename > class ComparePolicy >
//MYMTL_INLINE GRect< T, ComparePolicy >::GRect( T initLeft, T initTop, T initRight, T initBottom )
//	: left( initLeft ), top( initTop ), right( initRight ), bottom( initBottom ) 
//{}
//
//template< typename T, template< typename > class ComparePolicy >
//MYMTL_INLINE GRect< T, ComparePolicy >::GRect( const GRect& src )
//	: left( src.left ), top( src.top ), right( src.right ), bottom( src.bottom ) 
//{}
//
//template< typename T, template< typename > class ComparePolicy >
//MYMTL_INLINE GRect< T, ComparePolicy >&
//	GRect< T, ComparePolicy >::operator = ( const GRect& rhs )
//{
//	if ( &rhs != this )
//	{
//		left	= rhs.left;
//		top		= rhs.top;
//		right	= rhs.right;
//		bottom	= rhs.bottom;
//	}
//
//	return *this;
//}
//
//template< typename T, template< typename > class ComparePolicy >
//MYMTL_INLINE T GRect< T, ComparePolicy >::width() const
//{
//	return right - left;
//}
//
//template< typename T, template< typename > class ComparePolicy >
//MYMTL_INLINE T GRect< T, ComparePolicy >::height() const
//{
//	return bottom - top;
//}

//template< typename T, template< typename > class ComparePolicy >
//MYMTL_INLINE bool Rect2< T, ComparePolicy >::isEmpty() const
//{
//	return ( ( width() <= 0 ) || ( height() <= 0 ) );
//}
//

//template< typename T, template< typename > class ComparePolicy >
//MYMTL_INLINE bool GRect< T, ComparePolicy >::operator == ( const GRect& rhs ) const
//{
//	return ( isEqual( left, rhs.left ) && isEqual( top, rhs.top )
//				&& isEqual( right, rhs.right ) && isEqual( bottom, rhs.bottom ) );
//}
//
//template< typename T, template< typename > class ComparePolicy >
//MYMTL_INLINE bool GRect< T, ComparePolicy >::operator != ( const GRect& rhs ) const
//{
//	return !( operator ==( rhs ) );
//}
namespace {
    template<typename Unit>
    MYMTL_NO_DISCARD MYMTL_INLINE constexpr auto spaceToRect(const CoordSpaceBasis2D<Unit>& space2D) noexcept {
        return mymtl::Rect2<Unit>{
            (std::min)((std::min)(space2D.pts_[0].x, space2D.pts_[1].x), space2D.pts_[2].x),
                (std::min)((std::min)(space2D.pts_[0].y, space2D.pts_[1].y), space2D.pts_[2].y),
                (std::max)((std::max)(space2D.pts_[0].x, space2D.pts_[1].x), space2D.pts_[2].x),
                (std::max)((std::max)(space2D.pts_[0].y, space2D.pts_[1].y), space2D.pts_[2].y),
        };
    }

    template<typename Unit>
    MYMTL_NO_DISCARD MYMTL_INLINE constexpr mymtl::Rect2<Unit> getOrComputeScreenViewport(std::optional<mymtl::Rect2<Unit>> screenViewportOptional,
        const CoordSpaceBasis2D<Unit>& space) noexcept {
        return (screenViewportOptional) ? *screenViewportOptional : spaceToRect(space);
    }
};



template<typename Unit>
MYMTL_INLINE constexpr CoordSpaceBasis2D<Unit>::CoordSpaceBasis2D(const mymtl::Vector2<Unit>& pt1, const mymtl::Vector2<Unit>& pt2, const mymtl::Vector2<Unit>& pt3) noexcept
    : pts_{ pt1, pt2, pt3 } {}


template<typename Unit>
MYMTL_INLINE constexpr CoordSpaceBasis2D<Unit>::CoordSpaceBasis2D(const std::array<mymtl::Vector2<Unit>, 3>& pts) noexcept
    :pts_{ pts } {}



//------------------------------------------------------------------------------
// CoordSpace2D class implementation
//------------------------------------------------------------------------------
template<typename SU, typename CU, typename TU>
MYMTL_INLINE constexpr CoordSpace2D<SU, CU, TU>::CoordSpace2D(const ClientSpace& clientSpace, const ScreenSpace& screenSpace) noexcept
    : clientSpace_{ clientSpace }, screenSpace_{ screenSpace }
{
    recalculateTransformation();
}

template<typename SU, typename CU, typename TU>
MYMTL_INLINE constexpr CoordSpace2D<SU, CU, TU>::CoordSpace2D(const ClientSpace& clientSpace, const TransformationMatrix& m) noexcept
    : clientSpace_{ clientSpace }, transfMat_{ m }
{
    computeCoordSpace(transfMat_, clientSpace_, screenSpace_);
    inverse_matrix(transfMatInv_, static_cast<TransformationMatrix::value_type*>(nullptr), transfMat_);
}

//template<typename SU, typename CU, typename TU>
//MYMTL_INLINE CoordSpace2D<SU, CU, TU>::CoordSpace2D(const std::optional< ScreenRect > screenClipRectOptional, const ScreenSpace& screenSpace, const ClientSpace& clientSpace,
//	typename const CoordAxis::AxisInfo& absciss, typename const CoordAxis::AxisInfo& ordinate) noexcept
//	: screenClipRect_{ getOrComputeScreenViewport(screenClipRectOptional, screenSpace) }, screenSpace_{ screenSpace }, clientSpace_{ clientSpace }, recalculateInvMatrix_{ true },
//	axis_{ Axis2D<ScreenUnit, ClientUnit>{ absciss }, Axis2D<ScreenUnit, ClientUnit>{ ordinate } }
//{
//	computeTransformationMatrix();
//
//	inverse_matrix(transfMatInv_, static_cast<std::remove_pointer_t<TransformationMatrix::value_type>*>(nullptr), transfMat_);
//	recalculateInvMatrix_ = false;
//
//	clientClipRect_ = (screenClipRectOptional) ? screenToClientRect(*screenClipRectOptional) : spaceToRect(clientSpace);
//}

// Raplaces current matrix by the given one
template<typename SU, typename CU, typename TU>
MYMTL_INLINE constexpr auto& CoordSpace2D<SU, CU, TU>::loadMatrix(const TransformationMatrix& m, bool updateTransformation) {
    transfMat_ = m;

    if (updateTransformation) {
        computeCoordSpace(transfMat_, clientSpace_, screenSpace_);
        inverse_matrix(transfMatInv_, static_cast<TransformationMatrix::value_type*>(nullptr), transfMat_);
    }

    return *this;
}

// Raplaces current matrix by the unit one
template<typename SU, typename CU, typename TU>
MYMTL_INLINE constexpr auto& CoordSpace2D<SU, CU, TU>::loadIdentity() noexcept {
    MatrixIdentity(transfMat_);
    MatrixIdentity(transfMatInv_);

    computeCoordSpace(transfMat_, clientSpace_, screenSpace_);

    return *this;
}

template<typename SU, typename CU, typename TU>
MYMTL_INLINE constexpr auto& CoordSpace2D<SU, CU, TU>::multMatrixRight(typename const TransformationMatrix& m, bool updateTransformation) noexcept {
    transfMat_ *= m;

    if (updateTransformation) {
        computeCoordSpace(transfMat_, clientSpace_, screenSpace_);
        inverse_matrix(transfMatInv_, static_cast<TransformationMatrix::value_type*>(nullptr), transfMat_);
    }

    return *this;
}

template<typename SU, typename CU, typename TU>
MYMTL_INLINE constexpr auto& CoordSpace2D<SU, CU, TU>::multMatrix(const TransformationMatrix& m, bool updateTransformation) noexcept {
    transfMat_ = m * transfMat_;

    if (updateTransformation) {
        computeCoordSpace(transfMat_, clientSpace_, screenSpace_);
        inverse_matrix(transfMatInv_, static_cast<TransformationMatrix::value_type*>(nullptr), transfMat_);
    }

    return *this;
}

template<typename SU, typename CU, typename TU>
MYMTL_INLINE constexpr const auto& CoordSpace2D<SU, CU, TU>::getTramsformationMatrix() const noexcept {
    return transfMat_;
}

template<typename SU, typename CU, typename TU>
template<typename X, typename Y, typename Z>
MYMTL_INLINE constexpr auto CoordSpace2D<SU, CU, TU>::clientToScreen(X x, Y y, Z z) const {
    return xyzTransformInternal(x, y, z, transfMat_);
}

template<typename SU, typename CU, typename TU>
template<typename U, typename Z>
MYMTL_INLINE constexpr auto CoordSpace2D<SU, CU, TU>::clientToScreen(const mymtl::Vector2<U>& v, Z z) const {
    return xyzTransformInternal(v.x, v.y, z, transfMat_);
}

template<typename SU, typename CU, typename TU>
template<typename X, typename Y>
MYMTL_INLINE constexpr mymtl::Vector2<typename CoordSpace2D<SU, CU, TU>::ScreenUnit> CoordSpace2D<SU, CU, TU>::clientToScreenPoint(X x, Y y) const {
    const auto vectorHomScreenUnits = clientToScreen(x, y, 1);
    return { static_cast<ScreenUnit>(vectorHomScreenUnits.x), static_cast<ScreenUnit>(vectorHomScreenUnits.y) };
}

template<typename SU, typename CU, typename TU>
template<typename U>
MYMTL_INLINE constexpr mymtl::Vector2<typename CoordSpace2D<SU, CU, TU>::ScreenUnit> CoordSpace2D<SU, CU, TU>::clientToScreenPoint(const mymtl::Vector2<U>& v) const {
    const auto vectorHomScreenUnits = clientToScreen(v, 1);
    return { static_cast<ScreenUnit>(vectorHomScreenUnits.x), static_cast<ScreenUnit>(vectorHomScreenUnits.y) };
}

template<typename SU, typename CU, typename TU>
template<typename X, typename Y>
MYMTL_INLINE constexpr mymtl::Vector2<typename CoordSpace2D<SU, CU, TU>::ScreenUnit> CoordSpace2D<SU, CU, TU>::clientToScreenVector(X x, Y y) const {
    const auto vectorHomScreenUnits = clientToScreen(x, y, 0);
    return { static_cast<ScreenUnit>(vectorHomScreenUnits.x), static_cast<ScreenUnit>(vectorHomScreenUnits.y) };
}

template<typename SU, typename CU, typename TU>
template<typename U>
MYMTL_INLINE constexpr mymtl::Vector2<typename CoordSpace2D<SU, CU, TU>::ScreenUnit> CoordSpace2D<SU, CU, TU>::clientToScreenVector(const mymtl::Vector2<U>& v) const {
    const auto vectorHomScreenUnits = clientToScreen(v, 0);
    return { static_cast<ScreenUnit>(vectorHomScreenUnits.x), static_cast<ScreenUnit>(vectorHomScreenUnits.y) };
}

template<typename SU, typename CU, typename TU>
template<typename X, typename Y, typename Z>
MYMTL_INLINE constexpr auto CoordSpace2D<SU, CU, TU>::screenToClient(X x, Y y, Z z) const {
    return xyzTransformInternal(x, y, z, transfMatInv_);
}

template<typename SU, typename CU, typename TU>
template<typename U, typename Z>
MYMTL_INLINE constexpr auto CoordSpace2D<SU, CU, TU>::screenToClient(const mymtl::Vector2<U>& v, Z z) const {
    return xyzTransformInternal(v.x, v.y, z, transfMatInv_);
}

//template<typename SU, typename CU, typename TU>
//MYMTL_INLINE void CoordSpace2D<SU, CU, TU>::setScreenClipRect(const ScreenRect& r) {
//	screenClipRect_ = r;
//	clientClipRect_ = screenToClientRect(screenClipRect_);
//}


//template<typename SU, typename CU, typename TU>
//MYMTL_INLINE void CoordSpace2D<SU, CU, TU>::setClientClipRect(const ClientRect& r) {
//	clientClipRect_ = r;
//	screenClipRect_ = clientToScreenRect(clientClipRect_);
//}

template<typename SU, typename CU, typename TU>
MYMTL_INLINE constexpr auto& CoordSpace2D<SU, CU, TU>::setClientSpace(const ClientSpace& clientSpace, bool recalculateTransformation) noexcept {
    clientSpace_ = clientSpace;

    if (recalculateTransformation) {
        this->recalculateTransformation();
    }

    return *this;
}

template<typename SU, typename CU, typename TU>
MYMTL_INLINE constexpr const typename CoordSpace2D<SU, CU, TU>::ClientSpace& CoordSpace2D<SU, CU, TU>::getClientSpace() const noexcept {
    return clientSpace_;
}

template<typename SU, typename CU, typename TU>
MYMTL_INLINE constexpr auto& CoordSpace2D<SU, CU, TU>::setScreenSpace(const ScreenSpace& screenSpace, bool recalculateTransformation) noexcept {
    screenSpace_ = screenSpace;

    if (recalculateTransformation) {
        this->recalculateTransformation();
    }

    return *this;
}

template<typename SU, typename CU, typename TU>
MYMTL_INLINE constexpr const typename CoordSpace2D<SU, CU, TU>::ScreenSpace& CoordSpace2D<SU, CU, TU>::getScreenSpace() const noexcept {
    return screenSpace_;
}

template<typename SU, typename CU, typename TU>
MYMTL_INLINE constexpr void CoordSpace2D<SU, CU, TU>::recalculateTransformation() noexcept {
    computeTransformationMatrix(transfMat_);
    inverse_matrix(transfMatInv_, static_cast<TransformationMatrix::pointer>(nullptr), transfMat_);
}

template<typename SU, typename CU, typename TU>
MYMTL_INLINE constexpr void CoordSpace2D<SU, CU, TU>::computeTransformationMatrix(TransformationMatrix& m) noexcept {
    const auto denominator = static_cast<TransfUnit>(clientSpace_.pts_[2].x - clientSpace_.pts_[1].x) * clientSpace_.pts_[0].y
        + static_cast<TransfUnit>(clientSpace_.pts_[0].x - clientSpace_.pts_[2].x) * clientSpace_.pts_[1].y
        + static_cast<TransfUnit>(clientSpace_.pts_[1].x - clientSpace_.pts_[0].x) * clientSpace_.pts_[2].y;

    m(0, 0) = (static_cast<TransfUnit>(screenSpace_.pts_[2].x - screenSpace_.pts_[1].x) * clientSpace_.pts_[0].y
        + static_cast<TransfUnit>(screenSpace_.pts_[0].x - screenSpace_.pts_[2].x) * clientSpace_.pts_[1].y
        + static_cast<TransfUnit>(screenSpace_.pts_[1].x - screenSpace_.pts_[0].x) * clientSpace_.pts_[2].y) / denominator;

    m(0, 1) = (static_cast<TransfUnit>(screenSpace_.pts_[2].y - screenSpace_.pts_[1].y) * clientSpace_.pts_[0].y
        + static_cast<TransfUnit>(screenSpace_.pts_[0].y - screenSpace_.pts_[2].y) * clientSpace_.pts_[1].y
        + static_cast<TransfUnit>(screenSpace_.pts_[1].y - screenSpace_.pts_[0].y) * clientSpace_.pts_[2].y) / denominator;

    m(0, 2) = 0;

    m(1, 0) = (static_cast<TransfUnit>(screenSpace_.pts_[1].x - screenSpace_.pts_[2].x) * clientSpace_.pts_[0].x
        + static_cast<TransfUnit>(screenSpace_.pts_[2].x - screenSpace_.pts_[0].x) * clientSpace_.pts_[1].x
        + static_cast<TransfUnit>(screenSpace_.pts_[0].x - screenSpace_.pts_[1].x) * clientSpace_.pts_[2].x) / denominator;

    m(1, 1) = (static_cast<TransfUnit>(screenSpace_.pts_[1].y - screenSpace_.pts_[2].y) * clientSpace_.pts_[0].x
        + static_cast<TransfUnit>(screenSpace_.pts_[2].y - screenSpace_.pts_[0].y) * clientSpace_.pts_[1].x
        + static_cast<TransfUnit>(screenSpace_.pts_[0].y - screenSpace_.pts_[1].y) * clientSpace_.pts_[2].x) / denominator;

    m(1, 2) = 0;

    m(2, 0) = (static_cast<TransfUnit>(screenSpace_.pts_[2].x * clientSpace_.pts_[1].y - screenSpace_.pts_[1].x * clientSpace_.pts_[2].y) * clientSpace_.pts_[0].x
        + static_cast<TransfUnit>(screenSpace_.pts_[0].x * clientSpace_.pts_[2].y - screenSpace_.pts_[2].x * clientSpace_.pts_[0].y) * clientSpace_.pts_[1].x
        + static_cast<TransfUnit>(screenSpace_.pts_[1].x * clientSpace_.pts_[0].y - screenSpace_.pts_[0].x * clientSpace_.pts_[1].y) * clientSpace_.pts_[2].x) / denominator;

    m(2, 1) = (static_cast<TransfUnit>(screenSpace_.pts_[2].y * clientSpace_.pts_[1].y - screenSpace_.pts_[1].y * clientSpace_.pts_[2].y) * clientSpace_.pts_[0].x
        + static_cast<TransfUnit>(screenSpace_.pts_[0].y * clientSpace_.pts_[2].y - screenSpace_.pts_[2].y * clientSpace_.pts_[0].y) * clientSpace_.pts_[1].x
        + static_cast<TransfUnit>(screenSpace_.pts_[1].y * clientSpace_.pts_[0].y - screenSpace_.pts_[0].y * clientSpace_.pts_[1].y) * clientSpace_.pts_[2].x) / denominator;

    m(2, 2) = 1;
}

template<typename SU, typename CU, typename TU>
MYMTL_INLINE constexpr typename CoordSpace2D<SU, CU, TU>::ScreenRect CoordSpace2D<SU, CU, TU>::clientToScreenRect(const ClientRect& r) const {
    const auto screenTopLeftPointH = xyzTransformInternal(r.left, r.top, 1, transfMat_), screenBottomRightPointH = xyzTransformInternal(r.right, r.bottom, 1, transfMat_);
    return ScreenRect{ static_cast<ScreenRect::value_type>(screenTopLeftPointH.x), static_cast<ScreenRect::value_type>(screenTopLeftPointH.y),
        static_cast<ScreenRect::value_type>(screenBottomRightPointH.x), static_cast<ScreenRect::value_type>(screenBottomRightPointH.y) };
}

template<typename SU, typename CU, typename TU>
MYMTL_INLINE constexpr typename CoordSpace2D<SU, CU, TU>::ClientRect CoordSpace2D<SU, CU, TU>::screenToClientRect(const ScreenRect& r) const {
    const auto clientTopLeftPointH = xyzTransformInternal(r.left, r.top, 1, transfMatInv_), clientBottomRightPointH = xyzTransformInternal(r.right, r.bottom, 1, transfMatInv_);
    return ClientRect{ static_cast<ClientRect::value_type>(clientTopLeftPointH.x), static_cast<ClientRect::value_type>(clientTopLeftPointH.y),
        static_cast<ClientRect::value_type>(clientBottomRightPointH.x), static_cast<ClientRect::value_type>(clientBottomRightPointH.y) };
}


template<typename SU, typename CU, typename TU>
template<typename X, typename Y, typename Z>
    requires mymtl::NumericType<X>&& mymtl::NumericType<Y>&& mymtl::NumericType<Z>
MYMTL_INLINE constexpr auto CoordSpace2D<SU, CU, TU>::xyzTransformInternal(X x, Y y, Z z, const TransformationMatrix& m) const noexcept {
    return vector_transform(x, y, z, m);
}

template<typename SU, typename CU, typename TU>
template< typename R, typename X, typename Y, typename Z >
    requires mymtl::NumericType<X>&& mymtl::NumericType<Y>&& mymtl::NumericType<Z>
MYMTL_INLINE constexpr void CoordSpace2D<SU, CU, TU>::xyzTransformInternal(mymtl::Vector3<R>& r, X x, Y y, Z z, const TransformationMatrix& m) const noexcept {
    vector_transform(r, x, y, z, m);
}

template<typename SU, typename CU, typename TU>
template<typename InUnit, typename OutUnit>
MYMTL_INLINE constexpr void CoordSpace2D<SU, CU, TU>::computeCoordSpace(const TransformationMatrix& m, const CoordSpaceBasis2D<InUnit>& from, CoordSpaceBasis2D<OutUnit>& to) const noexcept {
    const std::array homPoints{ xyzTransformInternal(from.pt1().x, from.pt1().y, 1, m), xyzTransformInternal(from.pt2().x, from.pt2().y, 1, m), xyzTransformInternal(from.pt3().x, from.pt3().y, 1, m) };

    std::transform(std::execution::unseq, std::cbegin(homPoints), std::cend(homPoints), std::begin(to.pts_),
        [](auto&& p) {
            return mymtl::Vector2<OutUnit>{ static_cast<OutUnit>(p.x), static_cast<OutUnit>(p.y) };
        }
    );
}


MY_COORD_SPACE_END

#endif // !CartesianSpace2D_inl

