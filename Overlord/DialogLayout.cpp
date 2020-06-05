#include "DialogLayout.hpp"
#include "RectUtility.hpp"
#include <cassert>
#include <vector>

#define INITIALIZE_ASSERT assert(dialog_wnd);
using UserData = std::pair<DialogLayout*, SIZE*>;
static std::vector<DialogLayout::Layout> ReadAfxDialogLayout(UINT dialog_resource_id);
static void EnforceMinimumSize(RECT& new_rect, const SIZE& min_size, WPARAM wparam);

/// <summary>
/// Return the rectangle of the specified child window, relative to the specified parent window.
/// </summary>
static RECT GetChildWindowRect(HWND parent_wnd, HWND child_wnd)
{
	RECT rect;
	GetWindowRect(child_wnd, &rect);
	POINT p{rect.left, rect.top};
	ScreenToClient(parent_wnd, &p);
	OffsetRect(&rect, p.x - rect.left, p.y - rect.top);
	return rect;
}

void DialogLayout::Initialize(UINT dialog_resource_id, HWND dialog_wnd)
{
	RECT rect;
	GetWindowRect(dialog_wnd, &rect);
	this->dialog_size_min = SIZE{rect.right - rect.left, rect.bottom - rect.top};
	GetClientRect(dialog_wnd, &rect);
	this->dialog_size_initial = SIZE{rect.right - rect.left, rect.bottom - rect.top};
	this->dialog_wnd = dialog_wnd;

	auto layouts = ReadAfxDialogLayout(dialog_resource_id);
	if(layouts.size() == 0)
		return;

	auto child_wnd = GetWindow(dialog_wnd, GW_CHILD);

	for(auto& layout : layouts) {
		assert(child_wnd && "AfxDialogLayout count mismatch");
		layout.Initial = GetChildWindowRect(dialog_wnd, child_wnd);
		AddLayout(layout, child_wnd);
		child_wnd = GetWindow(child_wnd, GW_HWNDNEXT);
	}

	assert(!child_wnd && "AfxDialogLayout count mismatch");
}

void DialogLayout::AddLayout(Layout layout, HWND child_wnd)
{
	INITIALIZE_ASSERT
	layouts.push_back(layout);
	size_t index = layouts.size() - 1;
	SetWindowLongPtr(child_wnd, GWLP_USERDATA, (LONG_PTR)index);
}

void DialogLayout::OnSizing(RECT& new_rect, WPARAM wparam)
{
	EnforceMinimumSize(new_rect, dialog_size_min, wparam);
}

#include <sstream>

void DialogLayout::OnSize(int width, int height)
{
	auto offset = SIZE{width - dialog_size_initial.cx, height - dialog_size_initial.cy};

	UserData userdata(this, &offset);
	EnumChildWindows(dialog_wnd, EnumChildProc, (LPARAM)&userdata);
}

BOOL CALLBACK DialogLayout::EnumChildProc(HWND hwnd, LPARAM lparam)
{
	auto userdata = (UserData*)lparam;
	auto self = userdata->first;
	auto offset = userdata->second;
	auto index = (size_t)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	if(index < self->layouts.size())
		self->LayoutChild(hwnd, self->layouts[index], *offset);

	return TRUE;
}

void DialogLayout::LayoutChild(HWND child_wnd, const Layout& layout, const SIZE& offset)
{
	RECT old_rect = GetChildWindowRect(dialog_wnd, child_wnd);
	InvalidateRect(dialog_wnd, &old_rect, FALSE);

	SIZE initial_size{layout.Initial.right - layout.Initial.left, layout.Initial.bottom - layout.Initial.top};
	int mx = layout.Initial.left + (layout.Move.Horizontal() ? int(offset.cx * layout.Move.x_ratio) : 0);
	int my = layout.Initial.top + (layout.Move.Vertical() ? int(offset.cy * layout.Move.y_ratio) : 0);
	int sx = layout.Size.Horizontal() ? initial_size.cx + int(offset.cx * layout.Size.x_ratio) : initial_size.cx;
	int sy = layout.Size.Vertical() ? initial_size.cy + int(offset.cy * layout.Size.y_ratio) : initial_size.cy;
	MoveWindow(child_wnd, mx, my, sx, sy, TRUE);
}

std::vector<DialogLayout::Layout> ReadAfxDialogLayout(UINT dialog_resource_id)
{
	HRSRC res_info = FindResource(NULL, MAKEINTRESOURCE(dialog_resource_id), TEXT("AFX_DIALOG_LAYOUT"));
	DWORD res_size = SizeofResource(NULL, res_info);
	HGLOBAL res_data = LoadResource(NULL, res_info);
	void* res_ptr = LockResource(res_data);

	std::vector<DialogLayout::Layout> layouts;

	auto it = (unsigned short*)res_ptr;
	auto end = (unsigned short*)(((unsigned char*)res_ptr) + res_size);

	auto version = *it++;
	if(version == 0) {
		while(it + 4 <= end) {
			DialogLayout::Layout layout;
			layout.Move.x_ratio = *it++ / 100.f;
			layout.Move.y_ratio = *it++ / 100.f;
			layout.Size.x_ratio = *it++ / 100.f;
			layout.Size.y_ratio = *it++ / 100.f;
			layouts.push_back(layout);
		}
	}

	UnlockResource(res_data);
	FreeResource(res_data);

	return layouts;
}

static void EnforceMinimumSize(RECT& new_rect, const SIZE& min_size, WPARAM wparam)
{
	int w = new_rect.right - new_rect.left;
	int h = new_rect.bottom - new_rect.top;
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
