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

	if(cs->bitmap_gdi) {
		delete cs->bitmap_gdi;
		cs->bitmap_gdi = nullptr;
	}

	cs->bitmap_gdi = CaptureWnd::Capture(cs);

	if(!CaptureDialog::ShowDialog(cs)) {
		delete cs->bitmap_gdi;
		cs->bitmap_gdi = nullptr;
	}
}

void OnHotKeyCompare(int, void* userdata)
{
	auto cs = (CaptureSample*)userdata;

	if(!cs->bitmap_gdi)
		return;

	auto bitmal_l = cs->bitmap_gdi;

	cs->bitmap_gdi = new BitmapGdi(CaptureWnd::Recapture(*cs));
	if(!CaptureDialog::ShowDialog(cs)) {
		delete bitmal_l;
		cs->bitmap_gdi = nullptr;
		return;
	}

	auto bitmal_r = cs->bitmap_gdi;

	CompareDialog::ShowDialog(bitmal_l, bitmal_r);

	delete bitmal_l;
	delete bitmal_r;
	cs->bitmap_gdi = nullptr;
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
