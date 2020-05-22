#include "CaptureDialog.hpp"
#include "CaptureSample.hpp"
#include "CaptureWnd.hpp"
#include "CompareDialog.hpp"
#include "HotKeyMonitor.hpp"
#include "WindowGdi.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

void OnHotKeyCapture(int, void* userdata)
{
	auto cs = (CaptureSample*)userdata;

	if(cs->image) {
		Image::FreeImage(cs->image);
		cs->image = nullptr;
	}

	cs->image = CaptureWnd::Capture(cs);

	if(!CaptureDialog::ShowDialog(cs)) {
		Image::FreeImage(cs->image);
		cs->image = nullptr;
	}
}

void OnHotKeyCompare(int, void* userdata)
{
	auto cs = (CaptureSample*)userdata;

	if(!cs->image)
		return;

	auto image_l = cs->image;

	cs->image = CaptureWnd::Recapture(*cs);
	if(!CaptureDialog::ShowDialog(cs)) {
		Image::FreeImage(image_l);
		return;
	}

	auto image_r = cs->image;

	CompareDialog::ShowDialog(image_l, image_r);

	Image::FreeImage(image_l);
	Image::FreeImage(image_r);
}

int main()
{
	CaptureSample cs;

	HotKeyMonitor hkm;
	hkm.AddHotKey(OnHotKeyCapture, &cs, VK_NUMPAD4);
	hkm.AddHotKey(OnHotKeyCompare, &cs, VK_NUMPAD6);

	WindowGdi window(TEXT("Overlord"), 640, 480);
	window.Show();

	while(window.Update()) {
		hkm.Dispatch();
		Sleep(1);
	}

	return 0;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ INT nShowCmd)
{
	try {
		return main();
	} catch(std::exception& ex) {
		MessageBoxA(0, ex.what(), "Critical Exception", MB_OK | MB_ICONERROR);
	}

	return 1;
}
