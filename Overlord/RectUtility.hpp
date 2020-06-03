#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void RectToPoint(const RECT& rect, int& x, int& y, int& w, int& h);
