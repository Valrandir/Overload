#pragma once
#include "Dialog.hpp"
#include "DemoUserControl.hpp"

class DemoDialog : private Dialog {
	DemoUserControl demo_user_control;

public:
	static bool ShowDialog();
	void Initialize() override;
	INT_PTR DlgProc(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
	DemoDialog() {}

public:
	~DemoDialog();
};
