#include "WindowGdi.hpp"

void WindowGdi::AdjustAndCenter(int& x, int& y, int& width, int& height, DWORD style)
{
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);

	if(width == 0 && height == 0) {
		width = width = cx;
		height = height = cy;
	}

	RECT rect{0, 0, width, height};
	AdjustWindowRect(&rect, style, FALSE);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
	x = (cx - width) >> 1;
	y = (cy - height) >> 1;
}

void WindowGdi::OnPaint()
{
	PAINTSTRUCT ps;
	BeginPaint(window, &ps);
	int x = ps.rcPaint.left;
	int y = ps.rcPaint.top;
	int w = ps.rcPaint.right - x;
	int h = ps.rcPaint.bottom - y;
	BitBlt(ps.hdc, x, y, w, h, dc, x, y, SRCCOPY);
	EndPaint(window, &ps);
}

LRESULT CALLBACK WindowGdi::WndProcStatic(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	WindowGdi* wb;

	if(msg != WM_NCCREATE && (wb = reinterpret_cast<WindowGdi*>(GetWindowLongPtr(hWnd, GWLP_USERDATA))))
		return wb->WndProc(msg, wparam, lparam);

	return DefWindowProc(hWnd, msg, wparam, lparam);
}

LRESULT WindowGdi::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg) {
		case WM_SETCURSOR:
			if(!cursor)
				break;
			::SetCursor(cursor);
			return 0;
		case WM_KEYDOWN:
			if(LOWORD(wparam) == VK_ESCAPE) {
				Close();
				return 0;
			}
			break;
		case WM_PAINT:
			OnPaint();
			return 0;
		case WM_DESTROY:
			Close();
			return 0;
	}

	return DefWindowProc(window, msg, wparam, lparam);
}

WindowGdi::WindowGdi(LPCTSTR title, int width, int height, DWORD style) :
	width{width}, height{height}, destroyed{}
{
	const LPCTSTR CLASS_NAME = TEXT("WindowGdi");
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASSEX wc;

	if(!GetClassInfoEx(hInstance, CLASS_NAME, &wc)) {
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = WndProcStatic;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = CLASS_NAME;
		wc.hIconSm = NULL;

		RegisterClassEx(&wc);
	}

	int x, y;
	style = (style ? style : WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX) | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	AdjustAndCenter(x, y, width, height, style);
	window = CreateWindowEx(0, CLASS_NAME, title, style, x, y, width, height, HWND_DESKTOP, NULL, hInstance, NULL);
	SetWindowLongPtr(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	HDC hDC = ::GetDC(window);
	dc = CreateCompatibleDC(hDC);
	bitmap = CreateCompatibleBitmap(hDC, width, height);
	ReleaseDC(window, hDC);
	SelectObject(dc, bitmap);
	RECT rect{0, 0, width, height};
	FillRect(dc, &rect, (HBRUSH)DKGRAY_BRUSH);
}

WindowGdi::~WindowGdi()
{
	DeleteDC(dc);
	DeleteObject(bitmap);
}

void WindowGdi::SetTitle(LPCTSTR title)
{
	SetWindowText(window, title);
}

void WindowGdi::SetStyle(DWORD style)
{
	style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	SetWindowLongPtr(window, GWL_STYLE, style);
	int x, y, w = width, h = height;
	AdjustAndCenter(x, y, w, h, style);
	SetWindowPos(window, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
}

void WindowGdi::SetCursor(LPTSTR cursor)
{
	this->cursor = LoadCursor(0, cursor);
}

void WindowGdi::Show()
{
	ShowWindow(window, SW_SHOW);
}

void WindowGdi::Hide()
{
	ShowWindow(window, SW_HIDE);
}

bool WindowGdi::Update()
{
	MSG msg;

	while(!destroyed && PeekMessage(&msg, window, 0, 0, PM_REMOVE))
		DispatchMessage(&msg);

	return !destroyed;
}

void WindowGdi::Close()
{
	destroyed = true;
	DestroyWindow(window);
}

void WindowGdi::DrawImage(const Image* image, int x, int y)
{
	int w = image->GetWidth();
	int h = image->GetHeight();
	RECT rect{x, y, x + w, y + h};
	BitBlt(dc, x, y, w, h, image->GetDC(), 0, 0, SRCCOPY);
	InvalidateRect(window, &rect, FALSE);
}
