#include "UserControl.hpp"

LRESULT CALLBACK UserControl::WndProcStatic(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	UserControl* uc;

	if(msg != WM_NCCREATE && (uc = reinterpret_cast<UserControl*>(GetWindowLongPtr(hWnd, GWLP_USERDATA))))
		return uc->WndProc(msg, wparam, lparam);

	return DefWindowProc(hWnd, msg, wparam, lparam);
}

LRESULT UserControl::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg) {
		case WM_DESTROY:
			Close();
			return 0;
	}

	return DefWindowProc(window, msg, wparam, lparam);
}

void UserControl::Initialize(HWND parent_window, int x, int y, int width, int height)
{
	const LPCTSTR CLASS_NAME = TEXT("UserControl");
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASSEX wc;

	if(!GetClassInfoEx(hInstance, CLASS_NAME, &wc)) {
		wc.cbSize = sizeof(wc);
		wc.style = 0;
		wc.lpfnWndProc = WndProcStatic;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = 0;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		//wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
		wc.hbrBackground = CreateSolidBrush(RGB(128, 0, 0));
		wc.lpszMenuName = NULL;
		wc.lpszClassName = CLASS_NAME;
		wc.hIconSm = NULL;

		RegisterClassEx(&wc);
	}

	this->width = width;
	this->height = height;

	window = CreateWindowEx(0, CLASS_NAME, 0, WS_CHILDWINDOW | WS_VISIBLE, x, y, width, height, parent_window, NULL, hInstance, NULL);

	SetWindowLongPtr(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

UserControl::~UserControl()
{
}

bool UserControl::Update()
{
	MSG msg;

	while(!destroyed && PeekMessage(&msg, window, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return !destroyed;
}

void UserControl::Close()
{
	destroyed = true;
	DestroyWindow(window);
}
