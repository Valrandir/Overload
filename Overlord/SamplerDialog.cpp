#include "SamplerDialog.hpp"
#include "Overlord.rc.h"

static void OnHotKeyCaptureInitial(int, void* userdata);
static void OnHotKeyCaptureNext(int, void* userdata);

bool SamplerDialog::ShowDialog()
{
	SamplerDialog dialog;
	return dialog.ShowModal(IDD_DIALOG_SAMPLER);
}

void SamplerDialog::Initialize()
{
	HICON icon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCEW(IDI_ICON1));
	SendMessageW(dialog_wnd, WM_SETICON, ICON_BIG, LPARAM(icon));

	InitializeHotKeys();
}

void SamplerDialog::InitializeHotKeys()
{
	hotkey_monitor.SetReceiverWindow(dialog_wnd);
	hotkey_monitor.AddHotKey(OnHotKeyCaptureInitial, nullptr, MOD_CONTROL | MOD_SHIFT, 'H');
	hotkey_monitor.AddHotKey(OnHotKeyCaptureNext, nullptr, MOD_CONTROL | MOD_SHIFT, 'K');
}

INT_PTR SamplerDialog::DlgProc(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg) {
		case WM_HOTKEY:
			hotkey_monitor.OnHotKey(LOWORD(wparam));
			return true;
		case WM_COMMAND:
			switch(LOWORD(wparam)) {
				case IDOK:
					EndDialog(dialog_wnd, DIALOG_SUCCESS);
					return true;
				case IDCANCEL:
					EndDialog(dialog_wnd, DIALOG_CANCEL);
					return true;
			}
	}

	return DialogBase::DlgProc(dialog_wnd, msg, wparam, lparam);
}

static void OnHotKeyCaptureInitial(int, void* userdata)
{
	MessageBox(0, TEXT("OnHotKeyCaptureInitial"), TEXT("HotKey"), MB_OK);
}

static void OnHotKeyCaptureNext(int, void* userdata)
{
	MessageBox(0, TEXT("OnHotKeyCaptureNext"), TEXT("HotKey"), MB_OK);
}
