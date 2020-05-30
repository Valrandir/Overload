#include "ImageCapture.hpp"

BitmapGdi ImageCapture::CaptureWindow(HWND window)
{
	RECT rect;

	if(window == HWND_DESKTOP) {
		rect.left = 0;
		rect.top = 0;
		rect.right = GetSystemMetrics(SM_CXSCREEN);
		rect.bottom = GetSystemMetrics(SM_CYSCREEN);
	} else
		GetWindowRect(window, &rect);

	return CaptureWindow(window, rect);
}

BitmapGdi ImageCapture::CaptureWindow(HWND window, const RECT& capture_rect)
{
	int x = capture_rect.left;
	int y = capture_rect.top;
	int width = capture_rect.right - x;
	int height = capture_rect.bottom - y;

	BitmapGdi bitmap_gdi(width, height);
	HDC source_dc = GetDC(window);
	bitmap_gdi.Draw(source_dc, 0, 0, width, height, x, y);
	ReleaseDC(window, source_dc);

	return bitmap_gdi;
}

BitmapGdi ImageCapture::CaptureDesktop()
{
	return CaptureWindow(HWND_DESKTOP);
}

BitmapGdi ImageCapture::CaptureDesktop(const RECT& capture_rect)
{
	return CaptureWindow(HWND_DESKTOP, capture_rect);
}
