#include "BitmapGdi.hpp"

BitmapGdi::BitmapGdi(BitmapGdi&& other) noexcept
{
	Move(other);
}

BitmapGdi& BitmapGdi::operator=(BitmapGdi&& other) noexcept
{
	if(this == &other)
		return *this;

	Destroy();
	return Move(other);
}

BitmapGdi::BitmapGdi(int width, int height) :
	width{width}, height{height}
{
	HDC hDesktopDC = ::GetDC(HWND_DESKTOP);
	dc = CreateCompatibleDC(hDesktopDC);
	bitmap = CreateCompatibleBitmap(hDesktopDC, width, height);
	SelectObject(dc, bitmap);
	ReleaseDC(HWND_DESKTOP, hDesktopDC);
}

BitmapGdi::~BitmapGdi() { Destroy(); }

void BitmapGdi::Fill(const RECT& rect, COLORREF color)
{
	HBRUSH brush = CreateSolidBrush(color);
	::FillRect(dc, &rect, brush);
	DeleteObject(brush);
}

void BitmapGdi::Fill(int x, int y, int w, int h, COLORREF color)
{
	Fill({x, y, x + w, y + h}, color);
}

inline BitmapGdi& BitmapGdi::Move(BitmapGdi& other) noexcept
{
	width = other.width;
	height = other.height;
	dc = other.dc;
	bitmap = other.bitmap;

	other.width = 0;
	other.height = 0;
	other.dc = 0;
	other.bitmap = 0;

	return *this;
}

inline void BitmapGdi::Destroy() noexcept
{
	if(dc) {
		DeleteDC(dc);
		dc = nullptr;
	}

	if(bitmap) {
		DeleteObject(bitmap);
		bitmap = nullptr;
	}

	width = 0;
	height = 0;
}
