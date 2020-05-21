#pragma once

namespace ImageIO
{
	unsigned char* LoadFile(const char* filename, int& width, int& height);
	bool SaveFile(const char* filename, int width, int height, unsigned char* bits);
	void FreeBits(void* bits);
}
