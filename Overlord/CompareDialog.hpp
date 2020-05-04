#pragma once
#include "DialogBase.hpp"
#include "Image.hpp"

class CompareDialog : public DialogBase {
public:
	static bool ShowDialog(const Image* img1, const Image* img2);
	void Initialize() override;
	INT_PTR DlgProc(HWND hDialogWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

private:
	const Image *_img1, *_img2;

	CompareDialog(const Image* img1, const Image* img2) :
		_img1{img1}, _img2{img2} {}

	void DrawSampleImage(const Image* image, const DRAWITEMSTRUCT* dis);
};
