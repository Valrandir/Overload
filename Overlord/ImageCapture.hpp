#pragma once
#include "BitmapGdi.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace ImageCapture
{
	BitmapGdi CaptureWindow(HWND window);
	BitmapGdi CaptureWindow(HWND window, const RECT& capture_rect);
	BitmapGdi CaptureDesktop();
	BitmapGdi CaptureDesktop(const RECT& capture_rect);
};
