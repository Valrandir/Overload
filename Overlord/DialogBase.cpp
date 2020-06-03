#include "DialogBase.hpp"
#include "RectUtility.hpp"
#include <utility>

bool DialogBase::ShowModal(UINT dialog_template)
{
	return DIALOG_SUCCESS == DialogBoxParam(GetModuleHandle(0), MAKEINTRESOURCE(dialog_template), HWND_DESKTOP, DialogBase::DlgProcStatic, (LPARAM)this);
}

void DialogBase::GetDlgItemPoint(HWND parent_window, int dlg_item, int& x, int& y, int& w, int& h)
{
	WINDOWPLACEMENT wp{};
	wp.length = sizeof(wp);
	GetWindowPlacement(GetDlgItem(parent_window, dlg_item), &wp);
	RectToPoint(wp.rcNormalPosition, x, y, w, h);
}

INT_PTR CALLBACK DialogBase::DlgProcStatic(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if(msg == WM_INITDIALOG) {
		SetWindowLongPtr(dialog_wnd, GWLP_USERDATA, (LONG_PTR)lparam);
		DialogBase* db = (DialogBase*)lparam;
		db->dialog_wnd = dialog_wnd;

		RECT rect;
		GetClientRect(dialog_wnd, &rect);
		db->layout_handler.Initialize(dialog_wnd, SIZE{rect.right - rect.left, rect.bottom - rect.top});
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
		case WM_SIZING: {
			RECT& new_rect = *(RECT*)lparam;
			layout_handler.OnSizing(new_rect, wparam);
			return true;
		}
		case WM_SIZE:
			layout_handler.OnSize(LOWORD(lparam), HIWORD(lparam));
			return true;
		case WM_CLOSE:
			EndDialog(dialog_wnd, DIALOG_CANCEL);
			return true;
	}

	return false;
}
