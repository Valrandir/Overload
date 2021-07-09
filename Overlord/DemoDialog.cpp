#include "DemoDialog.hpp"
#include "Overlord.rc.h"

bool DemoDialog::ShowDialog()
{
	DemoDialog dialog;
	return dialog.ShowModal(IDD_DIALOG_SAMPLER);
}

void DemoDialog::Initialize()
{
	SetIcon(IDI_ICON1);

	int w{}, h{};
	int y = 5;

	for(auto& d : demo_user_control) {
		d.Initialize(dialog_wnd, 15, y += h + 10);
		d.GetSize(w, h);
	}
}

INT_PTR DemoDialog::DlgProc(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam)
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

	return Dialog::DlgProc(dialog_wnd, msg, wparam, lparam);
}

DemoDialog::~DemoDialog()
{
}
