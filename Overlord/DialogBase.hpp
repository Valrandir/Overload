#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class DialogBase {
public:
	bool ShowModal(UINT dialog_resource_id);
	static INT_PTR CALLBACK DlgProcStatic(HWND hDialogWnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	static const int DIALOG_SUCCESS = 1;
	static const int DIALOG_CANCEL = 2;
	DialogBase() {}
	virtual INT_PTR DlgProc(HWND hDialogWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	DialogBase(const DialogBase&) = delete;
	DialogBase& operator=(const DialogBase&) = delete;
};
