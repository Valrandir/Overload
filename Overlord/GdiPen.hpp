#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class GdiPen {
	HDC hdc;
	HPEN pen, prev_pen;

public:
	GdiPen(const GdiPen&) = delete;
	GdiPen(HDC hdc, COLORREF color);
	~GdiPen();
};
