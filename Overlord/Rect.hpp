#pragma once
#include "Point.hpp"
#include "Size.hpp"

struct Rect {
	int x, y, w, h;

	Rect& Move(Point p)
	{
		this->x = p.x;
		this->y = p.y;
		return *this;
	}

	Rect& Offset(Point o)
	{
		this->x += o.x;
		this->y += o.y;
		return *this;
	}

	Rect& Resize(Size size)
	{
		auto rw = (size.w - w) >> 1;
		auto rh = (size.h - h) >> 1;
		x -= rw;
		y -= rh;
		w += rw;
		h += rh;
		return *this;
	}

	Rect& Zoom(float factor)
	{
		int fx = int(factor / 2 * x);
		int fy = int(factor / 2 * y);
		x -= fx;
		y -= fy;
		w += fx;
		h += fy;
		return *this;
	}
};
