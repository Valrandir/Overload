#include "AreaView.hpp"
#include <algorithm>

AreaView::AreaView(Size area, Size view)
{
	this->area = area,
	this->view = Rect{0, 0, view.w, view.h};
	this->zoomed_view = this->view;
	CenterView();
}

void AreaView::Offset(Point offset)
{
	view.Offset(offset);
	ApplyZoom();
}

void AreaView::Zoom(float factor)
{
	zoom = factor;

	if(factor == 1.f) //Use Epsilon?
		zoomed_view = view;
	else {
		factor /= 2;
		zoomed_view.x = int(view.x * factor);
		zoomed_view.y = int(view.y * factor);
		zoomed_view.w = int(view.w * factor);
		zoomed_view.h = int(view.h * factor);
	}
}

void AreaView::OffsetZoom(float offset_factor)
{
	Zoom(std::clamp(zoom += offset_factor, 0.f, 4.f));
}

void AreaView::CenterView()
{
	view.x = (area.w - view.w) >> 1;
	view.y = (area.w - view.h) >> 1;
	ApplyZoom();
}

void AreaView::ZoomToArea()
{
	auto zx = (float)view.w / area.w;
	auto zy = (float)view.h / area.h;
	Zoom(zx < zy ? zx : zy);
}

void AreaView::GetMinimapInfo(Rect& out_miniarea, Rect& out_miniview)
{
	int w = view.w / 10;
	int h = view.h / 10;
	int x = w / 2;
	int y = view.h - h - h / 2;
	out_miniarea = {x, y, w, h};

	float xf = (float)w / area.w;
	float yf = (float)h / area.h;
	int vx = int(zoomed_view.x * xf);
	int vy = int(zoomed_view.y * yf);
	int vw = int(zoomed_view.w * xf);
	int vh = int(zoomed_view.h * yf);
	out_miniview = {x + vx, y + vy, vw, vh};
}
