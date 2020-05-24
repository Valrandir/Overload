#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class BitmapGdi {
public:
	BitmapGdi(int width, int height);
	BitmapGdi(BitmapGdi&&) noexcept;
	BitmapGdi& operator=(BitmapGdi&&) noexcept;
	virtual ~BitmapGdi();

	inline int GetWidth() const { return _width; }
	inline int GetHeight() const { return _height; }
	inline HDC GetDC() const { return _hDC; }
	inline HBITMAP GetBitmap() const { return _hBitmap; }

	void Fill(const RECT& rect, COLORREF color);
	void Fill(int x, int y, int w, int h, COLORREF color);

private:
	int _width, _height;
	HDC _hDC;
	HBITMAP _hBitmap;
	inline BitmapGdi& Move(BitmapGdi& rvalue) noexcept;
	inline void Destroy() noexcept;
};
