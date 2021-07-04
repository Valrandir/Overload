#include "DemoUserControl.hpp"

#include "DialogLayout.hpp"

void DemoUserControl::Initialize(HWND parent_window, int x, int y)
{
	UserControl::Initialize(parent_window, x, y, 300, 300);

	HWND hwnd_edit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT("Test"), WS_CHILD | WS_VISIBLE, 10, 10, 200, 24, window, 0, GetModuleHandle(0), 0);
}

LRESULT DemoUserControl::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg) {
	}

	return UserControl::WndProc(msg, wparam, lparam);
}
