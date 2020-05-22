#include "CaptureWnd.hpp"

static void DragRect(RECT& rect, const POINT origin, int x, int y)
{
	if(x < origin.x)
		rect.left = x;
	else
		rect.right = x;

	if(y < origin.y)
		rect.top = y;
	else
		rect.bottom = y;
}

static void MergeRect(RECT& dest, const RECT& src)
{
	if(dest.left > src.left)
		dest.left = src.left;

	if(dest.right < src.right)
		dest.right = src.right;

	if(dest.top > src.top)
		dest.top = src.top;

	if(dest.bottom < src.bottom)
		dest.bottom = src.bottom;
}

static void ShrinkRect(RECT& rect)
{
	SIZE focusBorder;
	SystemParametersInfo(SPI_GETFOCUSBORDERWIDTH, 0, &focusBorder.cx, 0);
	SystemParametersInfo(SPI_GETFOCUSBORDERHEIGHT, 0, &focusBorder.cy, 0);
	rect.left += focusBorder.cx;
	rect.right -= (focusBorder.cx + focusBorder.cx);
	rect.top += focusBorder.cy;
	rect.bottom -= (focusBorder.cy + focusBorder.cy);
}

static bool RectSizeIsNotZero(const RECT& rect)
{
	return rect.left < rect.right && rect.top < rect.bottom;
}

static HWND GetWindowFromRect(const RECT& rect)
{
	POINT{rect.left, rect.top};
	HWND hWnd = WindowFromPoint(POINT{rect.left, rect.top});

	if(!hWnd)
		throw new std::exception("WindowFromPoint failed");

	return hWnd;
}

static std::wstring GetWindowText(HWND hWnd)
{
	int cb_size = GetWindowTextLength(hWnd) * sizeof(wchar_t);
	wchar_t* buffer = new wchar_t[cb_size];
	GetWindowText(hWnd, buffer, cb_size);
	return std::wstring(buffer);
}

LRESULT CaptureWnd::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) {
		case WM_MOUSEMOVE: OnMouseMove(LOWORD(lParam), HIWORD(lParam)); break;
		case WM_LBUTTONDOWN: OnMouseDown(LOWORD(lParam), HIWORD(lParam)); break;
		case WM_LBUTTONUP: OnMouseUp(LOWORD(lParam), HIWORD(lParam)); break;
	}

	return WindowGdi::WndProc(msg, wParam, lParam);
}

void CaptureWnd::OnMouseMove(int x, int y)
{
	if(!_selecting)
		return;

	RECT rect = _sel_rect;

	DrawFocusRect(_hDC, &_sel_rect);
	DragRect(_sel_rect, _origin, x, y);
	DrawFocusRect(_hDC, &_sel_rect);
	MergeRect(rect, _sel_rect);
	InvalidateRect(_hWnd, &rect, FALSE);
}

void CaptureWnd::OnMouseDown(int x, int y)
{
	_selecting = true;
	_origin.x = x;
	_origin.y = y;

	_sel_rect.left = x;
	_sel_rect.top = y;
	_sel_rect.right = x;
	_sel_rect.bottom = y;

	DrawFocusRect(_hDC, &_sel_rect);
	InvalidateRect(_hWnd, &_sel_rect, FALSE);
}

void CaptureWnd::OnMouseUp(int x, int y)
{
	_selecting = false;

	ShrinkRect(_sel_rect);

	if(RectSizeIsNotZero(_sel_rect)) {
		_capturedImage = Image::Capture(_hWnd, &_sel_rect);
		Close();
	}
}

CaptureWnd::CaptureWnd() :
	WindowGdi(TEXT(""), 0, 0, WS_POPUP)
{
	Image* desktop_image = Image::CaptureDesktop();
	DrawImage(desktop_image, 0, 0);
	delete desktop_image;
	SetCursor(IDC_CROSS);
	Show();
}

CaptureWnd::~CaptureWnd() {}

Image* CaptureWnd::Capture(CaptureSource* out_capture_source)
{
	CaptureWnd capture_wnd;

	while(capture_wnd.Update())
		Sleep(1);

	if(out_capture_source) {
		out_capture_source->source_rect = capture_wnd._sel_rect;
		out_capture_source->window_handle = GetWindowFromRect(capture_wnd._sel_rect);
		out_capture_source->window_title = GetWindowText(out_capture_source->window_handle);
	}

	return capture_wnd._capturedImage;
}

Image* CaptureWnd::Recapture(const CaptureSource& capture_source)
{
	//SetActiveWindow(capture_source.window_handle);
	SetForegroundWindow(capture_source.window_handle);
	return Image::Capture(HWND_DESKTOP, &capture_source.source_rect);
}
