#include "ScrollHandler.hpp"
#include <exception>

static void UpdateContentSizeHV(LONG _view_size, LONG old_size, LONG new_size, LONG& scroll_pos);
static bool SetupSingleHV(HWND window, int scroll_pos, int _view_size, int content_size, int sb_hv);
static void HandleScrollHV(HWND window, WPARAM wParam, LPARAM lParam, int sb_hv, LONG& sref, LONG _view_size, LONG content_size);
static void OffsetScrollHV(HWND window, int sb_hv, LONG offset, LONG& sref, LONG max);

void ScrollBarHandler::Initialize(HWND parent_window, int _view_width, int _view_height, int content_width, int content_height)
{
	UpdateContentSizeHV(_view_width, _content.cx, content_width, _position.x);
	UpdateContentSizeHV(_view_height, _content.cy, content_height, _position.y);

	_parent_window = parent_window;
	_view.cx = _view_width;
	_view.cy = _view_height;
	_content.cx = content_width;
	_content.cy = content_height;
	_h_scroll_enabled = SetupSingleHV(parent_window, _position.x, _view_width, content_width, SB_HORZ);
	_v_scroll_enabled = SetupSingleHV(parent_window, _position.y, _view_height, content_height, SB_VERT);
}

void ScrollBarHandler::HandleScroll(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg == WM_HSCROLL)
		HandleScrollHV(_parent_window, wParam, lParam, SB_HORZ, _position.x, _view.cx, _content.cx);
	else if(msg == WM_VSCROLL)
		HandleScrollHV(_parent_window, wParam, lParam, SB_VERT, _position.y, _view.cy, _content.cy);
}

bool ScrollBarHandler::OffsetScroll(int sox, int soy)
{
	bool scrolled = false;

	if(sox != 0 && _h_scroll_enabled) {
		OffsetScrollHV(_parent_window, SB_HORZ, sox, _position.x, _content.cx - _view.cx);
		scrolled = true;
	}

	if(soy != 0 && _v_scroll_enabled) {
		OffsetScrollHV(_parent_window, SB_VERT, soy, _position.y, _content.cy - _view.cy);
		scrolled = true;
	}

	return scrolled;
}

static void UpdateContentSizeHV(LONG _view_size, LONG old_size, LONG new_size, LONG& scroll_pos)
{
	if(old_size == 0)
		return;

	auto ratio = (double)new_size / old_size;
	scroll_pos = int(scroll_pos * ratio);

	auto diff = new_size - _view_size;
	if(scroll_pos > diff)
		scroll_pos = diff;
}

static bool SetupSingleHV(HWND window, int scroll_pos, int _view_size, int content_size, int sb_hv)
{
	auto diff = content_size - _view_size;
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

static void HandleScrollHV(HWND window, WPARAM wParam, LPARAM lParam, int sb_hv, LONG& sref, LONG _view_size, LONG content_size)
{
	auto diff = content_size - _view_size;

	switch(LOWORD(wParam)) {
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
			sref = HIWORD(wParam);
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
			sref -= _view_size;
			if(sref < 0)
				sref = 0;
			SetScrollPos(window, sb_hv, sref, TRUE);
			break;
		case SB_PAGERIGHT:
			sref += _view_size;
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
