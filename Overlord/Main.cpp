#include "CaptureDialog.hpp"
#include "CaptureWnd.hpp"
#include "CompareDialog.hpp"
#include "HotKeyMonitor.hpp"
#include "WindowGdi.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

void OnHotKeyCapture(int, void* userdata)
{
	CaptureSample* cs = (CaptureSample*)userdata;

	*cs = CaptureWnd::Capture();
	//if(!CaptureDialog::ShowDialog(cs))
	//	cs->image = nullptr;
}

void OnHotKeyCompare(int, void* userdata)
{
	CaptureSample* cs = (CaptureSample*)userdata;

	if(!cs->image)
		return;

	auto image_l = cs->image;

	*cs = CaptureWnd::Recapture(*cs);
	//if(!CaptureDialog::ShowDialog(cs))
	//	return;

	auto image_r = cs->image;

	CompareDialog::ShowDialog(image_l, image_r);
}
#include <shellapi.h>

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
