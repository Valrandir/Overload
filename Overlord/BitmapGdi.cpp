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
	_width{width}, _height{height}
{
	HDC hDesktopDC = ::GetDC(HWND_DESKTOP);
	_hDC = CreateCompatibleDC(hDesktopDC);
	_hBitmap = CreateCompatibleBitmap(hDesktopDC, width, height);
	SelectObject(_hDC, _hBitmap);
	ReleaseDC(HWND_DESKTOP, hDesktopDC);
}

BitmapGdi::~BitmapGdi() { Destroy(); }

void BitmapGdi::Fill(const RECT& rect, COLORREF color)
{
	HBRUSH brush = CreateSolidBrush(color);
	::FillRect(_hDC, &rect, brush);
	DeleteObject(brush);
}

void BitmapGdi::Fill(int x, int y, int w, int h, COLORREF color)
{
	Fill({x, y, x + w, y + h}, color);
}

inline BitmapGdi& BitmapGdi::Move(BitmapGdi& other) noexcept
{
	_width = other._width;
	_height = other._height;
	_hDC = other._hDC;
	_hBitmap = other._hBitmap;

	other._width = 0;
	other._height = 0;
	other._hDC = 0;
	other._hBitmap = 0;

	return *this;
}

inline void BitmapGdi::Destroy() noexcept
{
	if(_hDC) {
		DeleteDC(_hDC);
		_hDC = nullptr;
	}

	if(_hBitmap) {
		DeleteObject(_hBitmap);
		_hBitmap = nullptr;
	}

	_width = 0;
	_height = 0;
}
