#include "LayoutHandler.hpp"
#include "RectUtility.hpp"
#include <cassert>

#define INITIALIZE_ASSERT assert(dialog_wnd);

using UserData = std::pair<LayoutHandler*, SIZE*>;

void LayoutHandler::Initialize(HWND dialog_wnd, SIZE initial_dialog_size)
{
	this->dialog_wnd = dialog_wnd;
	this->dialog_size = initial_dialog_size;
	this->dialog_size_min = initial_dialog_size;
}

void LayoutHandler::DefineLayout(LayoutHandler::Layout layout, HWND dialog_wnd, int dlgitemid)
{
	INITIALIZE_ASSERT
	layouts.push_back(layout);
	HWND dlgitem = GetDlgItem(dialog_wnd, dlgitemid);
	SetWindowLongPtr(dlgitem, GWLP_USERDATA, (LONG_PTR)&layouts.back());
}

static void EnforceMinimumSize(RECT& new_rect, const SIZE& min_size, WPARAM wparam)
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

void LayoutHandler::OnSizing(RECT& new_rect, WPARAM wparam)
{
	EnforceMinimumSize(new_rect, dialog_size_min, wparam);
}

void LayoutHandler::OnSize(int width, int height)
{
	auto offset = SIZE{width - dialog_size.cx, height - dialog_size.cy};

	if(offset.cx == 0 && offset.cy == 0)
		return;

	dialog_size.cx = width;
	dialog_size.cy = height;

	UserData userdata(this, &offset);
	EnumChildWindows(dialog_wnd, EnumChildProc, (LPARAM)&userdata);
	InvalidateRect(dialog_wnd, NULL, FALSE);
}

BOOL CALLBACK LayoutHandler::EnumChildProc(HWND hwnd, LPARAM lparam)
{
	auto userdata = (UserData*)lparam;
	auto layout = (const Layout*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	//if(layout)
	userdata->first->MoveChild(hwnd, *layout, *userdata->second);
	return TRUE;
}

void LayoutHandler::MoveChild(HWND child, const Layout& layout, const SIZE& offset)
{
	RECT rect;
	GetWindowRect(child, &rect);

	OffsetRect(&rect, offset.cx, offset.cy);
	int x, y, w, h;
	RectToPoint(rect, x, y, w, h);

	POINT p{x, y};
	ScreenToClient(dialog_wnd, &p);
	MoveWindow(child, p.x, p.y, w, h, TRUE);
}
