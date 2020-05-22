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
	GetDlgItemPoint(_hDialogWnd, IDC_STATIC_SAMPLE, x, y, w, h);
	_image_view.Initialize(_hDialogWnd, x, y, w, h, _image);

	if(!_capture_source)
		return;

	auto title = _capture_source->window_title.c_str();

	std::wostringstream woss;
	auto r = _capture_source->source_rect;
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
				DrawSampleImage(_image, (const DRAWITEMSTRUCT*)lParam);
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
