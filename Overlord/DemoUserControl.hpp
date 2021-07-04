#pragma once
#include "UserControl.hpp"

class DemoUserControl : public UserControl {
public:
	DemoUserControl() {}

	void Initialize(HWND parent_window, int x, int y);

private:
	LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;
};
