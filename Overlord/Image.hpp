#pragma once
#include "BitmapGdi.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class Image : public BitmapGdi {
	Image(int width, int height, HDC dc, HBITMAP bitmap) :
		BitmapGdi{width, height, dc, bitmap} {}

public:
	struct Pixel {
		BYTE b, g, r, a;
	};

	virtual ~Image();
	Image(const Image&) = delete;
	Image(const Image&&) = delete;
	Image& operator=(const Image&) = delete;
	Image& operator=(const Image&&) = delete;
	size_t GetPixels(Pixel*& bits) const;
	size_t GetPixels(Pixel*& begin, Pixel*& end) const;
	static void FreePixels(void* pixels_data);

	static Image* CreateBlank(int width, int height, COLORREF bgColor);
	static Image* Capture(HWND hWnd, const RECT* captureRect);
	static Image* CaptureDesktop();
	static Image* CreateFromPixels(Pixel* bits, int width, int height);
	static Image* LoadFile(const char* filename);
	static void FreeImage(Image* image);

	void SaveFile(const char* filename);
};
