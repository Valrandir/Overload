#include "ImageBits.hpp"

ImageBits::ImageBits(ImageBits&& other) noexcept { Move(other); }

ImageBits& ImageBits::operator=(ImageBits&& other) noexcept
{
	if(this == &other)
		return *this;

	Free();
	return Move(other);
}

ImageBits::~ImageBits() { Free(); }

inline ImageBits& ImageBits::Move(ImageBits& other) noexcept
{
	_begin = other._begin;
	_end = other._end;
	width = other.width;
	height = other.height;

	other._begin = nullptr;
	other._end = nullptr;
	other.width = 0;
	other.height = 0;

	return *this;
}

ImageBits::Pixel* ImageBits::Alloc(int width, int height)
{
	size_t count = (size_t)width * height;
	_begin = new Pixel[count];
	_end = _begin + count;
	this->width = width;
	this->height = height;
	return _begin;
}

void ImageBits::Free()
{
	if(_begin) {
		delete[] _begin;
		_begin = nullptr;
	}

	_end = nullptr;
}
