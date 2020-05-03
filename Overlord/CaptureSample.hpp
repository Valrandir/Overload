#pragma once
#include "CaptureSource.hpp"
#include "Image.hpp"

struct CaptureSample {
	CaptureSource capture_source;
	Image* image;
};
