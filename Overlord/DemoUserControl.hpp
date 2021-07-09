#pragma once
#include "UserControl.hpp"

class DemoUserControl : public UserControl {
public:
	DemoUserControl() {}
	void Initialize(HWND parent_window, int x, int y);

private:
	bool initialized = false;
	HWND edit_red{};
	HWND edit_green{};
	HWND edit_blue{};
	COLORREF bg_color{};
	void UpdateColor();
	void OnPaint();
	LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;
};
