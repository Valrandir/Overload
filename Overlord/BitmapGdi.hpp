#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "ImageBits.hpp"

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

	int Width() const noexcept { return width; }
	int Height() const noexcept { return height; }
	HDC GetDC() const noexcept { return dc; }

private:
	BitmapGdi& Move(BitmapGdi&) noexcept;
	void Destroy() noexcept;

public:
	ImageBits GetBits();
	const ImageBits GetBits() const;
	static BitmapGdi CreateFromBits(const ImageBits& image_bits);
	static BitmapGdi LoadFile(const char* filename);
	void SaveFile(const char* filename);

	void Clear(COLORREF color);
	void Fill(const RECT& rect, COLORREF color);
	void Fill(int x, int y, int w, int h, COLORREF color);
	void Draw(HDC source, int x, int y, int w, int h, int src_x = {}, int src_y = {});
};
