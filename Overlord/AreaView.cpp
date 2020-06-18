#include "AreaView.hpp"
#include <algorithm>

static void ClampOffsetRectToSize(const Rect& rect, const Size& size, int& x, int& y);

AreaView::AreaView(Size area, Size view)
{
	this->area = area,
	this->view = view;
	camera = {0, 0, view.w, view.h};
	camera_nozoom = {camera.w, camera.h};
}

void AreaView::OffsetCameraPosition(int x, int y)
{
	ClampCamera(x, y);
	camera.Offset({x, y});
}

void AreaView::OffsetCameraZoom(float zoom_offset)
{
	zoom = std::clamp(zoom + zoom_offset, 0.f, 4.f);

	const float EPSILON = 0.01f;
	if(abs(1 - zoom) < EPSILON) {
		zoom = 1.f;
		camera.Resize(camera_nozoom);
	} else
		camera.Zoom(zoom_offset);
}

void AreaView::ClampCamera(int& x, int& y)
{
	ClampOffsetRectToSize(camera, area, x, y);
}

void ClampOffsetRectToSize(const Rect& rect, const Size& size, int& x, int& y)
{
	if(rect.x + x < 0)
		x = rect.x - x;

	if(rect.y + y < 0)
		y = rect.y - y;

	if(rect.x + rect.w + x > size.w)
		x = size.w - (rect.x + rect.w);

	if(rect.y + rect.h + y > size.h)
		y = size.h - (rect.y + rect.h);
}
