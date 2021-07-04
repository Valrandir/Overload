#include "DemoDialog.hpp"
#include "Overlord.rc.h"

bool DemoDialog::ShowDialog()
{
	DemoDialog dialog;
	return dialog.ShowModal(IDD_DIALOG_SAMPLER);
}
DemoUserControl d2;

void DemoDialog::Initialize()
{
	SetIcon(IDI_ICON1);

	demo_user_control.Initialize(dialog_wnd, 32, 32);
	d2.Initialize(dialog_wnd, 400, 100);
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
