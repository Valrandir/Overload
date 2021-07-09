#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class GdiBrush {
	HDC hdc;
	HBRUSH brush, prev_brush;

public:
	GdiBrush(const GdiBrush&) = delete;
	GdiBrush(HDC hdc, COLORREF color);
	~GdiBrush();
};
