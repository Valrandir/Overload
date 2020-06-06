#include "DialogBase.hpp"
#include <utility>

bool DialogBase::ShowModal(UINT dialog_resource_id)
{
	this->dialog_resource_id = dialog_resource_id;
	return DIALOG_SUCCESS == DialogBoxParam(GetModuleHandle(0), MAKEINTRESOURCE(dialog_resource_id), HWND_DESKTOP, DialogBase::DlgProcStatic, (LPARAM)this);
}

RECT DialogBase::GetDlgItemRect(HWND parent_window, int dlg_item)
{
	return GetDlgItemRect(GetDlgItem(parent_window, dlg_item));
}

RECT DialogBase::GetDlgItemRect(HWND dlg_item)
{
	WINDOWPLACEMENT wp{};
	wp.length = sizeof(wp);
	GetWindowPlacement(dlg_item, &wp);
	return wp.rcNormalPosition;
}

INT_PTR CALLBACK DialogBase::DlgProcStatic(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if(msg == WM_INITDIALOG) {
		SetWindowLongPtr(dialog_wnd, GWLP_USERDATA, (LONG_PTR)lparam);
		DialogBase* db = (DialogBase*)lparam;
		db->dialog_wnd = dialog_wnd;
		db->layout.Initialize(db->dialog_resource_id, dialog_wnd);
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
			layout.OnSizing(new_rect, wparam);
			return true;
		}
		case WM_SIZE:
			OnSize(lparam);
			return true;
		case WM_CLOSE:
			EndDialog(dialog_wnd, DIALOG_CANCEL);
			return true;
	}

	return false;
}

void DialogBase::OnSize(LPARAM lparam)
{
	layout.OnSize(LOWORD(lparam), HIWORD(lparam));
}
