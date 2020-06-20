#include "BitmapGdi.hpp"
#include <cassert>
#include <utility>

#define INITIALIZE_ASSERT assert(dc);

BitmapGdi::BitmapGdi() :
	width{0}, height{0}, dc{0}, bitmap{0} {}

BitmapGdi::BitmapGdi(int width, int height)
{
	Initialize(width, height);
}

BitmapGdi::BitmapGdi(int width, int height, HDC dc, HBITMAP bitmap) :
	width{width}, height{height}, dc{dc}, bitmap{bitmap} {}

void BitmapGdi::Initialize(int width, int height)
{
	Destroy();

	this->width = width;
	this->height = height;

	HDC hDesktopDC = ::GetDC(HWND_DESKTOP);
	dc = CreateCompatibleDC(hDesktopDC);
	bitmap = CreateCompatibleBitmap(hDesktopDC, width, height);
	SelectObject(dc, bitmap);
	ReleaseDC(HWND_DESKTOP, hDesktopDC);
}

BitmapGdi::BitmapGdi(BitmapGdi&& other) noexcept { Move(other); }

BitmapGdi& BitmapGdi::operator=(BitmapGdi&& other) noexcept
{
	if(this == &other)
		return *this;

	Destroy();
	return Move(other);
}

BitmapGdi::~BitmapGdi() { Destroy(); }

BitmapGdi& BitmapGdi::Move(BitmapGdi& other) noexcept
{
	width = other.width;
	height = other.height;
	dc = other.dc;
	bitmap = other.bitmap;

	other.width = 0;
	other.height = 0;
	other.dc = 0;
	other.bitmap = 0;

	return *this;
}

void BitmapGdi::Destroy() noexcept
{
	if(dc) {
		DeleteDC(dc);
		dc = nullptr;
	}

	if(bitmap) {
		DeleteObject(bitmap);
		bitmap = nullptr;
	}

	width = 0;
	height = 0;
}

ImageBits BitmapGdi::GetBits()
{
	INITIALIZE_ASSERT
	auto const_this = (const BitmapGdi*)this;
	return const_this->GetBits();
}

const ImageBits BitmapGdi::GetBits() const
{
	INITIALIZE_ASSERT
	BITMAPINFO bi{};
	BITMAPINFOHEADER& bih = bi.bmiHeader;
	bih.biSize = sizeof(bih);
	GetDIBits(dc, bitmap, 0, 0, nullptr, &bi, 0);

	auto width = bih.biWidth;
	auto height = bih.biHeight;
	auto bits = ImageBits::AllocBits(width, height);

	bih.biHeight = -height;
	bih.biBitCount = 32;
	bih.biCompression = BI_RGB;
	GetDIBits(dc, bitmap, 0, height, bits, &bi, DIB_RGB_COLORS);

	return ImageBits::CreateFromBits(width, height, bits);
}

BitmapGdi BitmapGdi::CreateFromBits(const ImageBits& image_bits)
{
	//Create bitmap
	auto width = image_bits.Width();
	auto height = image_bits.Height();
	HDC desktop_dc = ::GetDC(HWND_DESKTOP);
	HDC dc = CreateCompatibleDC(desktop_dc);
	HBITMAP bitmap = CreateCompatibleBitmap(desktop_dc, width, height);
	ReleaseDC(HWND_DESKTOP, desktop_dc);

	//Set bits
	BITMAPINFO bi{};
	BITMAPINFOHEADER& bih = bi.bmiHeader;
	bih.biSize = sizeof(bih);
	bih.biWidth = width;
	bih.biHeight = -height;
	bih.biPlanes = 1;
	bih.biBitCount = 32;
	bih.biCompression = BI_RGB;
	SetDIBits(dc, bitmap, 0, height, image_bits.cbegin(), &bi, DIB_RGB_COLORS);

	SelectObject(dc, bitmap);

	return BitmapGdi(width, height, dc, bitmap);
}

BitmapGdi BitmapGdi::LoadPNG(const char* filename)
{
	return CreateFromBits(ImageBits::LoadPNG(filename));
}

void BitmapGdi::SavePNG(const char* filename)
{
	INITIALIZE_ASSERT
	auto image_bits = GetBits();

	for(auto& it : image_bits)
		it.a = 255;

	image_bits.SavePNG(filename);
}

void BitmapGdi::Clear(COLORREF color)
{
	INITIALIZE_ASSERT
	Fill({0, 0, width, height}, color);
}

void BitmapGdi::Fill(const RECT& rect, COLORREF color)
{
	INITIALIZE_ASSERT
	HBRUSH brush = CreateSolidBrush(color);
	::FillRect(dc, &rect, brush);
	DeleteObject(brush);
}

void BitmapGdi::Fill(int x, int y, int w, int h, COLORREF color)
{
	INITIALIZE_ASSERT
	Fill({x, y, x + w, y + h}, color);
}

void BitmapGdi::Draw(HDC source, int x, int y, int w, int h, int src_x, int src_y)
{
	INITIALIZE_ASSERT
	BitBlt(dc, x, y, w, h, source, src_x, src_y, SRCCOPY);
}

BitmapGdi BitmapGdi::Clone() const
{
	BitmapGdi clone(width, height);
	clone.Draw(dc, 0, 0, width, height);
	return std::move(clone);
}
