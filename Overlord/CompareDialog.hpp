#pragma once
#include "DialogBase.hpp"
#include "ImageView.hpp"

class CompareDialog : public DialogBase {
public:
	static bool ShowDialog(const Image* img_l, const Image* img_r);
	void Initialize() override;
	INT_PTR DlgProc(HWND hDialogWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

private:
	const Image *_img_l, *_img_r;
	ImageView _image_view_l, _image_view_r;

	CompareDialog(const Image* img_l, const Image* img_r) :
		_img_l{img_l}, _img_r{img_r} {}
};
