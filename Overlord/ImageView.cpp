#include "ImageView.hpp"
#include <cassert>
#include <exception>
#include "DialogBase.hpp"
#include "RectUtility.hpp"

void ImageView::Initialize(HWND parent_window, HWND placeholder, const BitmapGdi* bitmap_gdi)
{
	const LPCTSTR CLASS_NAME = TEXT("ImageView");
	HINSTANCE instance = GetModuleHandle(NULL);
	WNDCLASSEX wc;

	RECT placeholder_rect = DialogBase::GetDlgItemRect(placeholder);
	auto pos = RectToPoint(placeholder_rect);
	auto size = RectToSize(placeholder_rect);

	width = size.cx;
	height = size.cy;
	this->bitmap_gdi = bitmap_gdi;
	this->placeholder = placeholder;

	if(!GetClassInfoEx(instance, CLASS_NAME, &wc)) {
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = WndProcStatic;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = instance;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_HIGHLIGHT + 1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = CLASS_NAME;
		wc.hIconSm = NULL;

		RegisterClassEx(&wc);
	}

	DWORD style = WS_CHILDWINDOW | WS_HSCROLL | WS_VSCROLL | WS_VISIBLE;
	window = CreateWindowEx(WS_EX_CLIENTEDGE, CLASS_NAME, TEXT(""), style, pos.x, pos.y, width, height, parent_window, NULL, instance, NULL);
	SetWindowLongPtr(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	BitmapGdi::Initialize(width, height);
	ClearBackground();
	DrawImage(*bitmap_gdi, 0, 0);
	SetupScrollInfo(*bitmap_gdi);
}

ImageView::~ImageView()
{
	Close();
	bitmap_gdi = nullptr;
}

void ImageView::Show()
{
	ShowWindow(window, SW_SHOW);
}

void ImageView::UpdateLayout()
{
	RECT rect = DialogBase::GetDlgItemRect(placeholder);
	POINT p = RectToPoint(rect);
	SIZE s = RectToSize(rect);
	BitmapGdi::Initialize(s.cx, s.cy);
	MoveWindow(window, p.x, p.y, s.cx, s.cy, TRUE);

	Scroll(0, 0);
	sbh.Initialize(window, s.cx, s.cy, bitmap_gdi->Width() * zoom_factor, bitmap_gdi->Height() * zoom_factor);
	auto sp = sbh.GetPosition();
	ClearBackground();
	DrawImage(*bitmap_gdi, 0, 0);
}

void ImageView::Close()
{
	if(!destroyed) {
		destroyed = true;
		DestroyWindow(window);
	}
}

void ImageView::Scroll(int offset_x, int offset_y)
{
	if(sbh.OffsetScroll(offset_x, offset_y)) {
		auto sp = sbh.GetPosition();
		DrawImage(*bitmap_gdi, -sp.x, -sp.y);
	}
}

void ImageView::ZoomIn()
{
	UpdateZoom(1);
}

void ImageView::ZoomOut()
{
	UpdateZoom(-1);
}

void ImageView::SetupScrollInfo(const BitmapGdi& bitmap_gdi)
{
	sbh.Initialize(window, width, height, bitmap_gdi.Width(), bitmap_gdi.Height());
}

void ImageView::ClearBackground()
{
	RECT rect{0, 0, width, height};
	FillRect(dc, &rect, (HBRUSH)LTGRAY_BRUSH);
}

void ImageView::DrawImage(const BitmapGdi& bitmap_gdi, int x, int y)
{
	int w = bitmap_gdi.Width();
	int h = bitmap_gdi.Height();
	RECT rect{x, y, x + w, y + h};

	if(zoom_level > 1) {
		auto zx = zoom_factor * bitmap_gdi.Width();
		auto zy = zoom_factor * bitmap_gdi.Height();
		StretchBlt(dc, x, y, zx, zy, bitmap_gdi.GetDC(), 0, 0, bitmap_gdi.Width(), bitmap_gdi.Height(), SRCCOPY);
	} else
		BitBlt(dc, x, y, w, h, bitmap_gdi.GetDC(), 0, 0, SRCCOPY);

	InvalidateRect(window, NULL, FALSE);
}

#ifdef IMAGE_VIEW_DEBUG
#include <sstream>

static void DrawScrollInfo(HDC dc, int sx, int sy)
{
	std::wostringstream woss;
	woss << L"Scroll X: " << sx << L"    " << L"Scroll Y: " << sy;
	auto text = woss.str();
	TextOut(dc, 16, 16, text.c_str(), text.length());
}
#else
#define DrawScrollInfo
#endif

void ImageView::OnPaint()
{
	PAINTSTRUCT ps;
	BeginPaint(window, &ps);
	int x = ps.rcPaint.left;
	int y = ps.rcPaint.top;
	int w = ps.rcPaint.right - x;
	int h = ps.rcPaint.bottom - y;
	BitBlt(ps.hdc, x, y, w, h, dc, x, y, SRCCOPY);

	auto sp = sbh.GetPosition();
	DrawScrollInfo(ps.hdc, sp.x, sp.y);

	EndPaint(window, &ps);
}

void ImageView::OnScroll(UINT msg, WPARAM wparam, LPARAM lparam)
{
	sbh.HandleScroll(msg, wparam, lparam);
	auto sp = sbh.GetPosition();
	DrawImage(*bitmap_gdi, -sp.x, -sp.y);
}

void ImageView::OnLMouseMove()
{
	if(!mouse_dragging)
		return;

	POINT screen_pos;
	GetCursorPos(&screen_pos);

	auto offset_x = mouse_origin.x - screen_pos.x;
	auto offset_y = mouse_origin.y - screen_pos.y;
	mouse_origin.x = screen_pos.x;
	mouse_origin.y = screen_pos.y;

	Scroll(offset_x, offset_y);
	ScrollEvent(offset_x, offset_y);
}

void ImageView::OnLMouseDown()
{
	mouse_dragging = true;

	POINT screen_pos;
	GetCursorPos(&screen_pos);

	mouse_origin.x = screen_pos.x;
	mouse_origin.y = screen_pos.y;

	SetCapture(window);
}

void ImageView::OnLMouseUp()
{
	ReleaseCapture();
}

void ImageView::OnZoom(int direction)
{
	UpdateZoom(direction);
	ZoomEvent(direction == -1);
}

static int GetZoomFactor(int zoom_level)
{
	int zoom_factor = 1;

	for(int i = 0; i < zoom_level - 1; ++i)
		zoom_factor *= 2;

	return zoom_factor;
}

void ImageView::UpdateZoom(int direction)
{
	zoom_level += direction;

	if(zoom_level < 0)
		zoom_level = 0;

	if(zoom_level > ZOOM_LEVEL_MAX)
		zoom_level = ZOOM_LEVEL_MAX;

	zoom_factor = GetZoomFactor(zoom_level);
	sbh.Initialize(window, width, height, bitmap_gdi->Width() * zoom_factor, bitmap_gdi->Height() * zoom_factor);
	auto sp = sbh.GetPosition();
	ClearBackground();
	DrawImage(*bitmap_gdi, -sp.x, -sp.y);
}

LRESULT CALLBACK ImageView::WndProcStatic(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	ImageView* wb;

	if(msg != WM_NCCREATE && (wb = reinterpret_cast<ImageView*>(GetWindowLongPtr(hWnd, GWLP_USERDATA))))
		return wb->WndProc(msg, wparam, lparam);

	return DefWindowProc(hWnd, msg, wparam, lparam);
}

LRESULT ImageView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg) {
		case WM_PAINT:
			OnPaint();
			return 0;
		case WM_MOUSEMOVE:
			if(wparam & MK_LBUTTON) {
				OnLMouseMove();
				return 0;
			}
			break;
		case WM_LBUTTONDOWN:
			OnLMouseDown();
			return 0;
		case WM_LBUTTONUP:
			OnLMouseUp();
			return 0;
		case WM_MOUSEWHEEL:
			if((short)HIWORD(wparam) > 0)
				OnZoom(-1);
			else
				OnZoom(1);
			return 0;
		case WM_HSCROLL:
		case WM_VSCROLL:
			OnScroll(msg, wparam, lparam);
			return 0;
		case WM_CAPTURECHANGED:
			mouse_dragging = false;
			return 0;
		case WM_DESTROY:
			Close();
			return 0;
	}

	return DefWindowProc(window, msg, wparam, lparam);
}
