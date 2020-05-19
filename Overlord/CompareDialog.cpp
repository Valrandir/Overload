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

static void ScrollCallback(int offset_x, int offset_y, void* userdata)
{
	auto linked_image_view = (ImageView*)userdata;
	linked_image_view->ScrollImage(offset_x, offset_y);
}

static void ZoomCallback(bool is_zoom_out, void* userdata)
{
	auto linked_image_view = (ImageView*)userdata;

	if(is_zoom_out)
		linked_image_view->ZoomOut();
	else
		linked_image_view->ZoomIn();
}

bool CompareDialog::ShowDialog(const Image* img_l, const Image* img_r)
{
	assert(img_l);
	assert(img_r);

	CompareDialog compare_dialog(img_l, img_r);
	return compare_dialog.ShowModal(IDD_DIALOG_COMPARE);
}

void CompareDialog::Initialize()
{
	int x, y, w, h;

	GetDlgItemPoint(_hDialogWnd, IDC_STATIC_IMG_1, x, y, w, h);
	_image_view_l.Initialize(_hDialogWnd, x, y, w, h);
	_image_view_l.SetImage(this->_img_l);
	_image_view_l.ScrollEvent.SetHandler(ScrollCallback, &_image_view_r);
	_image_view_l.ZoomEvent.SetHandler(ZoomCallback, &_image_view_r);

	GetDlgItemPoint(_hDialogWnd, IDC_STATIC_IMG_2, x, y, w, h);
	_image_view_r.Initialize(_hDialogWnd, x, y, w, h);
	_image_view_r.SetImage(this->_img_r);
	_image_view_r.ScrollEvent.SetHandler(ScrollCallback, &_image_view_l);
	_image_view_r.ZoomEvent.SetHandler(ZoomCallback, &_image_view_l);
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
