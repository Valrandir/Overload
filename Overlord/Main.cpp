#include "CaptureWnd.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

void LoadImageTest()
{
	Image* image = Image::LoadFile("C:\\Users\\Valrandir\\Desktop\\out\\out.png");
	WindowGdi window(TEXT("Loaded image"), image->GetWidth(), image->GetHeight(), WS_OVERLAPPEDWINDOW | WS_VISIBLE);
	window.DrawImage(image, 0, 0);
	delete image;

	while(window.Update())
		Sleep(1);
}

void SaveImageTest()
{
	Image* image = CaptureWnd::Capture().image;

	int count;
	auto pixels = image->GetPixels(&count);
	if(!image)
		return;

	WindowGdi window(TEXT("Captured image"), 800, 600, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
	window.DrawImage(image, 0, 0);
	image->SaveToFile("C:\\Users\\Valrandir\\Desktop\\out\\out.png");
	delete image;

	while(window.Update())
		Sleep(1);
}

void ImageTest()
{
	SaveImageTest();
	LoadImageTest();
}

#include "CaptureDialog.hpp"
#include "CompareDialog.hpp"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ INT nShowCmd)
{
	CaptureSample cs1, cs2;
	cs1 = CaptureWnd::Capture();
	bool result = CaptureDialog::ShowDialog(&cs1);

	if(result) {
		cs2 = CaptureWnd::Recapture(cs1);
		result = CaptureDialog::ShowDialog(&cs2);
	}

	if(result) {
		CompareDialog::ShowDialog(cs1.image, cs2.image);
	}

	return 0;
}
