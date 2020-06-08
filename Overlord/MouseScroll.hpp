#pragma once
#include "Event.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class MouseScroll {
public:
	Event<int, int> ScrollEvent;

	MouseScroll() = default;
	MouseScroll(HWND window) :
		window{window} {};

	void SetWindow(HWND window) { this->window = window; }
	bool ProcessMessage(UINT msg, WPARAM wparam);

private:
	void OnLMouseMove();
	void OnLMouseDown();
	void OnLMouseUp();

	HWND window{};
	bool mouse_dragging{};
	POINT mouse_origin{};
};
