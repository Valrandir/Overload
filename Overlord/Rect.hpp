#pragma once
#include "Point.hpp"

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
