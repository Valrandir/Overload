#pragma once
#include "DialogBase.hpp"
#include "BitmapGdi.hpp"
#include "HotkeyMonitor.hpp"
#include "ImageView.hpp"
#include <list>

class SamplerDialog : private DialogBase {
	struct Element {
		BitmapGdi bitmap_gdi;
		ImageView image_view;

		Element(BitmapGdi&& bitmap_gdi)
		{
			this->bitmap_gdi = std::move(bitmap_gdi);
		}
	};

public:
	static bool ShowDialog();
	void Initialize() override;
	void InitializeHotKeys();
	INT_PTR DlgProc(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
	HotkeyMonitor hotkey_monitor;
	Element* root_element{};
	std::list<Element*> elements;
	SamplerDialog() {}
	void CaptureRootElement();
	static void OnHotKeyCaptureInitial(int, void* userdata);
	static void OnHotKeyCaptureNext(int, void* userdata);

public:
	~SamplerDialog();
};
