#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class BitmapGdi {
public:
	BitmapGdi(int width, int height);
	BitmapGdi(BitmapGdi&&) noexcept;
	BitmapGdi& operator=(BitmapGdi&&) noexcept;
	virtual ~BitmapGdi();

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }
	inline HDC GetDC() const { return dc; }
	inline HBITMAP GetBitmap() const { return bitmap; }

	void Fill(const RECT& rect, COLORREF color);
	void Fill(int x, int y, int w, int h, COLORREF color);

private:
	int width, height;
	HDC dc;
	HBITMAP bitmap;
	inline BitmapGdi& Move(BitmapGdi& rvalue) noexcept;
	inline void Destroy() noexcept;
};
