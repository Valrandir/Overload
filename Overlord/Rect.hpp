#pragma once
#include "Point.hpp"
#include "Size.hpp"
#include <cmath>

struct Rect {
	int x, y, w, h;

	Rect& MoveTo(Point p)
	{
		this->x = p.x;
		this->y = p.y;
		return *this;
	}

	Rect& MoveBy(Point o)
	{
		this->x += o.x;
		this->y += o.y;
		return *this;
	}

	Rect& ResizeTo(Size new_size)
	{
		auto rw = (new_size.w - w) / 2;
		auto rh = (new_size.h - h) / 2;
		x -= rw;
		y -= rh;
		w = new_size.w;
		h = new_size.h;
		return *this;
	}

	Rect& ZoomTo(float factor)
	{
		--factor;
		auto fx = (int)std::roundf(w * factor);
		auto fy = (int)std::roundf(h * factor);

		x -= fx / 2;
		y -= fy / 2;
		w += fx;
		h += fy;

		return *this;
	}
};
