#pragma once
#include "DialogBase.hpp"

class SamplerDialog : private DialogBase {
public:
	static bool ShowDialog();
	void Initialize() override;
	INT_PTR DlgProc(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
	SamplerDialog() {}
};
