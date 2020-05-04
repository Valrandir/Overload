#pragma once
#include "CaptureSource.hpp"
#include "Image.hpp"

struct CaptureSample : public CaptureSource {
	CaptureSample() {}

	CaptureSample(const CaptureSource& cs) :
		CaptureSource{cs} {}

	Image* image{};
};
