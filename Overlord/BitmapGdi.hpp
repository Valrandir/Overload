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
	virtual void Initialize(int width, int height);
	BitmapGdi(BitmapGdi&&) noexcept;
	BitmapGdi& operator=(BitmapGdi&&) noexcept;
	virtual ~BitmapGdi();

	virtual int Width() const noexcept { return width; }
	virtual int Height() const noexcept { return height; }
	virtual HDC GetDC() const noexcept { return dc; }

protected:
	BitmapGdi& Move(BitmapGdi&) noexcept;
	void Destroy() noexcept;

public:
	virtual ImageBits GetBits();
	virtual const ImageBits GetBits() const;
	static BitmapGdi CreateFromBits(const ImageBits& image_bits);
	static BitmapGdi LoadPNG(const char* filename);
	virtual void SavePNG(const char* filename);

	virtual void Clear(COLORREF color);
	virtual void Fill(const RECT& rect, COLORREF color);
	virtual void Fill(int x, int y, int w, int h, COLORREF color);
	virtual void Draw(HDC source, int x, int y, int w, int h, int src_x = {}, int src_y = {});
	virtual BitmapGdi Clone() const;
};
