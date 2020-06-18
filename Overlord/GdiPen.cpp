#include "GdiPen.hpp"

GdiPen::GdiPen(HDC hdc, COLORREF color)
{
	this->hdc = hdc;
	pen = CreatePen(PS_SOLID, 0, color);
	prev_pen = static_cast<HPEN>(SelectObject(hdc, pen));
}

GdiPen::~GdiPen()
{
	if(hdc) {
		SelectObject(hdc, prev_pen);
		DeleteObject(pen);
		hdc = 0;
		pen = prev_pen = 0;
	}
}
