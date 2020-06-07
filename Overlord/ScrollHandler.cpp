#include "ScrollHandler.hpp"
#include <exception>

static void UpdateContentSizeHV(LONG view_size, LONG old_size, LONG new_size, LONG& scroll_pos);
static bool SetupSingleHV(HWND window, int scroll_pos, int view_size, int content_size, int sb_hv);
static void HandleScrollHV(HWND window, WPARAM wparam, LPARAM lparam, int sb_hv, LONG& sref, LONG view_size, LONG content_size);
static void OffsetScrollHV(HWND window, int sb_hv, LONG offset, LONG& sref, LONG max);

void ScrollBarHandler::Initialize(HWND parent_window, int view_width, int view_height, int content_width, int content_height)
{
	UpdateContentSizeHV(view_width, content.cx, content_width, position.x);
	UpdateContentSizeHV(view_height, content.cy, content_height, position.y);

	this->parent_window = parent_window;
	view.cx = view_width;
	view.cy = view_height;
	content.cx = content_width;
	content.cy = content_height;
	h_scroll_enabled = SetupSingleHV(parent_window, position.x, view_width, content_width, SB_HORZ);
	v_scroll_enabled = SetupSingleHV(parent_window, position.y, view_height, content_height, SB_VERT);
}

void ScrollBarHandler::HandleScroll(UINT msg, WPARAM wparam, LPARAM lparam)
{
	if(msg == WM_HSCROLL)
		HandleScrollHV(parent_window, wparam, lparam, SB_HORZ, position.x, view.cx, content.cx);
	else if(msg == WM_VSCROLL)
		HandleScrollHV(parent_window, wparam, lparam, SB_VERT, position.y, view.cy, content.cy);
}

bool ScrollBarHandler::OffsetScroll(int sox, int soy)
{
	bool scrolled = false;

	if(sox != 0 && h_scroll_enabled) {
		OffsetScrollHV(parent_window, SB_HORZ, sox, position.x, content.cx - view.cx);
		scrolled = true;
	}

	if(soy != 0 && v_scroll_enabled) {
		OffsetScrollHV(parent_window, SB_VERT, soy, position.y, content.cy - view.cy);
		scrolled = true;
	}

	return scrolled;
}

static void UpdateContentSizeHV(LONG view_size, LONG old_size, LONG new_size, LONG& scroll_pos)
{
	if(old_size == 0)
		return;

	auto ratio = (double)new_size / old_size;
	scroll_pos = int(scroll_pos * ratio);

	auto diff = new_size - view_size;
	if(scroll_pos > diff)
		scroll_pos = diff;
}

static bool SetupSingleHV(HWND window, int scroll_pos, int view_size, int content_size, int sb_hv)
{
	auto diff = content_size - view_size;
	if(diff >= 0) {
		SCROLLINFO si{};
		si.cbSize = sizeof(si);
		si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
		si.nMin = 0;
		si.nMax = diff;
		si.nPage = 1;
		si.nPos = scroll_pos;
		SetScrollInfo(window, sb_hv, &si, TRUE);
		EnableScrollBar(window, sb_hv, ESB_ENABLE_BOTH);
		return true;
	}

	EnableScrollBar(window, sb_hv, ESB_DISABLE_BOTH);
	return false;
}

static void HandleScrollHV(HWND window, WPARAM wparam, LPARAM lparam, int sb_hv, LONG& sref, LONG view_size, LONG content_size)
{
	auto diff = content_size - view_size;

	switch(LOWORD(wparam)) {
		case SB_LEFT:
			sref = 0;
			SetScrollPos(window, sb_hv, sref, TRUE);
			break;
		case SB_RIGHT:
			sref = content_size;
			SetScrollPos(window, sb_hv, sref, TRUE);
			break;
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
			sref = HIWORD(wparam);
			SetScrollPos(window, sb_hv, sref, TRUE);
			break;
		case SB_LINELEFT:
			--sref;
			if(sref < 0)
				sref = 0;
			SetScrollPos(window, sb_hv, sref, TRUE);
			break;
		case SB_LINERIGHT:
			++sref;
			if(sref > diff)
				sref = diff;
			SetScrollPos(window, sb_hv, sref, TRUE);
			break;
		case SB_PAGELEFT:
			sref -= view_size;
			if(sref < 0)
				sref = 0;
			SetScrollPos(window, sb_hv, sref, TRUE);
			break;
		case SB_PAGERIGHT:
			sref += view_size;
			if(sref > diff)
				sref = diff;
			SetScrollPos(window, sb_hv, sref, TRUE);
			break;
	}
}

static void OffsetScrollHV(HWND window, int sb_hv, LONG offset, LONG& sref, LONG max)
{
	sref += offset;

	if(sref < 0)
		sref = 0;
	else if(sref > max)
		sref = max;

	SetScrollPos(window, sb_hv, sref, TRUE);
}
