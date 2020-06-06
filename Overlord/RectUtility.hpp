#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

inline POINT RectToPoint(const RECT& rect) { return POINT{rect.left, rect.top}; }
inline SIZE RectToSize(const RECT& rect) { return SIZE{rect.right - rect.left, rect.bottom - rect.top}; }
