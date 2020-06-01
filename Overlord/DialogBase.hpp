//VIS LAYOUT https://docs.microsoft.com/en-us/windows/win32/uxguide/vis-layout#recommended-sizing-and-spacing

#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class DialogBase {
public:
	bool ShowModal(UINT dialog_resource_id);
	static void RectToPoint(const RECT& rect, int& x, int& y, int& w, int& h);
	static void GetDlgItemPoint(HWND parent_window, int dlg_item, int& x, int& y, int& w, int& h);

private:
	static void EnforceMinimumSize(RECT& new_rect, const SIZE& min_size, WPARAM wparam);
	static INT_PTR CALLBACK DlgProcStatic(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam);

protected:
	HWND dialog_wnd{};
	SIZE dialog_size{};
	SIZE minimum_size{};

	static const int DIALOG_SUCCESS = 1;
	static const int DIALOG_CANCEL = 2;
	DialogBase() {}
	virtual void Initialize(){};
	void MoveChild(int dialog_item, const SIZE& offset);
	virtual void OnSize(int width, int height);
	virtual INT_PTR DlgProc(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
	DialogBase(const DialogBase&) = delete;
	DialogBase& operator=(const DialogBase&) = delete;
};
