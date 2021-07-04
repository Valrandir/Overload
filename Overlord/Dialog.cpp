#include "Dialog.hpp"
#include <utility>

bool Dialog::ShowModal(UINT dialog_resource_id)
{
	this->dialog_resource_id = dialog_resource_id;
	return DIALOG_SUCCESS == DialogBoxParam(GetModuleHandle(0), MAKEINTRESOURCE(dialog_resource_id), HWND_DESKTOP, Dialog::DlgProcStatic, (LPARAM)this);
}

RECT Dialog::GetDlgItemRect(HWND parent_window, int dlg_item)
{
	return GetDlgItemRect(GetDlgItem(parent_window, dlg_item));
}

RECT Dialog::GetDlgItemRect(HWND dlg_item)
{
	WINDOWPLACEMENT wp{};
	wp.length = sizeof(wp);
	GetWindowPlacement(dlg_item, &wp);
	return wp.rcNormalPosition;
}

INT_PTR CALLBACK Dialog::DlgProcStatic(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if(msg == WM_INITDIALOG) {
		SetWindowLongPtr(dialog_wnd, GWLP_USERDATA, (LONG_PTR)lparam);
		Dialog* db = (Dialog*)lparam;
		db->dialog_wnd = dialog_wnd;
		db->layout.Initialize(db->dialog_resource_id, dialog_wnd);
		db->Initialize();
		return true;
	}

	Dialog* db;
	if(db = (Dialog*)GetWindowLongPtr(dialog_wnd, GWLP_USERDATA))
		return db->DlgProc(dialog_wnd, msg, wparam, lparam);

	return false;
}

INT_PTR Dialog::DlgProc(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam)
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

void Dialog::OnSize(LPARAM lparam)
{
	layout.OnSize(LOWORD(lparam), HIWORD(lparam));
}

void Dialog::SetIcon(UINT icon_resource_id)
{
	HICON icon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCEW(icon_resource_id));
	SendMessageW(dialog_wnd, WM_SETICON, ICON_BIG, LPARAM(icon));
}
