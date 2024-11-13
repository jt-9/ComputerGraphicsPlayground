#include "View.h"
#include "MemoryDC.hpp"
#include "MyCG.h"
#include "unique_ptr.hpp"
#include "windows_dc_deleter.hpp"

#include <utility>
#include <execution>
#include <tuple>


//using namespace coordsys;

namespace {
	template<typename Unit>
	inline constexpr auto computePadding(Unit length, Unit desiredindent) noexcept {
		return (length > (desiredindent + desiredindent)) ? desiredindent : static_cast<Unit>(0);
	}

	template<typename Intensity>
	inline constexpr auto computeColourGreyScale(Intensity intensity, unsigned char maxIntensity) noexcept {
		return RGB(intensity * maxIntensity, intensity * maxIntensity, intensity * maxIntensity);
	}

	inline auto prepareViewHDC(HWND window) noexcept {
		const auto hdc = GetDC(window);

		return raii::unique_rc<HDC, raii::gdi_release_wnd_dc_nullptr>{hdc, raii::gdi_release_wnd_dc_nullptr{ window }};
	}

	inline void drawEllipse(HDC paintDC, RECT const& clientRect) noexcept {
		const raii::unique_rc<HBRUSH, raii::gdi_delete_object_nullptr<HBRUSH>> solidBrush{ static_cast<HBRUSH>(GetStockObject(DKGRAY_BRUSH)) };
		//FillRect(paintDC, &clientRect, solidBrush.get());
		SIZE clientArea{ abs(clientRect.right - clientRect.left), abs(clientRect.bottom - clientRect.top) };
		SIZE ellipseArea{ 2 * clientArea.cx / 3, 2 * clientArea.cy / 3 };
		SIZE indentSize{ (clientArea.cx - ellipseArea.cx) / 2, (clientArea.cy - ellipseArea.cy) / 2 };

		Ellipse(paintDC, clientRect.left + indentSize.cx, clientRect.top + indentSize.cy, clientRect.right - indentSize.cx, clientRect.bottom - indentSize.cy);
	}

	inline void drawLine(HDC paintDC, const CoordSystem2D& xy, CoordSystem2D::ClientUnit x1, CoordSystem2D::ClientUnit y1, CoordSystem2D::ClientUnit x2, CoordSystem2D::ClientUnit y2) noexcept {
		auto s = xy.clientToScreen(x1, y1, 1);
		MoveToEx(paintDC, (int)s.x, (int)s.y, nullptr);

		s = xy.clientToScreen(x2, y2, 1);
		LineTo(paintDC, (int)s.x, (int)s.y);
	}

	inline void drawLine(HDC paintDC, const CoordSystem2D& xy, CoordSystem2D::ClientUnit x, CoordSystem2D::ClientUnit y) noexcept {
		const auto s = xy.clientToScreen(x, y, 1);

		LineTo(paintDC, (int)s.x, (int)s.y);
	}

	inline void drawClipRectWithDiagonals(HDC paintDC, const CoordSystem2D& xy, const CoordSystem2D::ClientRect& clientClipRect) noexcept {

		drawLine(paintDC, xy, clientClipRect.left, clientClipRect.top, clientClipRect.right, clientClipRect.top);
		drawLine(paintDC, xy, clientClipRect.right, clientClipRect.bottom);
		drawLine(paintDC, xy, clientClipRect.left, clientClipRect.bottom);

		drawLine(paintDC, xy, clientClipRect.left, clientClipRect.top);

		//Diagonals
		drawLine(paintDC, xy, clientClipRect.left, clientClipRect.top, clientClipRect.right, clientClipRect.bottom);
		drawLine(paintDC, xy, clientClipRect.right, clientClipRect.top, clientClipRect.left, clientClipRect.bottom);
	}

	//inline void drawLines(HDC paintDC, const CoordSystem2D& xy) noexcept {
	//	const auto clientRect = xy.getClientClipRect();

	//	const SIZE clientArea{ abs(clientRect.right - clientRect.left), abs(clientRect.bottom - clientRect.top) };
	//	const SIZE lineArea{ 2 * clientArea.cx / 3, 2 * clientArea.cy / 3 };
	//	const SIZE indentSize{ (clientArea.cx - lineArea.cx) / 2, (clientArea.cy - lineArea.cy) / 2 };

	//	xy.line(paintDC, clientRect.left, clientRect.top - indentSize.cy, clientRect.right, clientRect.top - indentSize.cy/*, RGB(0, 180, 0)*/);
	//	xy.line(paintDC, clientRect.left, clientRect.bottom + indentSize.cy, clientRect.right, clientRect.top - indentSize.cy/*, RGB(180, 0, 0)*/);
	//	xy.line(paintDC, clientRect.left + lineArea.cx, clientRect.top, clientRect.left + lineArea.cx, clientRect.bottom + indentSize.cy/*, RGB(0, 0, 180)*/);

	//	xy.line(paintDC, clientRect.left, clientRect.bottom + indentSize.cy, clientRect.right, clientRect.bottom + indentSize.cy/*, RGB(180, 180, 0)*/);
	//	xy.line(paintDC, clientRect.left, clientRect.top - indentSize.cy, clientRect.right, clientRect.bottom + indentSize.cy/*, RGB(180, 0, 180)*/);
	//	xy.line(paintDC, clientRect.right - lineArea.cx, clientRect.top, clientRect.right - lineArea.cx, clientRect.bottom + indentSize.cy/*, RGB(0, 180, 180)*/);
	//}

	//inline void drawTriangleInPlace(int originX, int originY, HDC hdc, const CoordSystem2D& xy, int x0, int y0, int x1, int y1, int x2, int y2, unsigned long colour) noexcept {
	//    mycg::fillTriangle(hdc, xy, );
	//}

	//inline void drawTriangles(HDC paintDC, const CoordSystem2D& xy) noexcept {
	//	const auto clientRect = xy.getClientClipRect();

	//	const auto rowCount = 2;
	//	const auto colCount = 5;

	//	const SIZE clientArea{ abs(clientRect.right - clientRect.left), abs(clientRect.bottom - clientRect.top) };
	//	const SIZE indentSize{ 10, 10 };

	//	const SIZE oneTriangleExampleArea{ clientArea.cx / colCount, clientArea.cy / rowCount };

	//	auto i = 0, j = 0;

	//	{
	//		// Degenerate triangle, all points have same y
	//		const auto sameY = i * oneTriangleExampleArea.cy + oneTriangleExampleArea.cy / 2;
	//		mycg::fillTriangle2(paintDC, xy, j * oneTriangleExampleArea.cx + oneTriangleExampleArea.cx - indentSize.cx, sameY,
	//			j * oneTriangleExampleArea.cx + indentSize.cx, sameY, j * oneTriangleExampleArea.cx + (oneTriangleExampleArea.cx - 2 * indentSize.cx) / 3, sameY, RGB(180, 0, 0));
	//	}

	//	i = 0; j = 1;
	//	{
	//		const auto sameX = j * oneTriangleExampleArea.cx + oneTriangleExampleArea.cx / 2;
	//		mycg::fillTriangle2(paintDC, xy, sameX, i * oneTriangleExampleArea.cy + oneTriangleExampleArea.cy - indentSize.cy,
	//			sameX, i * oneTriangleExampleArea.cy + indentSize.cy, sameX, i * oneTriangleExampleArea.cy + (oneTriangleExampleArea.cy - 2 * indentSize.cy) / 3, RGB(0, 120, 0));
	//	}

	//	i = 0; j = 2;
	//	{
	//		// left + indentX, areaMiddleY
	//		Vector2i a{ j * oneTriangleExampleArea.cx + indentSize.cx, i * oneTriangleExampleArea.cy + oneTriangleExampleArea.cy / 2 },
	//			b{ j * oneTriangleExampleArea.cx + oneTriangleExampleArea.cx / 3, i * oneTriangleExampleArea.cy + indentSize.cy },
	//			c{ j * oneTriangleExampleArea.cx + oneTriangleExampleArea.cx - indentSize.cx, i * oneTriangleExampleArea.cy + (oneTriangleExampleArea.cy - 2 * indentSize.cy) / 4 };

	//		mycg::fillTriangle2(paintDC, xy, a.x, a.y,
	//			// left + left / 3, bottomY + intenY
	//			b.x, b.y,
	//			c.x, c.y,
	//			RGB(0, 0, 160));
	//		//OutputDebugString(TEXT("Triangle 2"));
	//	}

	//	i = 0; j = 3;
	//	{
	//		// Almost degenerate triangle
	//		// left + indentX, areaMiddleY
	//		mycg::fillTriangle2(paintDC, xy, j * oneTriangleExampleArea.cx + indentSize.cx, i * oneTriangleExampleArea.cy + oneTriangleExampleArea.cy / 2,
	//			j * oneTriangleExampleArea.cx + indentSize.cx + 2, i * oneTriangleExampleArea.cy + oneTriangleExampleArea.cy / 2,
	//			j * oneTriangleExampleArea.cx + oneTriangleExampleArea.cx - indentSize.cx, i * oneTriangleExampleArea.cy + (oneTriangleExampleArea.cy - 2 * indentSize.cy) / 4,
	//			RGB(180, 120, 0));
	//	}

	//	i = 0; j = 4;
	//	{
	//		// left + indentX, top - indentY
	//		mycg::fillTriangle2(paintDC, xy, j * oneTriangleExampleArea.cx + indentSize.cx, i * oneTriangleExampleArea.cy + oneTriangleExampleArea.cy - indentSize.cy,
	//			// left + indentX, bottom + indentY
	//			j * oneTriangleExampleArea.cx + indentSize.cx, i * oneTriangleExampleArea.cy + indentSize.cy,
	//			// right - indentX, top - indentY
	//			j * oneTriangleExampleArea.cx + oneTriangleExampleArea.cx - indentSize.cx, i * oneTriangleExampleArea.cy + oneTriangleExampleArea.cy - indentSize.cy,
	//			RGB(180, 180, 0));
	//	}

	//	i = 1; j = 0;
	//	{
	//		// left + indentX, top - indentY
	//		mycg::fillTriangle2(paintDC, xy, j * oneTriangleExampleArea.cx + indentSize.cx, i * oneTriangleExampleArea.cy + oneTriangleExampleArea.cy - indentSize.cy,
	//			// left + indentX + width / 3, bottom + indentY
	//			j * oneTriangleExampleArea.cx + indentSize.cx + oneTriangleExampleArea.cx / 3, i * oneTriangleExampleArea.cy + indentSize.cy,
	//			// right - indentX, top - indentY
	//			j * oneTriangleExampleArea.cx + oneTriangleExampleArea.cx - indentSize.cx, i * oneTriangleExampleArea.cy + oneTriangleExampleArea.cy - indentSize.cy,
	//			RGB(180, 0, 180));
	//	}

	//	i = 1; j = 1;
	//	{
	//		// left + indentX + width / 2, top - indentY
	//		mycg::fillTriangle2(paintDC, xy, j * oneTriangleExampleArea.cx + oneTriangleExampleArea.cx / 2, i * oneTriangleExampleArea.cy + oneTriangleExampleArea.cy - indentSize.cy,
	//			// left + indentX, bottom + indentY
	//			j * oneTriangleExampleArea.cx + indentSize.cx, i * oneTriangleExampleArea.cy + indentSize.cy,
	//			// right - indentX, bottom + indentY + 2*height / 3
	//			j * oneTriangleExampleArea.cx + oneTriangleExampleArea.cx - indentSize.cx, i * oneTriangleExampleArea.cy + indentSize.cy + 2 * oneTriangleExampleArea.cy / 3,
	//			RGB(0, 180, 200));
	//	}

	//	i = 1; j = 2;
	//	{
	//		// left + indentX, bottom + indentY
	//		mycg::fillTriangle2(paintDC, xy, j * oneTriangleExampleArea.cx + indentSize.cx, i * oneTriangleExampleArea.cy + indentSize.cy,
	//			// right - indentX, bottom + indentY
	//			j * oneTriangleExampleArea.cx + oneTriangleExampleArea.cx - indentSize.cx, i * oneTriangleExampleArea.cy + indentSize.cy,
	//			// left + width / 2, top - indentY
	//			j * oneTriangleExampleArea.cx + oneTriangleExampleArea.cx / 2, i * oneTriangleExampleArea.cy + oneTriangleExampleArea.cy - indentSize.cy,
	//			RGB(90, 180, 90));
	//	}

	//	i = 1; j = 3;
	//	{
	//		// left + indentX, top - indentY
	//		mycg::fillTriangle2(paintDC, xy, j * oneTriangleExampleArea.cx + indentSize.cx, i * oneTriangleExampleArea.cy + oneTriangleExampleArea.cy - indentSize.cy,
	//			// left + width / 2 - indentX, bottom + height / 2 - indentY
	//			j * oneTriangleExampleArea.cx + oneTriangleExampleArea.cx / 2 - indentSize.cx, i * oneTriangleExampleArea.cy + oneTriangleExampleArea.cy / 2 - indentSize.cy,
	//			// right - indentX, bottom + indentY
	//			j * oneTriangleExampleArea.cx + oneTriangleExampleArea.cx - indentSize.cx, i * oneTriangleExampleArea.cy + indentSize.cy,
	//			RGB(0, 0, 180));
	//	}
	//}

	template<typename T>
	inline void drawWireModel(HDC paintDC, const CoordSystem2D& xy, const mycg::CGModel<T>& model) {
		//for (int i = 0; i < model->nfaces(); i++) {
		//    std::vector<int> face = model->face(i);
		//    for (int j = 0; j < 3; j++) {
		//        Vec3f v0 = model->vert(face[j]);
		//        Vec3f v1 = model->vert(face[(j + 1) % 3]);
		//        int x0 = (v0.x + 1.) * width / 2.;
		//        int y0 = (v0.y + 1.) * height / 2.;
		//        int x1 = (v1.x + 1.) * width / 2.;
		//        int y1 = (v1.y + 1.) * height / 2.;
		//        line(x0, y0, x1, y1, image, white);
		//    }
		//}

		//auto clientRect = xy.getClientClipRect();
		//auto clientHalfWidth = abs(clientRect.Width()) / 2.0f;
		//auto clientHalfHeight = abs(clientRect.Height()) / 2.0f;

		//std::for_each(std::execution::seq, model.faces().cbegin(), model.faces().cend(), [&model, &xy, paintDC, clientHalfWidth, clientHalfHeight](const auto& face) {
		for (const auto& face : model.faces()) {
			for (int i = 0; i < std::ssize(face) - 1; i++) {
				auto v0 = model.vertexAt(face[i]);
				auto v1 = model.vertexAt(face[i + 1]);

				//mycg::bresenhamLine2(paintDC, xy, x0, y0, x1, y1, RGB(0, 0, 0));
				drawLine(paintDC, xy, v0.x, v0.y, v1.x, v1.y);
			}

			if (!face.empty()) {
				// Handle last edge
				auto v0 = model.vertexAt(face[face.size() - 1]);
				auto v1 = model.vertexAt(face[0]);

				//mycg::bresenhamLine2(paintDC, xy, x0, y0, x1, y1, RGB(0, 0, 0));
				drawLine(paintDC, xy, v0.x, v0.y, v1.x, v1.y);
			}
		}
	}

	template<typename T>
	inline void drawFilledModel(HDC paintDC, const CoordSystem2D& xy, const mycg::CGModel<T>& model, const mymtl::Vector3<T>& lightDirection, std::vector<int>& zbuffer, uint32_t zbufferWidth) {

		//std::for_each(std::execution::seq, model.faces().cbegin(), model.faces().cend(), [&model, &xy, paintDC, clientHalfWidth, clientHalfHeight](const auto& face) {
		for (const auto& face : model.faces()) {
			const auto vertexCount = face.size();
			if (vertexCount <= 2) {
				// Skip degenerate faces
				continue;
			}

			std::vector<typename mycg::CGModel<T>::VertexType> worldVertices{ vertexCount };
			std::vector<mymtl::Vector3< CoordSystem2D::ScreenUnit >> screenVertices{ vertexCount };
			for (std::remove_cv_t<decltype(vertexCount)> i = 0; i < vertexCount; i++) {
				const auto v = model.vertexAt(face[i]);
				worldVertices[i] = v;
				const auto [sx, sy, _unused] = xy.clientToScreen(v.x, v.y, 1);
				screenVertices[i].x = static_cast<CoordSystem2D::ScreenUnit>(sx);
				screenVertices[i].y = static_cast<CoordSystem2D::ScreenUnit>(sy);
				// Conversion z for artificial screen depth for z-buffer
				screenVertices[i].z = static_cast<CoordSystem2D::ScreenUnit>((v.z + 1.0) * 255 / 2.0);
			}

			std::remove_cvref_t<decltype(worldVertices[0])> faceNormal;
			mymtl::vector_normalize(mymtl::vector_cross(worldVertices[2] - worldVertices[0], worldVertices[1] - worldVertices[0]), faceNormal);

			// z is positive, from screen, i.e. z = cross(x, y), where x grows left to right, y grows from bottom to top
			const auto colourIntensity = mymtl::vector_dot(lightDirection, faceNormal);
			if (colourIntensity > 0) {
				mycg::fillTriangle(paintDC, screenVertices[0].x, screenVertices[0].y, screenVertices[0].z,
					screenVertices[1].x, screenVertices[1].y, screenVertices[1].z,
					screenVertices[2].x, screenVertices[2].y, screenVertices[2].z, computeColourGreyScale(colourIntensity, 255), zbuffer, zbufferWidth);
			}
		}
	}
}

MainView::MainView(HWND window) noexcept
	: frameWnd{ window }, ribbon{ nullptr }, ribbonHeight{ 0 },
	x0y{ CoordSystem2D::ClientSpace{ {1, -1}, {-1, -1}, {-1, 1} }, CoordSystem2D::ScreenSpace{ {1, 0}, {0, 0}, {1, 1} } },
	modelZBuffer{ ZBuffer{}, 0, 0 }
{}

MainView::~MainView() noexcept {
	frameWnd = nullptr;
}

HRESULT MainView::onViewChanged(UINT viewId,
	UI_VIEWTYPE typeId, IUnknown* pView, UI_VIEWVERB verb, INT uReasonCode) noexcept {

	UNREFERENCED_PARAMETER(uReasonCode);
	UNREFERENCED_PARAMETER(viewId);

	HRESULT hr = E_NOTIMPL;

	// Checks to see if the view that was changed was a Ribbon view.
	if (UI_VIEWTYPE_RIBBON == typeId) {
		switch (verb) {
			// The view was newly created.
		case UI_VIEWVERB_CREATE: {
			hr = pView->QueryInterface(IID_PPV_ARGS(&ribbon));
			break;
		}

							   // The view has been resized.  For the Ribbon view, the application should 
							   // call GetHeight to determine the height of the ribbon.
		case UI_VIEWVERB_SIZE:
		{
			// Call to the framework to determine the desired height of the Ribbon.
			hr = ribbon->GetHeight(&ribbonHeight);
			// Use the ribbon height to position controls in the client area of the window.

			invalidateScene();

			break;
		}
		// The view was destroyed.
		case UI_VIEWVERB_DESTROY: {
			releaseRibbonView();
			hr = S_OK;
			break;
		}
		}
	}

	return hr;
}

void MainView::onSizeChanged(uint32_t width, uint32_t height) noexcept {
	UNREFERENCED_PARAMETER(width);
	UNREFERENCED_PARAMETER(height);

	//invalidateScene();
}

void MainView::invalidate() noexcept {
	invalidateScene();
}

void MainView::setModel(MainView::VisualModel&& model, bool computeBoundingBox) {
	if (computeBoundingBox) {
		mymtl::BoundingBox3<VisualModel::CoordinateType> box;
		mycg::computeBoundingBox(model.vertices(), box);
		x0y.setClientSpace(CoordSystem2D::ClientSpace{ {box.sb.left_, box.sb.top_}, {box.sb.left_, box.sb.bottom_}, {box.sb.right_, box.sb.bottom_} }, true);
	}
	displayedModel = std::make_unique<VisualModel>(std::move(model));
}

void MainView::setModel(const MainView::VisualModel& model, bool computeBoundingBox) {
	if (computeBoundingBox) {
		mymtl::BoundingBox3<VisualModel::CoordinateType> box;
		mycg::computeBoundingBox(model.vertices(), box);
		x0y.setClientSpace(CoordSystem2D::ClientSpace{ {box.sb.left_, box.sb.top_}, {box.sb.left_, box.sb.bottom_}, {box.sb.right_, box.sb.bottom_} }, true);
	}
	displayedModel = std::make_unique<VisualModel>(model);
}

void MainView::clearScene() {
	displayedModel.release();
	std::get<1>(modelZBuffer) = 0;
	std::get<2>(modelZBuffer) = 0;
	std::get<ZBuffer>(modelZBuffer).clear();
}

void MainView::onDraw(HDC hdc) noexcept {
	RECT r;
	GetClipBox(hdc, &r);

	//SetGraphicsMode(hdc, GM_ADVANCED);
	//SetMapMode(hdc, MM_ANISOTROPIC);

	//XFORM xForm{ 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, (r.bottom - r.top) / 2.0f };
	// SetWorldTransform(hdc, &xForm);

	MemoryDC memDC{ hdc, r };
	memDC.fillBackgroundSolidColour(GetBkColor(hdc));

	drawScene(memDC.getDC());
	memDC.bitBlt(hdc);
}

void MainView::releaseRibbonView() noexcept {
	if (ribbon) {
		ribbon->Release();
		ribbon = nullptr;
	}
}

void MainView::drawScene(HDC paintDC) noexcept {
	//drawEllipse(paintDC, clientRect);
	if (displayedModel) {
		drawWireModel(paintDC, x0y, *displayedModel.get());
	}
	else {
		//drawLines(paintDC, x0y);
		//drawTriangles(paintDC, x0y);

		//CoordSystem2D::ClientSpace testSpace{ { -0.662479997f, 1.0f }, { -0.662479997f, -1.0f }, { 0.662479997f, -1 } };
		//CoordSystem2D testCoordSystem{ 
		//	CoordSystem2D::TransformationMatrix{ 
		//		454.35334124487048, 0.0, 0.0, 
		//		0.0, -414.0, 0.0,
		//		321.0, 581.0, 1.0 
		//	}, 
		//testSpace};

		//[[maybe_unused]] auto i = testCoordSystem.getTramsformationMatrix().size() + 4;
		//x0y.setClientSpace(CoordSystem2D::ClientSpace{pt0, pt1, pt2}).updateTransformation();

		//drawClipRectWithDiagonals(paintDC, x0y, CoordSystem2D::ClientRect{pt0.x, pt0.y, pt2.x, pt2.y});
	}
}

void MainView::invalidateScene() noexcept {
	RECT r;
	GetClientRect(frameWnd, &r);
	r.top += ribbonHeight;

	const auto cx = static_cast<CoordSystem2D::ScreenUnit>(r.right - r.left), cy = static_cast<CoordSystem2D::ScreenUnit>(r.bottom - r.top);

	if (displayedModel) {
		resizeZBufferIfNeeded(abs(r.right), abs(r.bottom));
	}

	const auto indentX = computePadding(cx, 20);
	const auto indentY = computePadding(cy, 20);

	x0y.setScreenSpace(CoordSystem2D::ScreenSpace{ {r.left + indentX, r.top + indentY}, {r.left + indentX, r.top + cy - indentY}, {r.left + cx - indentX, r.top + cy - indentY} }, true);

	InvalidateRect(frameWnd, &r, false);
}

void MainView::resizeZBufferIfNeeded(uint32_t width, uint32_t height) noexcept {
	const size_t newSize = width * height;
	auto& buffer = std::get<ZBuffer>(modelZBuffer);
	if (buffer.size() < newSize || (buffer.size() - newSize) > newSize / 3) {
		buffer.resize(newSize);
		std::get<1>(modelZBuffer) = width;
		std::get<2>(modelZBuffer) = height;
	}

	for (auto& z : buffer) {
		z = (std::numeric_limits<std::remove_cvref_t<decltype(buffer)>::value_type>::min)();
	}
}
