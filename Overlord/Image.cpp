#include "Image.hpp"
#include "ImageIO.hpp"
#include <exception>

static void GetCaptureZone(HWND hWnd, const RECT* rect, int& x, int& y, int& width, int& height);
static BYTE* BitsFromBitmap(HDC hDC, HBITMAP hBitmap);
static void BitmapFromBits(BYTE* bits, int width, int height, HDC& hDC, HBITMAP& hBitmap);
static void SwapRedAndBlue(Image::Pixel* bits, int width, int height);
static void SetMaxAlpha(Image::Pixel* bits, int width, int height);

Image::~Image()
{
	if(_hDC) {
		DeleteDC(_hDC);
		_hDC = nullptr;
	}

	if(_hBitmap) {
		DeleteObject(_hBitmap);
		_hBitmap = nullptr;
	}
}

size_t Image::GetPixels(Pixel*& bits)
{
	auto pixel_count = (size_t)_width * _height;
	bits = (Pixel*)BitsFromBitmap(_hDC, _hBitmap);
	return pixel_count;
}

size_t Image::GetPixels(Pixel*& begin, Pixel*& end)
{
	auto pixel_count = GetPixels(begin);
	end = begin + pixel_count;
	return pixel_count;
}

void Image::FreePixels(void* pixels_data)
{
	free(pixels_data);
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

Image* Image::CreateFromPixels(Pixel* bits, int width, int height)
{
	HDC hDC;
	HBITMAP hBitmap;
	BitmapFromBits((BYTE*)bits, width, height, hDC, hBitmap);
	return new Image(width, height, hDC, hBitmap);
}

Image* Image::LoadFile(const char* filename)
{
	int width, height;

	auto bits = (Pixel*)ImageIO::LoadFile(filename, width, height);
	if(!bits)
		throw std::exception("ImageIO::LoadFile failed");

	SwapRedAndBlue(bits, width, height);
	auto image = CreateFromPixels(bits, width, height);
	ImageIO::FreeBits(bits);

	return image;
}

void Image::SaveFile(const char* filename)
{
	Pixel* bits;
	GetPixels(bits);
	SwapRedAndBlue(bits, _width, _height);
	SetMaxAlpha(bits, _width, _height);
	int saved = ImageIO::SaveFile(filename, _width, _height, (unsigned char*)bits);
	if(!saved)
		throw std::exception("stbi_write_png failed");

	FreePixels(bits);
}

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

static void SwapRedAndBlue(Image::Pixel* bits, int width, int height)
{
	auto end = bits + (size_t)width * height;
	for(auto it = bits; it < end; ++it) {
		auto r = it->r;
		it->r = it->b;
		it->b = r;
	}
}

static void SetMaxAlpha(Image::Pixel* bits, int width, int height)
{
	const BYTE MAX_ALPHA = 255;
	for(auto it = bits, end = bits + (size_t)width * height; it < end; ++it)
		it->a = MAX_ALPHA;
}

static BYTE* BitsFromBitmap(HDC hDC, HBITMAP hBitmap)
{
	BITMAPINFO bi{};
	BITMAPINFOHEADER& bih = bi.bmiHeader;
	bih.biSize = sizeof(bih);
	GetDIBits(hDC, hBitmap, 0, 0, nullptr, &bi, 0);

	bih.biHeight *= -1;
	bih.biBitCount = 32;
	bih.biCompression = BI_RGB;
	BYTE* bits = (BYTE*)malloc(bih.biSizeImage);
	GetDIBits(hDC, hBitmap, 0, bih.biHeight, bits, &bi, 0);

	return bits;
}

static void BitmapFromBits(BYTE* bits, int width, int height, HDC& hDC, HBITMAP& hBitmap)
{
	//Create bitmap
	HDC hDesktopDC = GetDC(HWND_DESKTOP);
	hDC = CreateCompatibleDC(hDesktopDC);
	hBitmap = CreateCompatibleBitmap(hDesktopDC, width, height);
	ReleaseDC(HWND_DESKTOP, hDesktopDC);

	//Set bits
	BITMAPINFO bi{};
	BITMAPINFOHEADER& bih = bi.bmiHeader;
	bih.biSize = sizeof(bih);
	bih.biWidth = width;
	bih.biHeight = -height;
	bih.biPlanes = 1;
	bih.biBitCount = 32;
	bih.biCompression = BI_RGB;
	SetDIBits(hDC, hBitmap, 0, height, bits, &bi, 0);

	SelectObject(hDC, hBitmap);
}
