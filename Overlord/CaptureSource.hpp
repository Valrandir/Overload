#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

struct CaptureSource {
	HWND window_handle;
	std::wstring window_title;
	RECT source_rect{};
};
