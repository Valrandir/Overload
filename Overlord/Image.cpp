#include "Image.hpp"

void Fill(HDC hDC, COLORREF color, int width, int height)
{
	RECT rect{ 0, 0, width, height };
	HBRUSH hBrush = CreateSolidBrush(color);
	FillRect(hDC, &rect, hBrush);
	DeleteObject(hBrush);
}

Image::~Image()
{
	DeleteDC(_hDC);
	DeleteObject(_hBitmap);
}

Image* Image::CreateBlank(int width, int height, COLORREF bgColor)
{
	HDC hSourceDC = ::GetDC(HWND_DESKTOP);
	HDC hDC = CreateCompatibleDC(hSourceDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hSourceDC, width, height);
	ReleaseDC(HWND_DESKTOP, hSourceDC);
	SelectObject(hDC, hBitmap);

	Fill(hDC, bgColor, width, height);

	return new Image(width, height, hDC, hBitmap);
}

Image* Image::Capture(HWND hWnd, int x, int y, int width, int height)
{
	HDC hSourceDC = ::GetDC(hWnd);
	HDC hDC = CreateCompatibleDC(hSourceDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hSourceDC, width, height);
	SelectObject(hDC, hBitmap);
	BitBlt(hDC, 0, 0, width, height, hSourceDC, x, y, SRCCOPY);
	ReleaseDC(hWnd, hSourceDC);

	return new Image(width, height, hDC, hBitmap);
}
