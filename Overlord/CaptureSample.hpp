#pragma once
#include "CaptureSource.hpp"
#include "Image.hpp"

struct CaptureSample : public CaptureSource {
	Image* image{};
};
