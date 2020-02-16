#include "Image.hpp"

static void GetCaptureZone(HWND hWnd, const RECT* rect, int& x, int& y, int& width, int& height)
{
	RECT r;

	if(rect)
		r = *rect;
	else
		GetWindowRect(hWnd, &r);

	x = r.left;
	y = r.top;
	width = r.right - x;
	height = r.bottom - y;
}

static void Fill(HDC hDC, COLORREF color, int width, int height)
{
	RECT rect{0, 0, width, height};
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

Image* Image::Capture(HWND hWnd, const RECT* captureRect)
{
	int x, y, width, height;
	GetCaptureZone(hWnd, captureRect, x, y, width, height);

	HDC hSourceDC = ::GetDC(hWnd);
	HDC hDC = CreateCompatibleDC(hSourceDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hSourceDC, width, height);
	SelectObject(hDC, hBitmap);
	BitBlt(hDC, 0, 0, width, height, hSourceDC, x, y, SRCCOPY);
	ReleaseDC(hWnd, hSourceDC);

	return new Image(width, height, hDC, hBitmap);
}

Image* Image::CaptureDesktop()
{
	RECT rect{0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};
	return Capture(HWND_DESKTOP, &rect);
}

static void LoadImageFromFile(const char* filename, int& width, int& height, HDC& hDC, HBITMAP& hBitmap);
static void SaveImageToFile(const char* filename, HDC hDC, HBITMAP hBitmap);

Image* Image::LoadFile(const char* filename)
{
	int width, height;
	HDC hDC;
	HBITMAP hBitmap;

	LoadImageFromFile(filename, width, height, hDC, hBitmap);
	return new Image(width, height, hDC, hBitmap);
}

void Image::SaveToFile(const char* filename)
{
	SaveImageToFile(filename, _hDC, _hBitmap);
}

#include "lib/lodepng/lodepng.h"
static void SwapColors(unsigned char* bits, size_t size);

static void LoadImageFromFile(const char* filename, int& width, int& height, HDC& hDC, HBITMAP& hBitmap)
{
	unsigned char* image = 0;
	unsigned w, h;

	//Decode png from file
	lodepng_decode32_file(&image, &w, &h, filename);
	width = (int)w;
	height = (int)h;

	//Swap RGBA to BGRA
	SwapColors(image, width * height * 4);

	//Create bitmap
	HDC hDesktopDC = GetDC(HWND_DESKTOP);
	hDC = CreateCompatibleDC(hDesktopDC);
	hBitmap = CreateCompatibleBitmap(hDesktopDC, width, height);
	SelectObject(hDC, hBitmap);
	ReleaseDC(HWND_DESKTOP, hDesktopDC);

	//Set bits
	BITMAPINFO bi{};
	BITMAPINFOHEADER& bih = bi.bmiHeader;
	bih.biSize = sizeof(bih);
	bih.biPlanes = 1;
	bih.biCompression = BI_RGB;
	bih.biBitCount = 32;
	bih.biWidth = width;
	bih.biHeight = -height;
	SetDIBits(hDC, hBitmap, 0, height, image, &bi, DIB_RGB_COLORS);

	free(image);
}

static void SaveImageToFile(const char* filename, HDC hDC, HBITMAP hBitmap)
{
	BITMAPINFO bi{};
	BITMAPINFOHEADER& bih = bi.bmiHeader;
	bih.biSize = sizeof(bih);

	//Initialize BITMAPINFO
	GetDIBits(hDC, hBitmap, 0, 0, nullptr, &bi, DIB_RGB_COLORS);
	bih.biCompression = BI_RGB;
	bih.biBitCount = 32;
	bih.biHeight *= -1;

	//Get bits
	unsigned char* bits = new unsigned char[bih.biSizeImage];
	GetDIBits(hDC, hBitmap, 0, bih.biHeight, bits, &bi, DIB_RGB_COLORS);

	//Swap BGRA to RGBA
	SwapColors(bits, bih.biSizeImage);

	//Encore png to file
	lodepng_encode32_file(filename, bits, bih.biWidth, -bih.biHeight);

	delete[] bits;
}

//Swap R and B in RGBA
static void SwapColors(unsigned char* bits, size_t size)
{
	for(auto it = bits, end = it + size; it < end; it += 4) {
		auto s = *it;
		it[0] = it[2];
		it[2] = s;
	}
}
