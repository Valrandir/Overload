#include "SamplerDialog.hpp"
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
}

INT_PTR SamplerDialog::DlgProc(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg) {
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
