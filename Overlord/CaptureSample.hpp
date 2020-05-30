#pragma once
#include "CaptureSource.hpp"
#include "BitmapGdi.hpp"

struct CaptureSample : public CaptureSource {
	CaptureSample() :
		CaptureSource{} {}

	BitmapGdi* bitmap_gdi{};
};
