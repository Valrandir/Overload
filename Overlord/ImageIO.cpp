#include "ImageIO.hpp"
#include "lib/stb/stb_custom.h"

namespace ImageIO
{
	unsigned char* LoadFile(const char* filename, int& width, int& height)
	{
		int comp;
		return stbi_load(filename, &width, &height, &comp, 4);
	}

	bool SaveFile(const char* filename, int width, int height, unsigned char* bits)
	{
		return 1 == stbi_write_png(filename, width, height, 4, bits, width * 4);
	}

	void FreeBits(void* bits)
	{
		stbi_image_free(bits);
	}
}
