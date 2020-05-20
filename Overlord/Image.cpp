#include "Image.hpp"
#include "ImageIO.hpp"
#include <exception>

static BYTE* BitsFromBitmap(HDC hDC, HBITMAP hBitmap);
static void BitmapFromBits(BYTE* bits, int width, int height, HDC& hDC, HBITMAP& hBitmap);

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

Image::~Image()
{
	ClearCachedBits();

	if(_hDC) {
		DeleteDC(_hDC);
		_hDC = nullptr;
	}

	if(_hBitmap) {
		DeleteObject(_hBitmap);
		_hBitmap = nullptr;
	}
}

size_t Image::GetPixels(Pixel* first, Pixel* last)
{
	if(!_bits)
		_bits = BitsFromBitmap(_hDC, _hBitmap);

	size_t pixel_count = (size_t)_width * _height;

	first = (Pixel*)_bits;
	last = first + pixel_count;

	return pixel_count;
}

void Image::ClearCachedBits()
{
	if(_bits) {
		free(_bits);
		_bits = nullptr;
	}
}

void Image::FillRect(const RECT& rect, COLORREF color)
{
	HBRUSH brush = CreateSolidBrush(color);
	::FillRect(_hDC, &rect, brush);
	DeleteObject(brush);
}

void Image::FillRect(int x, int y, int w, int h, COLORREF color)
{
	FillRect({x, y, x + w, y + h}, color);
}

Image* Image::CreateBlank(int width, int height, COLORREF bgColor)
{
	HDC hSourceDC = ::GetDC(HWND_DESKTOP);
	HDC hDC = CreateCompatibleDC(hSourceDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hSourceDC, width, height);
	ReleaseDC(HWND_DESKTOP, hSourceDC);
	SelectObject(hDC, hBitmap);

	RECT rect{0, 0, width, height};
	HBRUSH hBrush = CreateSolidBrush(bgColor);
	::FillRect(hDC, &rect, hBrush);
	DeleteObject(hBrush);

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

Image* Image::LoadFile(const char* filename)
{
	int width, height;
	HDC hDC;
	HBITMAP hBitmap;

	auto bits = ImageIO::LoadFile(filename, width, height);
	if(!bits)
		throw std::exception("ImageIO::LoadFile failed");

	BitmapFromBits(bits, width, height, hDC, hBitmap);

	return new Image(width, height, hDC, hBitmap, bits);
}

void Image::SaveToFile(const char* filename)
{
	if(_bits == nullptr)
		_bits = BitsFromBitmap(_hDC, _hBitmap);

	int saved = ImageIO::SaveFile(filename, _width, _height, _bits);
	if(!saved)
		throw std::exception("stbi_write_png failed");
}

//Swap R and B in RGBA
static void SwapColors(BYTE* bits, size_t size)
{
	for(auto it = bits, end = it + size; it < end; it += 4) {
		auto s = *it;
		it[0] = it[2];
		it[2] = s;
		it[3] = 255; //Set alpha channel because GDI functions like FillRect clear it.
	}
}

static BYTE* BitsFromBitmap(HDC hDC, HBITMAP hBitmap)
{
	BITMAPINFO bi{};
	BITMAPINFOHEADER& bih = bi.bmiHeader;
	bih.biSize = sizeof(bih);
	GetDIBits(hDC, hBitmap, 0, 0, nullptr, &bi, DIB_RGB_COLORS);

	bih.biCompression = BI_RGB;
	bih.biBitCount = 32;
	bih.biHeight *= -1;
	BYTE* bits = (BYTE*)malloc(bih.biSizeImage);
	GetDIBits(hDC, hBitmap, 0, bih.biHeight, bits, &bi, DIB_RGB_COLORS);

	SwapColors(bits, bih.biSizeImage);

	return bits;
}

static void BitmapFromBits(BYTE* bits, int width, int height, HDC& hDC, HBITMAP& hBitmap)
{
	//Swap RGBA to BGRA
	SwapColors(bits, (size_t)width * height * 4);

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
	SetDIBits(hDC, hBitmap, 0, height, bits, &bi, DIB_RGB_COLORS);
}
