#pragma once
#pragma warning(push)
#pragma warning(disable : 6011 6262 6285 6308 6385 26451)

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#define STBI_NO_FAILURE_STRINGS
#define STBI_NO_GIF
#define STBI_NO_LINEAR
#define STBI_NO_HDR
#define STBI_ONLY_PNG
#define STBI_ASSERT

#include "stb_image.h"
#include "stb_image_write.h"

#pragma warning(pop)
