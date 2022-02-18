#include "WindowGdi.hpp"

void WindowGdi::AdjustAndCenter(int& x, int& y, int& width, int& height, DWORD style, DWORD ex_style)
{
	auto ss = GetScreenSize();

	if(width == 0 && height == 0) {
		this->width = width = ss.cx;
		this->height = height = ss.cy;
	}

	RECT rect{0, 0, width, height};
	AdjustWindowRectEx(&rect, style, FALSE, ex_style);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
	x = (ss.cx - width) >> 1;
	y = (ss.cy - height) >> 1;
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
			if(OnKeyDown(wparam))
				return 0;
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

WindowGdi::WindowGdi(LPCTSTR title, int width, int height, DWORD style, DWORD ex_style)
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

	this->width = width;
	this->height = height;

	int x, y;
	style = (style ? style : WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX) | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_EX_CONTROLPARENT;
	AdjustAndCenter(x, y, width, height, style, ex_style);
	window = CreateWindowEx(ex_style, CLASS_NAME, title, style, x, y, width, height, HWND_DESKTOP, NULL, hInstance, NULL);
	SetWindowLongPtr(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	Initialize(this->width, this->height);
	Clear(RGB(0x3f, 0x3f, 0x3f));
}

WindowGdi::~WindowGdi()
{
}

void WindowGdi::SetTitle(LPCTSTR title)
{
	SetWindowText(window, title);
}

void WindowGdi::SetStyle(DWORD style, DWORD ex_style)
{
	style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	SetWindowLongPtr(window, GWL_STYLE, style);
	int x, y, w = width, h = height;
	AdjustAndCenter(x, y, w, h, style, ex_style);
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

	while(!destroyed && PeekMessage(&msg, window, 0, 0, PM_REMOVE)) {
		if(IsDialogMessage(msg.hwnd, &msg))
			continue;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return !destroyed;
}

bool WindowGdi::UpdateLoop()
{
	MSG msg;

	while(!destroyed && GetMessage(&msg, 0, 0, 0)) {
		if(IsDialogMessage(msg.hwnd, &msg))
			continue;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return !destroyed;
}

void WindowGdi::Close()
{
	destroyed = true;
	DestroyWindow(window);
}

bool WindowGdi::OnKeyDown(WPARAM wparam)
{
	if(LOWORD(wparam) == VK_ESCAPE) {
		Close();
		return true;
	}

	return false;
}

void WindowGdi::DrawBitmap(const BitmapGdi& source, int x, int y)
{
	int w = source.Width();
	int h = source.Height();

	Draw(source.GetDC(), x, y, w, h);

	RECT rect{x, y, x + w, y + h};
	InvalidateRect(window, &rect, FALSE);
}

SIZE GetScreenSize()
{
	return SIZE{GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};
}
