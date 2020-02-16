#include "CaptureWnd.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

void LoadImageTest()
{
	Image* image = Image::LoadFile("C:\\Users\\Valrandir\\Desktop\\out\\out.png");
	WindowGdi window(TEXT("Overlord"), image->GetWidth(), image->GetHeight(), WS_OVERLAPPEDWINDOW | WS_VISIBLE);
	window.DrawImage(image, 0, 0);
	delete image;

	while(window.Update())
		Sleep(1);
}

void SaveImageTest()
{
	Image* image = CaptureWnd::Capture();
	if(!image)
		return;

	WindowGdi window(TEXT("Overlord"), 800, 600, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
	window.DrawImage(image, 0, 0);
	image->SaveToFile("C:\\Users\\Valrandir\\Desktop\\out\\out.png");
	delete image;

	while(window.Update())
		Sleep(1);
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ INT nShowCmd)
{
	SaveImageTest();
	LoadImageTest();
	return 0;
}
