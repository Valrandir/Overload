#pragma once
#include "DialogBase.hpp"
#include "ImageView.hpp"

class CompareDialog : public DialogBase {
public:
	static bool ShowDialog(const Image* imgA, const Image* imgB);
	void Initialize() override;
	INT_PTR DlgProc(HWND hDialogWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

private:
	const Image *_imgA, *_imgB;
	ImageView _imageViewA, _imageViewB;

	CompareDialog(const Image* imgA, const Image* imgB) :
		_imgA{imgA}, _imgB{imgB} {}
};
