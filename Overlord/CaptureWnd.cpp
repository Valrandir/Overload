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
	rect.right -= focusBorder.cx;
	rect.top += focusBorder.cy;
	rect.bottom -= focusBorder.cy;
}

static bool RectSizeIsNotZero(const RECT& rect)
{
	return rect.left < rect.right && rect.top < rect.bottom;
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

	RECT rect = _selRect;

	DrawFocusRect(_hDC, &_selRect);
	DragRect(_selRect, _origin, x, y);
	DrawFocusRect(_hDC, &_selRect);
	MergeRect(rect, _selRect);
	InvalidateRect(_hWnd, &rect, FALSE);
}

void CaptureWnd::OnMouseDown(int x, int y)
{
	_selecting = true;
	_origin.x = x;
	_origin.y = y;

	_selRect.left = x;
	_selRect.top = y;
	_selRect.right = x;
	_selRect.bottom = y;

	DrawFocusRect(_hDC, &_selRect);
	InvalidateRect(_hWnd, &_selRect, FALSE);
}

void CaptureWnd::OnMouseUp(int x, int y)
{
	_selecting = false;

	RECT shrinkRect = _selRect;
	ShrinkRect(shrinkRect);

	if(RectSizeIsNotZero(shrinkRect)) {
		_capturedImage = Image::Capture(_hWnd, &shrinkRect);
		Close();
	}
}

CaptureWnd::CaptureWnd()
	: WindowGdi(TEXT(""), 0, 0, WS_POPUP)
{
	Image* desktopImage = Image::CaptureDesktop();
	DrawImage(desktopImage, 0, 0);
	delete desktopImage;
	SetCursor(IDC_CROSS);
	Show();

	RECT rect {100, 100, 300, 300};
	FillRect(_hDC, &rect, CreateSolidBrush(RGB(128, 0, 0)));
	InvalidateRect(_hWnd, 0, FALSE);
}

CaptureWnd::~CaptureWnd() {}

Image* CaptureWnd::Capture()
{
	CaptureWnd captureWnd;

	while(captureWnd.Update())
		Sleep(1);

	return captureWnd._capturedImage;
}
