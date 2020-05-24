#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class DialogBase {
public:
	bool ShowModal(UINT dialog_resource_id);
	static INT_PTR CALLBACK DlgProcStatic(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam);

protected:
	HWND dialog_wnd{};

	static const int DIALOG_SUCCESS = 1;
	static const int DIALOG_CANCEL = 2;
	DialogBase() {}
	virtual void Initialize(){};
	virtual INT_PTR DlgProc(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam);

	static void RectToPoint(const RECT& rect, int& x, int& y, int& w, int& h);
	static void GetDlgItemPoint(HWND parent_window, int dlg_item, int& x, int& y, int& w, int& h);

private:
	DialogBase(const DialogBase&) = delete;
	DialogBase& operator=(const DialogBase&) = delete;
};
