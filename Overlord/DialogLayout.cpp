#include "DialogLayout.hpp"
#include "RectUtility.hpp"
#include <cassert>
#include <vector>

#define INITIALIZE_ASSERT assert(dialog_wnd);
using UserData = std::pair<DialogLayout*, SIZE*>;
static std::vector<DialogLayout::Layout> ReadAfxDialogLayout(UINT dialog_resource_id);
static void EnforceMinimumSize(RECT& new_rect, const SIZE& min_size, WPARAM wparam);

void DialogLayout::Initialize(UINT dialog_resource_id, HWND dialog_wnd)
{
	RECT rect;
	GetWindowRect(dialog_wnd, &rect);
	this->dialog_size_min = SIZE{rect.right - rect.left, rect.bottom - rect.top};
	GetClientRect(dialog_wnd, &rect);
	this->dialog_size = SIZE{rect.right - rect.left, rect.bottom - rect.top};
	this->dialog_wnd = dialog_wnd;

	auto layouts = ReadAfxDialogLayout(dialog_resource_id);
	if(layouts.size() == 0)
		return;

	auto child_wnd = GetWindow(dialog_wnd, GW_CHILD);

	for(auto& layout : layouts) {
		assert(child_wnd && "AfxDialogLayout count mismatch");
		GetWindowRect(child_wnd, &layout.Initial);
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

void DialogLayout::OnSize(int width, int height)
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

void DialogLayout::LayoutChild(HWND child, const Layout& layout, const SIZE& offset)
{
	RECT rect;
	GetWindowRect(child, &rect);

	int ox = rect.left - layout.Initial.left + offset.cx;
	int oy = rect.top - layout.Initial.top + offset.cy;
	int ow = rect.right - rect.left - (layout.Initial.right - layout.Initial.left) + offset.cx;
	int oh = rect.bottom - rect.top - (layout.Initial.bottom - layout.Initial.top) + offset.cy;

	rect = layout.Initial;

	auto mx = int(ox * (layout.Move.x_ratio / 100.0f));
	auto my = int(oy * (layout.Move.y_ratio / 100.0f));
	OffsetRect(&rect, mx, my);

	auto sx = int(ow * (layout.Size.x_ratio / 100.0f));
	//auto sx = ow;
	auto sy = int(oh * (layout.Size.y_ratio / 100.0f));
	rect.right += sx;
	rect.bottom += sy;

	int x, y, w, h;
	RectToPoint(rect, x, y, w, h);
	POINT p{x, y};
	ScreenToClient(dialog_wnd, &p);
	MoveWindow(child, p.x, p.y, w, h, TRUE);
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
			layout.Move.x_ratio = *it++;
			layout.Move.y_ratio = *it++;
			layout.Size.x_ratio = *it++;
			layout.Size.y_ratio = *it++;
			layouts.push_back(layout);
		}
	}

	UnlockResource(res_data);
	FreeResource(res_data);

	return layouts;
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
