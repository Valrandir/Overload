#include "GdiBrush.hpp"

GdiBrush::GdiBrush(HDC hdc, COLORREF color)
{
	this->hdc = hdc;
	brush = CreateSolidBrush(color);
	prev_brush = static_cast<HBRUSH>(SelectObject(hdc, brush));
}

GdiBrush::~GdiBrush()
{
	if(hdc) {
		SelectObject(hdc, prev_brush);
		DeleteObject(brush);
		hdc = 0;
		brush = prev_brush = 0;
	}
}
