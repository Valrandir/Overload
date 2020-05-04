#include "CaptureDialog.hpp"
#include "resource.h"
#include <sstream>

bool CaptureDialog::ShowDialog(const CaptureSample* capture_sample)
{
	CaptureDialog capture_dialog(capture_sample);
	return capture_dialog.ShowModal(IDD_CAPTURE_SAMPLE);
}

void CaptureDialog::Initialize()
{
	auto title = _capture_sample->window_title.c_str();

	std::wostringstream woss;
	auto r = _capture_sample->source_rect;
	woss << L'(' << r.left << L',' << r.top << L"), " << L'(' << r.right << L',' << r.bottom << L')';
	auto position = woss.str();

	SetDlgItemText(_hDialogWnd, IDC_EDIT_TITLE, title);
	SetDlgItemText(_hDialogWnd, IDC_EDIT_POSITION, position.c_str());
}

INT_PTR CaptureDialog::DlgProc(HWND hDialogWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) {
		case WM_DRAWITEM:
			if(wParam == IDC_STATIC_SAMPLE) {
				DrawSampleImage((const DRAWITEMSTRUCT*)lParam);
				return true;
			}
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDOK:
					EndDialog(hDialogWnd, DIALOG_SUCCESS);
					return true;
				case IDCANCEL:
					EndDialog(hDialogWnd, DIALOG_CANCEL);
					return true;
			}
	}

	return DialogBase::DlgProc(hDialogWnd, msg, wParam, lParam);
}

void CaptureDialog::DrawSampleImage(const DRAWITEMSTRUCT* dis)
{
	if(dis->itemAction != ODA_DRAWENTIRE)
		return;

	auto image = _capture_sample->image;
	if(image) {
		auto hdcSrc = image->GetDC();
		auto w = image->GetWidth();
		auto h = image->GetHeight();

		BitBlt(dis->hDC, 0, 0, w, h, hdcSrc, 0, 0, SRCCOPY);
	}
}
