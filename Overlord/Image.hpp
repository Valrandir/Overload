#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class Image {
	int _width, _height;
	HDC _hDC;
	HBITMAP _hBitmap;

	Image(int width, int height, HDC hDC, HBITMAP hBitmap) :
		_width{width}, _height{height}, _hDC{hDC}, _hBitmap{hBitmap} {}

public:
	struct Pixel {
		BYTE b, g, r, a;
	};

	virtual ~Image();
	Image(const Image&) = delete;
	Image(const Image&&) = delete;
	Image& operator=(const Image&) = delete;
	Image& operator=(const Image&&) = delete;
	inline HDC GetDC() const { return _hDC; }
	inline int GetWidth() const { return _width; }
	inline int GetHeight() const { return _height; }
	size_t GetPixels(Pixel*& bits) const;
	size_t GetPixels(Pixel*& begin, Pixel*& end) const;
	static void FreePixels(void* pixels_data);

	void FillRect(const RECT& rect, COLORREF color);
	void FillRect(int x, int y, int w, int h, COLORREF color);

	static Image* CreateBlank(int width, int height, COLORREF bgColor);
	static Image* Capture(HWND hWnd, const RECT* captureRect);
	static Image* CaptureDesktop();
	static Image* CreateFromPixels(Pixel* bits, int width, int height);
	static Image* LoadFile(const char* filename);
	static void FreeImage(Image* image);

	void SaveFile(const char* filename);
};
