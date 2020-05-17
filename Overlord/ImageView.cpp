#include "ImageView.hpp"
#include <cassert>
#include <exception>

void ImageView::Initialize(HWND hWndParent, int x, int y, int width, int height)
{
	const LPCTSTR CLASS_NAME = TEXT("ImageView");
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASSEX wc;

	_width = width;
	_height = height;

	if(!GetClassInfoEx(hInstance, CLASS_NAME, &wc)) {
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = WndProcStatic;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_HIGHLIGHT + 1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = CLASS_NAME;
		wc.hIconSm = NULL;

		RegisterClassEx(&wc);
	}

	DWORD style = WS_CHILDWINDOW | WS_HSCROLL | WS_VSCROLL | WS_VISIBLE;
	_hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, CLASS_NAME, TEXT(""), style, x, y, width, height, hWndParent, NULL, hInstance, NULL);
	SetWindowLongPtr(_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	HDC hDC = ::GetDC(_hWnd);
	_hDC = CreateCompatibleDC(hDC);
	_hBitmap = CreateCompatibleBitmap(hDC, _width, _height);
	ReleaseDC(_hWnd, hDC);
	SelectObject(_hDC, _hBitmap);
	RECT rect{0, 0, _width, _height};
	FillRect(_hDC, &rect, (HBRUSH)LTGRAY_BRUSH);
}

ImageView::~ImageView()
{
	DeleteDC(_hDC);
	DeleteObject(_hBitmap);

	if(_image) {
		delete _image;
		_image = nullptr;
	}
}

void ImageView::Show()
{
	ShowWindow(_hWnd, SW_SHOW);
}

bool ImageView::Update()
{
	MSG msg;

	while(!_destroyed && PeekMessage(&msg, _hWnd, 0, 0, PM_REMOVE))
		DispatchMessage(&msg);

	return !_destroyed;
}

void ImageView::Close()
{
	_destroyed = true;
	DestroyWindow(_hWnd);
}

void ImageView::SetImage(const Image* image)
{
	DrawImage(image, 0, 0);
	_image = image;
	SetupScrollInfo(image);
}

void ImageView::ScrollImage(int offset_x, int offset_y)
{
	if(_sbh.OffsetScroll(offset_x, offset_y)) {
		auto sp = _sbh.GetPosition();
		DrawImage(_image, -sp.x, -sp.y);
	}
}

void ImageView::SetScrollCallback(ScrollCallbackFunc callback, void* userdata)
{
	_scroll_cb_func = callback;
	_scroll_cb_userdata = userdata;
}

void ImageView::SetupScrollInfo(const Image* image)
{
	assert(image);
	if(!image)
		throw new std::exception("image is null");

	_sbh.Setup(_hWnd, _width, _height, image->GetWidth(), image->GetHeight());
}

void ImageView::DrawImage(const Image* image, int x, int y)
{
	if(!image)
		return;

	int w = image->GetWidth();
	int h = image->GetHeight();
	RECT rect{x, y, x + w, y + h};
	BitBlt(_hDC, x, y, w, h, image->GetDC(), 0, 0, SRCCOPY);
	InvalidateRect(_hWnd, NULL, FALSE);
}

#ifdef IMAGE_VIEW_DEBUG
#include <sstream>
#endif

static void DrawScrollInfo(HDC dc, int sx, int sy)
{
#ifdef IMAGE_VIEW_DEBUG
	std::wostringstream woss;
	woss << L"Scroll X: " << sx << L"    " << L"Scroll Y: " << sy;
	auto text = woss.str();
	TextOut(dc, 16, 16, text.c_str(), text.length());
#endif
}

void ImageView::OnPaint()
{
	PAINTSTRUCT ps;
	BeginPaint(_hWnd, &ps);
	int x = ps.rcPaint.left;
	int y = ps.rcPaint.top;
	int w = ps.rcPaint.right - x;
	int h = ps.rcPaint.bottom - y;
	BitBlt(ps.hdc, x, y, w, h, _hDC, x, y, SRCCOPY);

	auto sp = _sbh.GetPosition();
	DrawScrollInfo(ps.hdc, sp.x, sp.y);

	EndPaint(_hWnd, &ps);
}

void ImageView::OnScroll(UINT msg, WPARAM wParam, LPARAM lParam)
{
	_sbh.HandleScroll(msg, wParam, lParam);
	auto sp = _sbh.GetPosition();
	DrawImage(_image, -sp.x, -sp.y);
}

void ImageView::OnLMouseMove()
{
	if(!_mouse_dragging)
		return;

	POINT screen_pos;
	GetCursorPos(&screen_pos);

	auto offset_x = _mouse_origin.x - screen_pos.x;
	auto offset_y = _mouse_origin.y - screen_pos.y;
	_mouse_origin.x = screen_pos.x;
	_mouse_origin.y = screen_pos.y;

	ScrollImage(offset_x, offset_y);
	if(_scroll_cb_func)
		_scroll_cb_func(offset_x, offset_y, _scroll_cb_userdata);
}

void ImageView::OnLMouseDown()
{
	_mouse_dragging = true;

	POINT screen_pos;
	GetCursorPos(&screen_pos);

	_mouse_origin.x = screen_pos.x;
	_mouse_origin.y = screen_pos.y;

	SetCapture(_hWnd);
}

void ImageView::OnLMouseUp()
{
	ReleaseCapture();
}

LRESULT CALLBACK ImageView::WndProcStatic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ImageView* wb;

	if(msg != WM_NCCREATE && (wb = reinterpret_cast<ImageView*>(GetWindowLongPtr(hWnd, GWLP_USERDATA))))
		return wb->WndProc(msg, wParam, lParam);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT ImageView::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) {
		case WM_PAINT:
			OnPaint();
			return 0;
		case WM_MOUSEMOVE:
			if(wParam & MK_LBUTTON) {
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
		case WM_HSCROLL:
		case WM_VSCROLL:
			OnScroll(msg, wParam, lParam);
			return 0;
		case WM_CAPTURECHANGED:
			_mouse_dragging = false;
			return 0;
		case WM_DESTROY:
			Close();
			return 0;
	}

	return DefWindowProc(_hWnd, msg, wParam, lParam);
}
