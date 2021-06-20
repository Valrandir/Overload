#include "BitmapGdiZoom.hpp"

BitmapGdiZoom::BitmapGdiZoom() :
	BitmapGdi()
{
}

BitmapGdiZoom::BitmapGdiZoom(int width, int height) :
	BitmapGdi(width, height)
{
	original_bitmap = this->Clone();
}

BitmapGdiZoom::BitmapGdiZoom(int width, int height, HDC dc, HBITMAP bitmap) :
	BitmapGdi(width, height, dc, bitmap)
{
	original_bitmap = this->Clone();
}

BitmapGdiZoom::BitmapGdiZoom(BitmapGdi&& other) noexcept
{
	BitmapGdi::Move(other);
	original_bitmap = this->Clone();
}

BitmapGdiZoom& BitmapGdiZoom::operator=(BitmapGdi&& other) noexcept
{
	if(this == &other)
		return *this;

	Destroy();
	BitmapGdi::Move(other);
	original_bitmap = this->Clone();

	return *this;
}

BitmapGdiZoom::BitmapGdiZoom(BitmapGdiZoom&& other) noexcept
{
	Move(other);
}

BitmapGdiZoom& BitmapGdiZoom::operator=(BitmapGdiZoom&& other) noexcept
{
	if(this == &other)
		return *this;

	Destroy();
	return Move(other);
}

BitmapGdiZoom::~BitmapGdiZoom()
{
	BitmapGdi::Destroy();
}

BitmapGdiZoom& BitmapGdiZoom::Move(BitmapGdiZoom& other) noexcept
{
	zoom = other.zoom;

	other.zoom = 0;

	BitmapGdi::Move(other);

	return *this;
}

void BitmapGdiZoom::Destroy() noexcept
{
	zoom = 0;
}

void BitmapGdiZoom::Zoom(float factor)
{
	zoom = factor;

	int w = (int)(original_bitmap.Width() * zoom);
	int h = (int)(original_bitmap.Height() * zoom);

	Initialize(w, h);
	StretchBlt(GetDC(), 0, 0, w, h, original_bitmap.GetDC(), 0, 0, original_bitmap.Width(), original_bitmap.Height(), SRCCOPY);
}
