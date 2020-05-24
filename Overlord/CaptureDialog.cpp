#include "CaptureDialog.hpp"
#include "resource.h"
#include <sstream>

static void DrawSampleImage(const Image* image, const DRAWITEMSTRUCT* dis);

bool CaptureDialog::ShowDialog(const Image* image, const CaptureSource* capture_source)
{
	CaptureDialog capture_dialog(image, capture_source);
	return capture_dialog.ShowModal(IDD_CAPTURE_SAMPLE);
}

bool CaptureDialog::ShowDialog(const CaptureSample* capture_sample)
{
	return ShowDialog(capture_sample->image, capture_sample);
}

void CaptureDialog::Initialize()
{
	int x, y, w, h;
	GetDlgItemPoint(dialog_wnd, IDC_STATIC_SAMPLE, x, y, w, h);
	imageview.Initialize(dialog_wnd, x, y, w, h, image);

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
		case WM_DRAWITEM:
			if(wparam == IDC_STATIC_SAMPLE) {
				DrawSampleImage(image, (const DRAWITEMSTRUCT*)lparam);
				return true;
			}
			break;
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

static void DrawSampleImage(const Image* image, const DRAWITEMSTRUCT* dis)
{
	if(dis->itemAction != ODA_DRAWENTIRE)
		return;

	if(image) {
		auto hdcapture_sourcerc = image->GetDC();
		auto w = image->GetWidth();
		auto h = image->GetHeight();

		BitBlt(dis->hDC, 0, 0, w, h, hdcapture_sourcerc, 0, 0, SRCCOPY);
	}
}
