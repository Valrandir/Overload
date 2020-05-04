#include "DialogBase.hpp"

bool DialogBase::ShowModal(UINT dialog_template)
{
	return DIALOG_SUCCESS == DialogBoxParam(GetModuleHandle(0), MAKEINTRESOURCE(dialog_template), HWND_DESKTOP, DialogBase::DlgProcStatic, (LPARAM)this);
}

INT_PTR CALLBACK DialogBase::DlgProcStatic(HWND hDialogWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg == WM_INITDIALOG) {
		SetWindowLongPtr(hDialogWnd, GWLP_USERDATA, (LONG_PTR)lParam);
		DialogBase* db = (DialogBase*)lParam;
		db->_hDialogWnd = hDialogWnd;
		db->Initialize();
		return true;
	}

	DialogBase* db;
	if(db = (DialogBase*)GetWindowLongPtr(hDialogWnd, GWLP_USERDATA))
		return db->DlgProc(hDialogWnd, msg, wParam, lParam);

	return false;
}

INT_PTR DialogBase::DlgProc(HWND hDialogWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) {
		case WM_CLOSE:
			EndDialog(hDialogWnd, DIALOG_CANCEL);
			return true;
	}

	return false;
}
