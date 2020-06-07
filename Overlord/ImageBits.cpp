#include "ImageBits.hpp"
#include "ImageIO.hpp"
#include <exception>
#include <fstream>

//#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>

ImageBits::ImageBits(ImageBits&& other) noexcept
{
	Move(other);
}

ImageBits& ImageBits::operator=(ImageBits&& other) noexcept
{
	if(this == &other)
		return *this;

	Destroy();
	return Move(other);
}

ImageBits::~ImageBits() { Destroy(); }

ImageBits::Pixel* ImageBits::GetPixel(int x, int y)
{
	return _begin + y * (size_t)width + x;
}

const ImageBits::Pixel* ImageBits::GetPixel(int x, int y) const
{
	return _begin + y * (size_t)width + x;
}

inline ImageBits& ImageBits::Move(ImageBits& other) noexcept
{
	_begin = other._begin;
	_end = other._end;
	width = other.width;
	height = other.height;
	created_from_imageio = other.created_from_imageio;

	other._begin = nullptr;
	other._end = nullptr;
	other.width = 0;
	other.height = 0;
	other.created_from_imageio = created_from_imageio;

	return *this;
}

void ImageBits::Destroy()
{
	if(_begin) {
		if(created_from_imageio) {
			ImageIO::FreeBits(_begin);
			created_from_imageio = false;
		} else
			delete[] _begin;

		_begin = nullptr;
	}

	_end = nullptr;
}

ImageBits::Pixel ImageBits::CreatePixel(Byte red, Byte green, Byte blue, Byte alpha)
{
	return Pixel{blue, green, red, alpha};
}

ImageBits::Byte* ImageBits::AllocBits(int width, int height)
{
	auto size = (size_t)width * height * 4;
	return new Byte[size];
}

void ImageBits::FreeBits(Byte* bits)
{
	if(bits)
		delete[] bits;
}

ImageBits ImageBits::CreateFromBits(int width, int height, Byte* bits, bool created_from_imageio)
{
	auto begin = (Pixel*)bits;
	auto end = begin + (size_t)width * height;
	auto image_bits = ImageBits(begin, end, width, height);
	image_bits.created_from_imageio = created_from_imageio;
	return image_bits;
}

ImageBits ImageBits::CreateEmpty(int width, int height)
{
	auto bits = AllocBits(width, height);
	return CreateFromBits(width, height, bits);
}

ImageBits ImageBits::LoadPNG(const char* filename)
{
	int width, height;

	auto bits = (ImageBits::Pixel*)ImageIO::LoadFile(filename, width, height);
	if(!bits)
		throw std::exception("ImageIO::LoadFile failed"); //TODO: Reconsider using exceptions

	auto begin = (ImageBits::Pixel*)bits;
	auto end = begin + (size_t)width * height;

	auto image_bits = ImageBits(begin, end, width, height);
	image_bits.created_from_imageio = true;
	return image_bits;
}

ImageBits ImageBits::LoadBits(const char* filename)
{
	std::ifstream in(filename, std::ostream::binary);
	if(in.fail())
		throw new std::exception("ifstream failed"); //TODO: Reconsider using exceptions

	FileHeader fh;
	in.read((char*)&fh, sizeof(fh));
	in.seekg(fh.header_bytesize);

	if((size_t)fh.width * fh.height * sizeof(Pixel) != fh.data_bytesize)
		throw new std::exception("width * height * Pixel does not match data_bytesize"); //TODO: Reconsider using exceptions

	Pixel* data = new Pixel[fh.width * fh.height];
	in.read((char*)data, fh.data_bytesize);
	in.close();

	return ImageBits(data, data + fh.data_bytesize, fh.width, fh.height);
}

void ImageBits::SavePNG(const char* filename)
{
	int saved = ImageIO::SaveFile(filename, width, height, (Byte*)_begin);

	if(!saved)
		throw std::exception("stbi_write_png failed"); //TODO: Reconsider using exceptions
}

void ImageBits::SaveBits(const char* filename)
{
	FileHeader fh;
	fh.data_bytesize = ByteSize();
	fh.width = width;
	fh.height = height;

	std::ofstream out(filename, std::ostream::binary);
	if(out.fail())
		throw new std::exception("ofstream failed"); //TODO: Reconsider using exceptions

	out.write((char*)&fh, sizeof(fh));
	out.write((char*)_begin, fh.data_bytesize);
	out.close();
}

void ImageBits::Clear(const Pixel& color)
{
	for(auto& it : *this)
		it = color;
}

void ImageBits::Fill(int x, int y, int w, int h, const Pixel& color, BlendMode blend_mode)
{
	int x2, y2;
	if(!AdjustClip(x, y, x2, y2, w, h))
		return;

	auto dest_y = GetPixel(x, y);
	auto dest_y_end = GetPixel(0, y2);
	for(; dest_y < dest_y_end; dest_y += width) {
		auto dest_x = dest_y;
		auto dest_x_end = dest_x + x2 - x;
		for(; dest_x < dest_x_end; ++dest_x)
			Blend(*dest_x, color, blend_mode);
	}
}

void ImageBits::Fill(int x, int y, const ImageBits& source, BlendMode blend_mode)
{
	int sx = x, sy = y;
	int x2, y2;
	if(!AdjustClip(x, y, x2, y2, source.Width(), source.Height()))
		return;

	sx = sx < x ? -sx : 0;
	sy = sy < y ? -sy : 0;

	auto dest_y = GetPixel(x, y);
	auto src_y = source.GetPixel(sx, sy);
	auto dest_y_end = GetPixel(0, y2 - sy);
	for(; dest_y < dest_y_end; dest_y += width, src_y += source.Width()) {
		auto dest_x = dest_y;
		auto src_x = src_y;
		auto dest_x_end = dest_x + x2 - x - sx;
		for(; dest_x < dest_x_end; ++dest_x, ++src_x)
			Blend(*dest_x, *src_x, blend_mode);
	}
}

bool ImageBits::AdjustClip(int& x, int& y, int& x2, int& y2, int w, int h)
{
	if(x < 0)
		x = 0;

	if(y < 0)
		y = 0;

	x2 = x + w;
	y2 = y + h;

	if(x2 > width)
		x2 = width;

	if(y2 > height)
		y2 = height;

	return !(x2 <= x || y2 <= y || x >= width || y >= height || x2 > width || y2 > height);
}

void ImageBits::Blend(ImageBits::Pixel& dest, const ImageBits::Pixel& src, BlendMode blend_mode)
{
	switch(blend_mode) {
		case BlendMode::Add: {
			auto a = src.a / 255.0;
			dest.b = (Byte)(dest.b + src.b * a);
			dest.g = (Byte)(dest.g + src.g * a);
			dest.r = (Byte)(dest.r + src.r * a);
			return;
		}
		case BlendMode::Alpha: {
			auto src_a = src.a / 255.0;
			auto dest_a = dest.a / 255.0;
			auto alpha = (src.a + dest.a * (1 - src_a)) / 255.0;
			if(alpha == 0) {
				dest.b = dest.g = dest.r = 0;
				return;
			}
			dest.b = (Byte)((src.b * src_a + dest.b * dest_a * (1 - src_a)) / alpha);
			dest.g = (Byte)((src.g * src_a + dest.g * dest_a * (1 - src_a)) / alpha);
			dest.r = (Byte)((src.r * src_a + dest.r * dest_a * (1 - src_a)) / alpha);
			dest.a = (Byte)(src.a + dest.a * (1 - src_a));
			return;
		}
		default:
			dest = src;
			return;
	}
}

inline bool operator==(const ImageBits::Pixel& a, const ImageBits::Pixel& b)
{
	return a.b == b.b && a.g == b.g && a.r == b.r && a.a == b.a;
}
