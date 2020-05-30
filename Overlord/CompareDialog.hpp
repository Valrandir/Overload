#pragma once
#include "DialogBase.hpp"
#include "BitmapGdi.hpp"
#include "ImageView.hpp"

class CompareDialog : private DialogBase {
public:
	static bool ShowDialog(const BitmapGdi* bitmap_gdi_a, const BitmapGdi* bitmap_gdi_b);
	void Initialize() override;
	INT_PTR DlgProc(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam) override;
	void OnCompare();

private:
	const BitmapGdi *bitmap_gdi_a, *bitmap_gdi_b;
	ImageView imageview_a, imageview_b;

	CompareDialog(const BitmapGdi* bitmap_gdi_a, const BitmapGdi* bitmap_gdi_b) :
		bitmap_gdi_a{bitmap_gdi_a}, bitmap_gdi_b{bitmap_gdi_b} {}
};
