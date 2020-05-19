#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class ScrollBarHandler {
public:
	void Setup(HWND parent_window, int view_width, int view_height, int content_width, int content_height);
	inline POINT GetPosition() { return {_scroll_pos_x, _scroll_pos_y}; }
	void HandleScroll(UINT msg, WPARAM wParam, LPARAM lParam);
	bool OffsetScroll(int sox, int soy);

private:
	bool SetupSingle(int scroll_pos, int view_size, int content_size, int sb_hv);
	void HandleScroll(WPARAM wParam, LPARAM lParam, int sb_hv, int& sref, int view_size, int content_size);
	void OffsetScroll(int sb_hv, int offset, int& sref, int max);
	HWND _parent_window{};
	int _view_width{}, _view_height{}, _content_width{}, _content_height{};
	int _scroll_pos_x{}, _scroll_pos_y{};
	bool _h_scroll_enabled{}, _v_scroll_enabled{};
};
