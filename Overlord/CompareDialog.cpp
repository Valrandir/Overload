#include "CompareDialog.hpp"
#include "resource.h"
#include <cassert>

bool CompareDialog::ShowDialog(const Image* img1, const Image* img2)
{
	assert(img1);
	assert(img2);

	CompareDialog compare_dialog(img1, img2);
	return compare_dialog.ShowModal(IDD_DIALOG_COMPARE);
}

void CompareDialog::Initialize()
{
}

INT_PTR CompareDialog::DlgProc(HWND hDialogWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) {
		case WM_DRAWITEM:
			if(wParam == IDC_STATIC_IMG_1) {
				DrawSampleImage(_img1, (const DRAWITEMSTRUCT*)lParam);
				return true;
			}
			if(wParam == IDC_STATIC_IMG_2) {
				DrawSampleImage(_img2, (const DRAWITEMSTRUCT*)lParam);
				return true;
			}
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDOK:
					EndDialog(hDialogWnd, DIALOG_SUCCESS);
					return true;
				case IDCANCEL:
					EndDialog(hDialogWnd, DIALOG_CANCEL);
					return true;
			}
	}

	return DialogBase::DlgProc(hDialogWnd, msg, wParam, lParam);
}

void CompareDialog::DrawSampleImage(const Image* image, const DRAWITEMSTRUCT* dis)
{
	if(dis->itemAction != ODA_DRAWENTIRE)
		return;

	auto hdcSrc = image->GetDC();
	auto w = image->GetWidth();
	auto h = image->GetHeight();

	BitBlt(dis->hDC, 0, 0, w, h, hdcSrc, 0, 0, SRCCOPY);
}
