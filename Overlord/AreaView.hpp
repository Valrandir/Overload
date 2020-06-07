#pragma once
#include "Rect.hpp"
#include "Size.hpp"

class AreaView {
public:
	AreaView(Size area, Size view);

protected:
	Size area;
	Rect view;
};

AreaView::AreaView(Size area, Size view)
{
	this->area = area,
	this->view = Rect{0, 0, view.w, view.h};
}
