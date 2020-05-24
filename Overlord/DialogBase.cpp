#include "DialogBase.hpp"

bool DialogBase::ShowModal(UINT dialog_template)
{
	return DIALOG_SUCCESS == DialogBoxParam(GetModuleHandle(0), MAKEINTRESOURCE(dialog_template), HWND_DESKTOP, DialogBase::DlgProcStatic, (LPARAM)this);
}

INT_PTR CALLBACK DialogBase::DlgProcStatic(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if(msg == WM_INITDIALOG) {
		SetWindowLongPtr(dialog_wnd, GWLP_USERDATA, (LONG_PTR)lparam);
		DialogBase* db = (DialogBase*)lparam;
		db->dialog_wnd = dialog_wnd;
		db->Initialize();
		return true;
	}

	DialogBase* db;
	if(db = (DialogBase*)GetWindowLongPtr(dialog_wnd, GWLP_USERDATA))
		return db->DlgProc(dialog_wnd, msg, wparam, lparam);

	return false;
}

INT_PTR DialogBase::DlgProc(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg) {
		case WM_CLOSE:
			EndDialog(dialog_wnd, DIALOG_CANCEL);
			return true;
	}

	return false;
}

void DialogBase::RectToPoint(const RECT& rect, int& x, int& y, int& w, int& h)
{
	x = rect.left;
	y = rect.top;
	w = rect.right - x;
	h = rect.bottom - y;
}

void DialogBase::GetDlgItemPoint(HWND parent_window, int dlg_item, int& x, int& y, int& w, int& h)
{
	WINDOWPLACEMENT wp{};
	wp.length = sizeof(wp);
	GetWindowPlacement(GetDlgItem(parent_window, dlg_item), &wp);
	RectToPoint(wp.rcNormalPosition, x, y, w, h);
}
