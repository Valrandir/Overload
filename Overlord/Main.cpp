#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "WindowGdi.hpp"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	WindowGdi wnd(TEXT("Eve Valrandir"), 800, 600, WS_OVERLAPPEDWINDOW | WS_VISIBLE);

	//Image* img = Image::CreateBlank(200, 200, RGB(255, 255, 0));
	HWND hwnd = FindWindow(nullptr, TEXT("EVE"));
	ShowWindow(hwnd, SW_NORMAL);
	Sleep(500);
	Image* img = Image::Capture(hwnd, 0, 0, 200, 200);
	wnd.DrawImage(img, 50, 20);
	delete img;

	while(wnd.Update())
		Sleep(1);

	return 0;
}
