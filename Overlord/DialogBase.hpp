//VIS LAYOUT https://docs.microsoft.com/en-us/windows/win32/uxguide/vis-layout#recommended-sizing-and-spacing
#pragma once
#include "DialogLayout.hpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class DialogBase {
public:
	bool ShowModal(UINT dialog_resource_id);
	static void GetDlgItemPoint(HWND parent_window, int dlg_item, int& x, int& y, int& w, int& h);

private:
	void InitializeLayout();
	static INT_PTR CALLBACK DlgProcStatic(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam);

protected:
	HWND dialog_wnd{};
	DialogLayout layout;
	UINT dialog_resource_id{};

	static const int DIALOG_SUCCESS = 1;
	static const int DIALOG_CANCEL = 2;
	DialogBase() {}
	virtual void Initialize() = 0;
	virtual INT_PTR DlgProc(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
	DialogBase(const DialogBase&) = delete;
	DialogBase& operator=(const DialogBase&) = delete;
};
