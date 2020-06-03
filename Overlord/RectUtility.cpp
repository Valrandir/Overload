#include "RectUtility.hpp"

void RectToPoint(const RECT& rect, int& x, int& y, int& w, int& h)
{
	x = rect.left;
	y = rect.top;
	w = rect.right - x;
	h = rect.bottom - y;
}
