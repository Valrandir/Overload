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
		auto rw = (size.w - w) / 2;
		auto rh = (size.h - h) / 2;
		x -= rw;
		y -= rh;
		w = size.w;
		h = size.h;
		return *this;
	}

	Rect& Zoom(float factor)
	{
		--factor;
		int fx = int(factor * w) / 2;
		int fy = int(factor * h) / 2;
		x -= fx;
		y -= fy;
		w += fx;
		h += fy;
		return *this;
	}
};
