#include "CaptureDialog.hpp"
#include "resource.h"
#include <sstream>

static void InitDialog(HWND hDialogWnd, const CaptureSample* capture_sample)
{
	auto title = capture_sample->capture_source.window_title.c_str();

	std::wostringstream woss;
	auto r = capture_sample->capture_source.source_rect;
	woss << L'(' << r.left << L',' << r.top << L"), " << L'(' << r.right << L',' << r.bottom << L')';
	auto position = woss.str();

	SetDlgItemText(hDialogWnd, IDC_EDIT_TITLE, title);
	SetDlgItemText(hDialogWnd, IDC_EDIT_POSITION, position.c_str());
}

static void DrawSampleImage(const CaptureSample* capture_sample, const DRAWITEMSTRUCT* dis)
{
	if(dis->itemAction != ODA_DRAWENTIRE)
		return;

	auto image = capture_sample->image;
	auto hdcSrc = image->GetDC();
	auto w = image->GetWidth();
	auto h = image->GetHeight();

	BitBlt(dis->hDC, 0, 0, w, h, hdcSrc, 0, 0, SRCCOPY);
}

static void SetDialogData(HWND hDialogWnd, LPARAM lParam)
{
	SetWindowLongPtr(hDialogWnd, GWLP_USERDATA, (LONG_PTR)lParam);
}

static const CaptureSample* GetDialogData(HWND hDialogWnd)
{
	return (const CaptureSample*)GetWindowLongPtr(hDialogWnd, GWLP_USERDATA);
}

static INT_PTR DlgProc(HWND hDialogWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) {
		case WM_INITDIALOG:
			SetDialogData(hDialogWnd, lParam);
			InitDialog(hDialogWnd, (const CaptureSample*)lParam);
			return true;

		case WM_DRAWITEM:
			if(wParam == IDC_STATIC_SAMPLE) {
				DrawSampleImage(GetDialogData(hDialogWnd), (const DRAWITEMSTRUCT*)lParam);
				return true;
			}
			break;

		case WM_CLOSE:
			EndDialog(hDialogWnd, IDCANCEL);
			return true;

		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDOK:
				case IDCANCEL:
					EndDialog(hDialogWnd, LOWORD(wParam));
					return true;
			}
	}

	return false;
}

bool CaptureDialog::ShowDialog(const CaptureSample& capture_sample)
{
	return IDOK == DialogBoxParam(GetModuleHandle(0), MAKEINTRESOURCE(IDD_CAPTURE_SAMPLE), HWND_DESKTOP, DlgProc, reinterpret_cast<LPARAM>(&capture_sample));
}
