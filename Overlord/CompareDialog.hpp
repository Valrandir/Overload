#pragma once
#include "DialogBase.hpp"
#include "ImageView.hpp"

class CompareDialog : private DialogBase {
public:
	static bool ShowDialog(const Image* image_a, const Image* image_b);
	void Initialize() override;
	INT_PTR DlgProc(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam) override;
	void OnCompare();

private:
	const Image *image_a, *image_b;
	ImageView imageview_a, imageview_b;

	CompareDialog(const Image* image_a, const Image* image_b) :
		image_a{image_a}, image_b{image_b} {}
};
