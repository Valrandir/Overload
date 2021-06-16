#include "CaptureWnd.hpp"
#include "ImageCapture.hpp"
#include <utility>

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

static POINT GetRectCenterPoint(const RECT& rect)
{
	auto w = rect.right - rect.left;
	auto h = rect.bottom - rect.top;
	return POINT{rect.right - w / 2, rect.bottom - h / 2};
}

static HWND GetWindowFromRect(const RECT& rect)
{
	HWND hWnd = WindowFromPoint(GetRectCenterPoint(rect));

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

LRESULT CaptureWnd::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg) {
		case WM_MOUSEMOVE: OnMouseMove(LOWORD(lparam), HIWORD(lparam)); break;
		case WM_LBUTTONDOWN: OnMouseDown(LOWORD(lparam), HIWORD(lparam)); break;
		case WM_LBUTTONUP: OnMouseUp(LOWORD(lparam), HIWORD(lparam)); break;
	}

	return WindowGdi::WndProc(msg, wparam, lparam);
}

void CaptureWnd::OnMouseMove(int x, int y)
{
	if(!selecting)
		return;

	RECT rect = sel_rect;

	DrawFocusRect(dc, &sel_rect);
	DragRect(sel_rect, origin, x, y);
	DrawFocusRect(dc, &sel_rect);
	MergeRect(rect, sel_rect);
	InvalidateRect(window, &rect, FALSE);
}

void CaptureWnd::OnMouseDown(int x, int y)
{
	selecting = true;
	origin.x = x;
	origin.y = y;

	sel_rect.left = x;
	sel_rect.top = y;
	sel_rect.right = x;
	sel_rect.bottom = y;

	DrawFocusRect(dc, &sel_rect);
	InvalidateRect(window, &sel_rect, FALSE);
}

void CaptureWnd::OnMouseUp(int x, int y)
{
	selecting = false;

	ShrinkRect(sel_rect);

	if(RectSizeIsNotZero(sel_rect)) {
		captured_image = new BitmapGdi(ImageCapture::CaptureWindow(window, sel_rect));
		Close();
	}
}

CaptureWnd::CaptureWnd() :
	WindowGdi(TEXT(""), 0, 0, WS_POPUP, WS_EX_TOPMOST)
{
	BitmapGdi desktop_bitmap = ImageCapture::CaptureDesktop();
	DrawBitmap(desktop_bitmap, 0, 0);
	SetCursor(IDC_CROSS);
	Show();
}

CaptureWnd::~CaptureWnd() {}

BitmapGdi* CaptureWnd::Capture(CaptureSource* out_capture_source)
{
	CaptureWnd capture_wnd;

	while(capture_wnd.Update())
		Sleep(1);

	if(out_capture_source) {
		out_capture_source->source_rect = capture_wnd.sel_rect;
		out_capture_source->window_handle = GetWindowFromRect(capture_wnd.sel_rect);
		out_capture_source->window_title = GetWindowText(out_capture_source->window_handle);
	}

	return capture_wnd.captured_image;
}

BitmapGdi CaptureWnd::Recapture(const CaptureSource& capture_source)
{
	//SetActiveWindow(capture_source.window_handle);
	SetForegroundWindow(capture_source.window_handle);
	return ImageCapture::CaptureDesktop(capture_source.source_rect);
}
