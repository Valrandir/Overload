#include "CaptureDialog.hpp"
#include "CaptureSample.hpp"
#include "CaptureWnd.hpp"
#include "CompareDialog.hpp"
#include "HotkeyMonitor.hpp"
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

int ScenarioCaptureCompare()
{
	CaptureSample cs;

	HotkeyMonitor hkm;
	hkm.AddHotKey(OnHotKeyCapture, &cs, 0, VK_NUMPAD4);
	hkm.AddHotKey(OnHotKeyCompare, &cs, 0, VK_NUMPAD6);

	WindowGdi window(TEXT("Overlord"), 640, 480);
	window.Show();

	while(window.Update()) {
		hkm.Dispatch();
		Sleep(1);
	}

	return 0;
}
