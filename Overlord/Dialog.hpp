//VIS LAYOUT https://docs.microsoft.com/en-us/windows/win32/uxguide/vis-layout#recommended-sizing-and-spacing
#pragma once
#include "DialogLayout.hpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class Dialog {
public:
	bool ShowModal(UINT dialog_resource_id);
	static RECT GetDlgItemRect(HWND parent_window, int dlg_item);
	static RECT GetDlgItemRect(HWND dlg_item);

private:
	static INT_PTR CALLBACK DlgProcStatic(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam);

protected:
	HWND dialog_wnd{};
	DialogLayout layout;
	UINT dialog_resource_id{};

	static const int DIALOG_SUCCESS = 1;
	static const int DIALOG_CANCEL = 2;
	Dialog() {}
	virtual void Initialize() = 0;
	virtual INT_PTR DlgProc(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam);
	virtual void OnSize(LPARAM lparam);
	void SetIcon(UINT icon_resource_id);

private:
	Dialog(const Dialog&) = delete;
	Dialog& operator=(const Dialog&) = delete;
};
