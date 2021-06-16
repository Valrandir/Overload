#include "CaptureDialog.hpp"
#include "CaptureSample.hpp"
#include "CaptureWnd.hpp"
#include "HotkeyMonitor.hpp"
#include "KeyOut.hpp"
#include "WindowGdi.hpp"

void OnHotKeyInitialCapture(int, void* userdata)
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

void OnHotKeyFlask(int, void* userdata)
{
	std::vector<UINT> keys {'Z', '1', '2', '3', '4', '5'};
	KeyOut::SendKeyInRandomOrder(keys);
	HotkeyMonitor::Flush();
}

void Console(const char* cmd)
{
	KeyOut::SendKey(VK_RETURN);
	KeyOut::SendText(cmd);
	KeyOut::SendKey(VK_RETURN);
}

void OnHotKeyExit(int, void* userdata)
{
	Console("/exit");
}

void OnHotKeyHideout(int, void* userdata)
{
	Console("/hideout");
}

int ScenarioPathOfExile()
{
	CaptureSample cs;

	HotkeyMonitor hkm;
	hkm.AddHotKey(OnHotKeyInitialCapture, &cs, 0, VK_NUMPAD5);
	hkm.AddHotKey(OnHotKeyFlask, &cs, 0, 'Z');
	hkm.AddHotKey(OnHotKeyExit, &cs, 0, VK_F4);
	hkm.AddHotKey(OnHotKeyHideout, &cs, 0, VK_F5);

	WindowGdi window(TEXT("Overlord - Path of Exile Scenario"), 640, 480);
	window.Show();

	while(window.Update()) {
		hkm.Dispatch();
		Sleep(1);
	}

	return 0;
}
