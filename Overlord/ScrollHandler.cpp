#include "ScrollHandler.hpp"
#include <exception>

static void AdjustForNewSize(int view_size, int old_size, int new_size, int& scroll_pos)
{
	if(old_size == 0)
		return;

	auto ratio = (double)new_size / old_size;
	scroll_pos = int(scroll_pos * ratio);

	auto diff = new_size - view_size;
	if(scroll_pos > diff)
		scroll_pos = diff;
}

void ScrollBarHandler::Setup(HWND parent_window, int view_width, int view_height, int content_width, int content_height)
{
	AdjustForNewSize(view_width, _content_width, content_width, _scroll_pos_x);
	AdjustForNewSize(view_height, _content_height, content_height, _scroll_pos_y);

	_parent_window = parent_window;
	_view_width = view_width;
	_view_height = view_height;
	_content_width = content_width;
	_content_height = content_height;
	_h_scroll_enabled = SetupSingle(_scroll_pos_x, view_width, content_width, SB_HORZ);
	_v_scroll_enabled = SetupSingle(_scroll_pos_y, view_height, content_height, SB_VERT);
}

void ScrollBarHandler::HandleScroll(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg == WM_HSCROLL)
		HandleScroll(wParam, lParam, SB_HORZ, _scroll_pos_x, _view_width, _content_width);
	else if(msg == WM_VSCROLL)
		HandleScroll(wParam, lParam, SB_VERT, _scroll_pos_y, _view_height, _content_height);
}

bool ScrollBarHandler::OffsetScroll(int sox, int soy)
{
	bool scrolled = false;

	if(sox != 0 && _h_scroll_enabled) {
		OffsetScroll(SB_HORZ, sox, _scroll_pos_x, _content_width - _view_width);
		scrolled = true;
	}

	if(soy != 0 && _v_scroll_enabled) {
		OffsetScroll(SB_VERT, soy, _scroll_pos_y, _content_height - _view_height);
		scrolled = true;
	}

	return scrolled;
}

bool ScrollBarHandler::SetupSingle(int scroll_pos, int view_size, int content_size, int sb_hv)
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
		SetScrollInfo(_parent_window, sb_hv, &si, TRUE);
		EnableScrollBar(_parent_window, sb_hv, ESB_ENABLE_BOTH);
		return true;
	}

	EnableScrollBar(_parent_window, sb_hv, ESB_DISABLE_BOTH);
	return false;
}

void ScrollBarHandler::HandleScroll(WPARAM wParam, LPARAM lParam, int sb_hv, int& sref, int view_size, int content_size)
{
	if(!_parent_window)
		throw new std::exception("_parent_window not initialized, call Setup first.");

	auto diff = content_size - view_size;

	switch(LOWORD(wParam)) {
		case SB_LEFT:
			sref = 0;
			SetScrollPos(_parent_window, sb_hv, sref, TRUE);
			break;
		case SB_RIGHT:
			sref = content_size;
			SetScrollPos(_parent_window, sb_hv, sref, TRUE);
			break;
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
			sref = HIWORD(wParam);
			SetScrollPos(_parent_window, sb_hv, sref, TRUE);
			break;
		case SB_LINELEFT:
			--sref;
			if(sref < 0)
				sref = 0;
			SetScrollPos(_parent_window, sb_hv, sref, TRUE);
			break;
		case SB_LINERIGHT:
			++sref;
			if(sref > diff)
				sref = diff;
			SetScrollPos(_parent_window, sb_hv, sref, TRUE);
			break;
		case SB_PAGELEFT:
			sref -= view_size;
			if(sref < 0)
				sref = 0;
			SetScrollPos(_parent_window, sb_hv, sref, TRUE);
			break;
		case SB_PAGERIGHT:
			sref += view_size;
			if(sref > diff)
				sref = diff;
			SetScrollPos(_parent_window, sb_hv, sref, TRUE);
			break;
	}
}

void ScrollBarHandler::OffsetScroll(int sb_hv, int offset, int& sref, int max)
{
	sref += offset;

	if(sref < 0)
		sref = 0;
	else if(sref > max)
		sref = max;

	SetScrollPos(_parent_window, sb_hv, sref, TRUE);
}
