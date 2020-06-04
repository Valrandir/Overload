#include "SamplerDialog.hpp"
#include "CaptureWnd.hpp"
#include "Overlord.rc.h"

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
	hotkey_monitor.AddHotKey(OnHotKeyCaptureInitial, this, MOD_CONTROL | MOD_SHIFT, 'H');
	hotkey_monitor.AddHotKey(OnHotKeyCaptureNext, this, MOD_CONTROL | MOD_SHIFT, 'K');
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

void SamplerDialog::CaptureRootElement()
{
	auto bitmap = CaptureWnd::Capture();
	if(!bitmap)
		return;

	SetActiveWindow(dialog_wnd);

	for(auto& e : elements)
		delete e;
	elements.clear();

	int x, y, w, h;
	GetDlgItemPoint(dialog_wnd, IDC_STATIC_IMG_1, x, y, w, h);
	root_element = new Element{std::move(*bitmap)};
	root_element->image_view.Initialize(dialog_wnd, x, y, w, h, &root_element->bitmap_gdi);

	elements.push_back(root_element);
}

void SamplerDialog::OnHotKeyCaptureInitial(int, void* userdata)
{
	auto& dialog = *(SamplerDialog*)userdata;
	dialog.CaptureRootElement();
}

void SamplerDialog::OnHotKeyCaptureNext(int, void* userdata)
{
	auto& dialog = *(SamplerDialog*)userdata;
}

SamplerDialog::~SamplerDialog()
{
	for(auto& e : elements)
		delete e;
}
