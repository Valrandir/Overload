#pragma once

struct Rect {
	int x, y, w, h;

	Rect& Move(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	Rect& Offset(int x, int y)
	{
		this->x += x;
		this->y += y;
	}

	Rect& Zoom(float factor)
	{
		int fx = int(factor / 2 * x);
		int fy = int(factor / 2 * y);
		x -= fx;
		y -= fy;
		w += fx;
		h += fy;
	}
};
