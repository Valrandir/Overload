#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class ScrollBarHandler {
private:
	HWND parent_window{};
	SIZE view{};
	SIZE content{};
	POINT position{};
	bool h_scroll_enabled{}, v_scroll_enabled{};

public:
	void Initialize(HWND parent_window, int view_width, int view_height, int content_width, int content_height);
	void HandleScroll(UINT msg, WPARAM wparam, LPARAM lparam);
	bool OffsetScroll(int sox, int soy);
	inline POINT GetPosition() { return {position.x, position.y}; }
};
