#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class BitmapGdi {
protected:
	int width, height;
	HDC dc;
	HBITMAP bitmap;

public:
	BitmapGdi();
	BitmapGdi(int width, int height);
	BitmapGdi(int width, int height, HDC dc, HBITMAP bitmap);
	void Initialize(int width, int height);
	BitmapGdi(BitmapGdi&&) noexcept;
	BitmapGdi& operator=(BitmapGdi&&) noexcept;
	virtual ~BitmapGdi();

private:
	inline BitmapGdi& Move(BitmapGdi&) noexcept;
	inline void Destroy() noexcept;

public:
	[[nodiscard]] int GetWidth() const noexcept { return width; }
	[[nodiscard]] int GetHeight() const noexcept { return height; }
	[[nodiscard]] HDC GetDC() const noexcept { return dc; }
	[[nodiscard]] HBITMAP GetBitmap() const noexcept { return bitmap; }

	void Fill(COLORREF color);
	void Fill(const RECT& rect, COLORREF color);
	void Fill(int x, int y, int w, int h, COLORREF color);
	void Draw(HDC source, int x, int y, int w, int h);
};
