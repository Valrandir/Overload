#include "ImageView.hpp"
#include <cassert>
#include <exception>

void ImageView::Initialize(HWND hWndParent, int x, int y, int width, int height, const Image* image)
{
	const LPCTSTR CLASS_NAME = TEXT("ImageView");
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASSEX wc;

	this->width = width;
	this->height = height;

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
	window = CreateWindowEx(WS_EX_CLIENTEDGE, CLASS_NAME, TEXT(""), style, x, y, width, height, hWndParent, NULL, hInstance, NULL);
	SetWindowLongPtr(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	HDC hDC = ::GetDC(window);
	dc = CreateCompatibleDC(hDC);
	bitmap = CreateCompatibleBitmap(hDC, width, height);
	ReleaseDC(window, hDC);
	SelectObject(dc, bitmap);

	this->image = image;
	ClearBackground();
	DrawImage(image, 0, 0);
	SetupScrollInfo(image);
}

ImageView::~ImageView()
{
	if(dc) {
		DeleteDC(dc);
		dc = 0;
	}

	if(bitmap) {
		DeleteObject(bitmap);
		bitmap = 0;
	}

	image = nullptr;
}

void ImageView::Show()
{
	ShowWindow(window, SW_SHOW);
}

bool ImageView::Update()
{
	MSG msg;

	while(!destroyed && PeekMessage(&msg, window, 0, 0, PM_REMOVE))
		DispatchMessage(&msg);

	return !destroyed;
}

void ImageView::Close()
{
	destroyed = true;
	DestroyWindow(window);
}

void ImageView::Scroll(int offset_x, int offset_y)
{
	if(sbh.OffsetScroll(offset_x, offset_y)) {
		auto sp = sbh.GetPosition();
		DrawImage(image, -sp.x, -sp.y);
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

void ImageView::SetupScrollInfo(const Image* image)
{
	assert(image);
	if(!image)
		throw new std::exception("image is null");

	sbh.Initialize(window, width, height, image->GetWidth(), image->GetHeight());
}

void ImageView::ClearBackground()
{
	RECT rect{0, 0, width, height};
	FillRect(dc, &rect, (HBRUSH)LTGRAY_BRUSH);
}

void ImageView::DrawImage(const Image* image, int x, int y)
{
	if(!image)
		return;

	int w = image->GetWidth();
	int h = image->GetHeight();
	RECT rect{x, y, x + w, y + h};

	if(zoom_level > 1) {
		auto zx = zoom_factor * image->GetWidth();
		auto zy = zoom_factor * image->GetHeight();
		StretchBlt(dc, x, y, zx, zy, image->GetDC(), 0, 0, image->GetWidth(), image->GetHeight(), SRCCOPY);
	} else
		BitBlt(dc, x, y, w, h, image->GetDC(), 0, 0, SRCCOPY);

	InvalidateRect(window, NULL, FALSE);
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
	DrawImage(image, -sp.x, -sp.y);
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
	sbh.Initialize(window, width, height, image->GetWidth() * zoom_factor, image->GetHeight() * zoom_factor);
	auto sp = sbh.GetPosition();
	ClearBackground();
	DrawImage(image, -sp.x, -sp.y);
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
