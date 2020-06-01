#include "DialogBase.hpp"

bool DialogBase::ShowModal(UINT dialog_template)
{
	return DIALOG_SUCCESS == DialogBoxParam(GetModuleHandle(0), MAKEINTRESOURCE(dialog_template), HWND_DESKTOP, DialogBase::DlgProcStatic, (LPARAM)this);
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

void DialogBase::EnforceMinimumSize(RECT& new_rect, const SIZE& min_size, WPARAM wparam)
{
	int x, y, w, h;
	RectToPoint(new_rect, x, y, w, h);

	SIZE offset = {w - min_size.cx, h - min_size.cy};

	if(offset.cx < 0)
		if(wparam == WMSZ_LEFT || wparam == WMSZ_TOPLEFT || wparam == WMSZ_BOTTOMLEFT)
			new_rect.left += offset.cx;
		else
			new_rect.right -= offset.cx;

	if(offset.cy < 0)
		if(wparam == WMSZ_TOP || wparam == WMSZ_TOPLEFT || wparam == WMSZ_TOPRIGHT)
			new_rect.top += offset.cy;
		else
			new_rect.bottom -= offset.cy;
}

INT_PTR CALLBACK DialogBase::DlgProcStatic(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if(msg == WM_INITDIALOG) {
		SetWindowLongPtr(dialog_wnd, GWLP_USERDATA, (LONG_PTR)lparam);
		DialogBase* db = (DialogBase*)lparam;
		db->dialog_wnd = dialog_wnd;

		RECT rect;
		GetClientRect(dialog_wnd, &rect);
		db->dialog_size = SIZE{rect.right - rect.left, rect.bottom - rect.top};
		db->minimum_size = db->dialog_size;
		db->Initialize();

		return true;
	}

	DialogBase* db;
	if(db = (DialogBase*)GetWindowLongPtr(dialog_wnd, GWLP_USERDATA))
		return db->DlgProc(dialog_wnd, msg, wparam, lparam);

	return false;
}

void DialogBase::MoveChild(int dialog_item, const SIZE& offset)
{
	RECT rect;
	auto child = GetDlgItem(dialog_wnd, dialog_item);
	GetWindowRect(child, &rect);
	OffsetRect(&rect, offset.cx, offset.cy);
	int x, y, w, h;
	RectToPoint(rect, x, y, w, h);
	POINT p{x, y};
	ScreenToClient(dialog_wnd, &p);
	MoveWindow(child, p.x, p.y, w, h, TRUE);
}

void DialogBase::OnSize(int width, int height)
{
	auto offset = SIZE{width - dialog_size.cx, height - dialog_size.cy};

	if(offset.cx == 0 && offset.cy == 0)
		return;

	dialog_size.cx = width;
	dialog_size.cy = height;

	MoveChild(IDOK, offset);
	MoveChild(IDCANCEL, offset);
}

INT_PTR DialogBase::DlgProc(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg) {
		case WM_SIZING: {
			RECT& new_rect = *(RECT*)lparam;
			EnforceMinimumSize(new_rect, minimum_size, wparam);
			return true;
		}
		case WM_SIZE:
			OnSize(LOWORD(lparam), HIWORD(lparam));
			InvalidateRect(dialog_wnd, NULL, FALSE);
			return true;
		case WM_CLOSE:
			EndDialog(dialog_wnd, DIALOG_CANCEL);
			return true;
	}

	return false;
}
