#pragma once

#include "BitmapGdi.hpp"
#include <utility>

class BitmapGdiZoom : public BitmapGdi {
	BitmapGdi original;
	float zoom{};

public:
	BitmapGdiZoom();
	BitmapGdiZoom(int width, int height);
	BitmapGdiZoom(int width, int height, HDC dc, HBITMAP bitmap);
	virtual void Initialize(int width, int height);
	BitmapGdiZoom(BitmapGdi&&) noexcept;
	BitmapGdiZoom& operator=(BitmapGdi&&) noexcept;
	BitmapGdiZoom(BitmapGdiZoom&&) noexcept;
	BitmapGdiZoom& operator=(BitmapGdiZoom&&) noexcept;
	virtual ~BitmapGdiZoom();

private:
	BitmapGdiZoom& Move(BitmapGdiZoom&) noexcept;
	void Destroy() noexcept;

public:
	const BitmapGdi& Original() { return original; }
	const float Zoom() { return zoom; }

	void Zoom(float factor);
};
