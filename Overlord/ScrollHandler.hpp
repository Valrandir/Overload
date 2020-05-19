#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class ScrollBarHandler {
private:
	HWND _parent_window{};
	SIZE _view{};
	SIZE _content{};
	POINT _position{};
	bool _h_scroll_enabled{}, _v_scroll_enabled{};

public:
	void Initialize(HWND parent_window, int view_width, int view_height, int content_width, int content_height);
	void HandleScroll(UINT msg, WPARAM wParam, LPARAM lParam);
	bool OffsetScroll(int sox, int soy);
	inline POINT GetPosition() { return {_position.x, _position.y}; }
};
