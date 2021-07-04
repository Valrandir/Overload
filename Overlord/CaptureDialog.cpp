#include "CaptureDialog.hpp"
#include "Overlord.rc.h"
#include <sstream>

bool CaptureDialog::ShowDialog(const BitmapGdi* bitmap_gdi, const CaptureSource* capture_source)
{
	CaptureDialog capture_dialog(bitmap_gdi, capture_source);
	return capture_dialog.ShowModal(IDD_CAPTURE_SAMPLE);
}

bool CaptureDialog::ShowDialog(const CaptureSample* capture_sample)
{
	return ShowDialog(capture_sample->bitmap_gdi, capture_sample);
}

void CaptureDialog::Initialize()
{
	HWND placeholder = GetDlgItem(dialog_wnd, IDC_STATIC_SAMPLE);
	imageview.Initialize(dialog_wnd, placeholder, bitmap_gdi);

	if(!capture_source)
		return;

	auto title = capture_source->window_title.c_str();

	std::wstringstream wss;
	auto r = capture_source->source_rect;
	wss << L'(' << r.left << L',' << r.top << L"), " << L'(' << r.right << L',' << r.bottom << L')';
	auto position = wss.str();

	SetDlgItemText(dialog_wnd, IDC_EDIT_TITLE, title);
	SetDlgItemText(dialog_wnd, IDC_EDIT_POSITION, position.c_str());
}

INT_PTR CaptureDialog::DlgProc(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam)
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
