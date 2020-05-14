#include "CompareDialog.hpp"
#include "resource.h"
#include <cassert>

static void RectToPoint(const RECT& rect, int& x, int& y, int& w, int& h)
{
	x = rect.left;
	y = rect.top;
	w = rect.right - x;
	h = rect.bottom - y;
}

static void GetDlgItemPoint(HWND parent_window, int dlg_item, int& x, int& y, int& w, int& h)
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(GetDlgItem(parent_window, dlg_item), &wp);
	RectToPoint(wp.rcNormalPosition, x, y, w, h);
}

bool CompareDialog::ShowDialog(const Image* imgA, const Image* imgB)
{
	assert(imgA);
	assert(imgB);

	CompareDialog compare_dialog(imgA, imgB);
	return compare_dialog.ShowModal(IDD_DIALOG_COMPARE);
}

void CompareDialog::Initialize()
{
	int x, y, w, h;

	GetDlgItemPoint(_hDialogWnd, IDC_STATIC_IMG_1, x, y, w, h);
	_imageViewA.Initialize(_hDialogWnd, x, y, w, h);
	_imageViewA.SetImage(this->_imgA);

	GetDlgItemPoint(_hDialogWnd, IDC_STATIC_IMG_2, x, y, w, h);
	_imageViewB.Initialize(_hDialogWnd, x, y, w, h);
	_imageViewB.SetImage(this->_imgB);
}

INT_PTR CompareDialog::DlgProc(HWND hDialogWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) {
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
