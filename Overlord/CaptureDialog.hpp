#pragma once
#include "CaptureSample.hpp"

class CaptureDialog {
public:
	CaptureDialog() = delete;
	static bool ShowDialog(const CaptureSample& capture_sample);
};
