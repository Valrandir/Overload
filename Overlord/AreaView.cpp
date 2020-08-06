#include "AreaView.hpp"
#include <algorithm>

static void ClampOffsetRectToSize(Rect& rect, const Size& size);

AreaView::AreaView(Size area, Size view)
{
	this->area = area,
	this->view = view;
	camera = {0, 0, view.w, view.h};
	camera_nozoom = {view.w, view.h};
}

void AreaView::OffsetCameraPosition(int x, int y)
{
	camera.Offset({x, y});
	ClampOffsetRectToSize(camera, area);
}

void AreaView::OffsetCameraZoom(float zoom_offset)
{
	zoom = std::clamp(zoom + zoom_offset, .1f, 4.f);

	const float EPSILON = 0.01f;
	if(abs(1 - zoom) < EPSILON)
		zoom = 1.f;

	camera.Resize(camera_nozoom).Zoom(1 / zoom);
	ClampOffsetRectToSize(camera, area);
}

void ClampOffsetRectToSize(Rect& rect, const Size& size)
{
	int x{}, y{};

	if(rect.x < 0)
		x = -rect.x;
	else if(rect.x + rect.w > size.w)
		x = size.w - (rect.x + rect.w);

	if(rect.y < 0)
		y = -rect.y;
	else if(rect.y + rect.h + y > size.h)
		y = size.h - (rect.y + rect.h);

	rect.Offset({x, y});
}
