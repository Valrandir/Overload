#include "WindowGdi.hpp"

void WindowGdi::AdjustAndCenter(int& x, int& y, int& width, int& height, DWORD style)
{
	RECT rect{ 0, 0, width, height };
	AdjustWindowRect(&rect, style, FALSE);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
	x = (GetSystemMetrics(SM_CXSCREEN) - width) >> 1;
	y = (GetSystemMetrics(SM_CYSCREEN) - height) >> 1;
}

void WindowGdi::OnPaint()
{
	PAINTSTRUCT ps;
	BeginPaint(_hwnd, &ps);
	int x = ps.rcPaint.left;
	int y = ps.rcPaint.top;
	int w = ps.rcPaint.right - x;
	int h = ps.rcPaint.bottom - y;
	BitBlt(ps.hdc, x, y, w, h, _hDC, x, y, SRCCOPY);
	EndPaint(_hwnd, &ps);
}

LRESULT CALLBACK WindowGdi::WndProcStatic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WindowGdi* wb;

	if (msg != WM_NCCREATE && (wb = reinterpret_cast<WindowGdi*>(GetWindowLongPtr(hWnd, GWLP_USERDATA))))
		return wb->WndProc(msg, wParam, lParam);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WindowGdi::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (LOWORD(wParam) == VK_ESCAPE)
		{
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

	return DefWindowProc(_hwnd, msg, wParam, lParam);
}

WindowGdi::WindowGdi(LPCTSTR title, int width, int height, DWORD style) : _width{ width }, _height{ height }, _destroyed{}
{
	const LPCTSTR CLASS_NAME = TEXT("WindowGdi");
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASSEX wc;

	if (!GetClassInfoEx(hInstance, CLASS_NAME, &wc))
	{
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
	_hwnd = CreateWindowEx(0, CLASS_NAME, title, style, x, y, width, height, HWND_DESKTOP, NULL, hInstance, NULL);
	SetWindowLongPtr(_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	HDC hDC = ::GetDC(_hwnd);
	_hDC = CreateCompatibleDC(hDC);
	_hBitmap = CreateCompatibleBitmap(hDC, _width, _height);
	ReleaseDC(_hwnd, hDC);
	SelectObject(_hDC, _hBitmap);
	RECT rect{ 0, 0, _width, _height };
	FillRect(_hDC, &rect, (HBRUSH)DKGRAY_BRUSH);
}

WindowGdi::~WindowGdi()
{
	DeleteDC(_hDC);
	DeleteObject(_hBitmap);
}

void WindowGdi::SetTitle(LPCTSTR title)
{
	SetWindowText(_hwnd, title);
}

void WindowGdi::SetStyle(DWORD style)
{
	style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	SetWindowLongPtr(_hwnd, GWL_STYLE, style);
	int x, y, w = _width, h = _height;
	AdjustAndCenter(x, y, w, h, style);
	SetWindowPos(_hwnd, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
}

void WindowGdi::Show()
{
	ShowWindow(_hwnd, SW_SHOW);
}

void WindowGdi::Hide()
{
	ShowWindow(_hwnd, SW_HIDE);
}

bool WindowGdi::Update()
{
	MSG msg;

	while (!_destroyed && PeekMessage(&msg, _hwnd, 0, 0, PM_REMOVE))
		DispatchMessage(&msg);

	return !_destroyed;
}

void WindowGdi::Close()
{
	_destroyed = true;
	DestroyWindow(_hwnd);
}

void WindowGdi::DrawImage(const Image* image, int x, int y)
{
	int w = image->GetWidth();
	int h = image->GetHeight();
	RECT rect{ x, y, x + w, y + h };
	BitBlt(_hDC, x, y, w, h, image->GetDC(), 0, 0, SRCCOPY);
	InvalidateRect(_hwnd, &rect, FALSE);
}
