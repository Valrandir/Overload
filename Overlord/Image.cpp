#include "Image.hpp"
#include "ImageIO.hpp"
#include <exception>

static void GetCaptureZone(HWND hWnd, const RECT* rect, int& x, int& y, int& width, int& height);
static BYTE* BitsFromBitmap(HDC dc, HBITMAP bitmap);
static void BitmapFromBits(BYTE* bits, int width, int height, HDC& dc, HBITMAP& bitmap);
static void SwapRedAndBlue(Image::Pixel* bits, int width, int height);
static void SetMaxAlpha(Image::Pixel* bits, int width, int height);

Image::~Image()
{
	if(dc) {
		DeleteDC(dc);
		dc = nullptr;
	}

	if(bitmap) {
		DeleteObject(bitmap);
		bitmap = nullptr;
	}
}

size_t Image::GetPixels(Pixel*& bits) const
{
	auto pixel_count = (size_t)width * height;
	bits = (Pixel*)BitsFromBitmap(dc, bitmap);
	return pixel_count;
}

size_t Image::GetPixels(Pixel*& begin, Pixel*& end) const
{
	auto pixel_count = GetPixels(begin);
	end = begin + pixel_count;
	return pixel_count;
}

void Image::FreePixels(void* pixels_data)
{
	free(pixels_data);
}

Image* Image::CreateBlank(int width, int height, COLORREF bgColor)
{
	HDC desktop_dc = ::GetDC(HWND_DESKTOP);
	HDC dc = CreateCompatibleDC(desktop_dc);
	HBITMAP bitmap = CreateCompatibleBitmap(desktop_dc, width, height);
	ReleaseDC(HWND_DESKTOP, desktop_dc);
	SelectObject(dc, bitmap);

	RECT rect{0, 0, width, height};
	HBRUSH hBrush = CreateSolidBrush(bgColor);
	::FillRect(dc, &rect, hBrush);
	DeleteObject(hBrush);

	return new Image(width, height, dc, bitmap);
}

Image* Image::Capture(HWND hWnd, const RECT* captureRect)
{
	int x, y, width, height;
	GetCaptureZone(hWnd, captureRect, x, y, width, height);

	HDC source_dc = ::GetDC(hWnd);
	HDC dc = CreateCompatibleDC(source_dc);
	HBITMAP bitmap = CreateCompatibleBitmap(source_dc, width, height);
	SelectObject(dc, bitmap);
	BitBlt(dc, 0, 0, width, height, source_dc, x, y, SRCCOPY);
	ReleaseDC(hWnd, source_dc);

	return new Image(width, height, dc, bitmap);
}

Image* Image::CaptureDesktop()
{
	RECT rect{0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};
	return Capture(HWND_DESKTOP, &rect);
}

Image* Image::CreateFromPixels(Pixel* bits, int width, int height)
{
	HDC dc;
	HBITMAP bitmap;
	BitmapFromBits((BYTE*)bits, width, height, dc, bitmap);
	return new Image(width, height, dc, bitmap);
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

void Image::FreeImage(Image* image)
{
	delete image;
}

void Image::SaveFile(const char* filename)
{
	Pixel* bits;
	GetPixels(bits);
	SwapRedAndBlue(bits, width, height);
	SetMaxAlpha(bits, width, height);
	int saved = ImageIO::SaveFile(filename, width, height, (unsigned char*)bits);
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

static BYTE* BitsFromBitmap(HDC dc, HBITMAP bitmap)
{
	BITMAPINFO bi{};
	BITMAPINFOHEADER& bih = bi.bmiHeader;
	bih.biSize = sizeof(bih);
	GetDIBits(dc, bitmap, 0, 0, nullptr, &bi, 0);

	auto height = bih.biHeight;
	bih.biHeight = -height;
	bih.biBitCount = 32;
	bih.biCompression = BI_RGB;
	BYTE* bits = (BYTE*)malloc(bih.biSizeImage);
	GetDIBits(dc, bitmap, 0, height, bits, &bi, DIB_RGB_COLORS);

	return bits;
}

static void BitmapFromBits(BYTE* bits, int width, int height, HDC& dc, HBITMAP& bitmap)
{
	//Create bitmap
	HDC hDesktopDC = GetDC(HWND_DESKTOP);
	dc = CreateCompatibleDC(hDesktopDC);
	bitmap = CreateCompatibleBitmap(hDesktopDC, width, height);
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
	SetDIBits(dc, bitmap, 0, height, bits, &bi, DIB_RGB_COLORS);

	SelectObject(dc, bitmap);
}
