#include "BitmapGdi.hpp"
#include <cassert>

#define INITIALIZE_CHECK assert(dc);

BitmapGdi::BitmapGdi() :
	width{0}, height{0}, dc{0}, bitmap{0} {}

BitmapGdi::BitmapGdi(int width, int height)
{
	Initialize(width, height);
}

BitmapGdi::BitmapGdi(int width, int height, HDC dc, HBITMAP bitmap) :
	width{width}, height{height}, dc{dc}, bitmap{bitmap} {}

void BitmapGdi::Initialize(int width, int height)
{
	Destroy();

	this->width = width;
	this->height = height;

	HDC hDesktopDC = ::GetDC(HWND_DESKTOP);
	dc = CreateCompatibleDC(hDesktopDC);
	bitmap = CreateCompatibleBitmap(hDesktopDC, width, height);
	SelectObject(dc, bitmap);
	ReleaseDC(HWND_DESKTOP, hDesktopDC);
}

BitmapGdi::BitmapGdi(BitmapGdi&& other) noexcept { Move(other); }

BitmapGdi& BitmapGdi::operator=(BitmapGdi&& other) noexcept
{
	if(this == &other)
		return *this;

	Destroy();
	return Move(other);
}

BitmapGdi::~BitmapGdi() { Destroy(); }

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

void BitmapGdi::Fill(COLORREF color)
{
	INITIALIZE_CHECK
	Fill({0, 0, width, height}, color);
}

void BitmapGdi::Fill(const RECT& rect, COLORREF color)
{
	INITIALIZE_CHECK
	HBRUSH brush = CreateSolidBrush(color);
	::FillRect(dc, &rect, brush);
	DeleteObject(brush);
}

void BitmapGdi::Fill(int x, int y, int w, int h, COLORREF color)
{
	INITIALIZE_CHECK
	Fill({x, y, x + w, y + h}, color);
}

void BitmapGdi::Draw(HDC source, int x, int y, int w, int h)
{
	INITIALIZE_CHECK
	BitBlt(dc, x, y, w, h, source, 0, 0, SRCCOPY);
}
