#include "ImageIO.hpp"
#include "lib/stb/stb_custom.h"
#include <algorithm>
#include <memory>

static void SwapRedAndBlue(unsigned char* bits, int width, int height);

namespace ImageIO
{
	unsigned char* LoadFile(const char* filename, int& width, int& height)
	{
		int comp;
		auto bits = stbi_load(filename, &width, &height, &comp, 4);

		if(bits)
			SwapRedAndBlue(bits, width, height);

		return bits;
	}

	bool SaveFile(const char* filename, int width, int height, unsigned char* bits)
	{
		size_t size = (size_t)width * height * 4;
		auto clone = new unsigned char[size];
		memcpy(clone, bits, size);

		SwapRedAndBlue(clone, width, height);

		auto success = 1 == stbi_write_png(filename, width, height, 4, clone, width * 4);

		delete[] clone;
		return success;
	}

	void FreeBits(void* bits)
	{
		stbi_image_free(bits);
	}
}

static void SwapRedAndBlue(unsigned char* bits, int width, int height)
{
	auto end = bits + (size_t)width * height * 4;
	for(auto it = bits; it < end; it += 4)
		std::swap(it[0], it[2]);
}
