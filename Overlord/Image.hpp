#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class Image {
	int _width, _height;
	HDC _hDC;
	HBITMAP _hBitmap;
	BYTE* _bits;

	Image(int width, int height, HDC hDC, HBITMAP hBitmap, BYTE* bits = nullptr)
		: _width{width}, _height{height}, _hDC{hDC}, _hBitmap{hBitmap}, _bits{bits} {}

public:
	struct Pixel {
		BYTE r, g, b, a;
	};

	virtual ~Image();
	Image(const Image&) = delete;
	Image& operator=(const Image&) = delete;
	inline HDC GetDC() const { return _hDC; }
	inline int GetWidth() const { return _width; }
	inline int GetHeight() const { return _height; }
	Pixel* GetPixels(int* pixelCount = nullptr);

	static Image* CreateBlank(int width, int height, COLORREF bgColor);
	static Image* Capture(HWND hWnd, const RECT* captureRect);
	static Image* CaptureDesktop();
	static Image* LoadFile(const char* filename);

	void SaveToFile(const char* filename);
};
