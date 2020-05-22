#pragma once
#include "CaptureSource.hpp"
#include "Image.hpp"

struct CaptureSample : public CaptureSource {
	CaptureSample() :
		CaptureSource{} {}

	CaptureSample(const CaptureSource& cs) :
		CaptureSource{cs} {}

	Image* image{};
};
