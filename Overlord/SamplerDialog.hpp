#pragma once
#include "DialogBase.hpp"
#include "BitmapGdi.hpp"
#include "HotkeyMonitor.hpp"
#include "ImageView.hpp"
#include <list>

class SamplerDialog : private DialogBase {
	struct Element {
		BitmapGdi bitmap;
		ImageView view;
	};

public:
	static bool ShowDialog();
	void Initialize() override;
	void InitializeHotKeys();
	INT_PTR DlgProc(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
	HotkeyMonitor hotkey_monitor;
	std::list<Element*> elements;
	SamplerDialog() {}
};
