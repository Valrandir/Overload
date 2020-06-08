#include "AreaView.hpp"

AreaView::AreaView(Size area, Size view)
{
	this->area = area,
	this->view = Rect{0, 0, view.w, view.h};
	this->zoomed_view = this->view;
	CenterView();
}

void AreaView::CenterView()
{
	view.x = (area.w - view.w) >> 1;
	view.y = (area.h - view.h) >> 1;
	ApplyZoom();
}

void AreaView::ZoomToArea()
{
	auto zx = view.w / area.w;
	auto zy = view.h / area.h;
	Zoom(zx < zy ? zx : zy);
}

void AreaView::Zoom(float factor)
{
	zoom = factor;

	if(factor == 0.f) //Use Epsilon?
		zoomed_view = view;
	else {
		factor /= 2;
		zoomed_view.x = view.x * factor;
		zoomed_view.y = view.y * factor;
		zoomed_view.w = view.w * factor;
		zoomed_view.h = view.h * factor;
	}
}
