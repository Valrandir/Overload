#include "CaptureDialog.hpp"
#include "resource.h"
#include <sstream>

static CaptureSample g_capture_sample;

static void InitDialog(HWND hDialogWnd, const CaptureSample& capture_sample)
{
	g_capture_sample = capture_sample;
	SetWindowText(hDialogWnd, L"Capture Sample");

	auto title = capture_sample.capture_source.window_title.c_str();

	std::wostringstream woss;
	auto r = capture_sample.capture_source.source_rect;
	woss << L'(' << r.left << L',' << r.top << L"), " << L'(' << r.right << L',' << r.bottom << L')';
	auto position = woss.str();

	SetDlgItemText(hDialogWnd, IDC_EDIT_TITLE, title);
	SetDlgItemText(hDialogWnd, IDC_EDIT_POSITION, position.c_str());
}

static void DrawSampleImage(const DRAWITEMSTRUCT dis)
{
	if(dis.itemAction != ODA_DRAWENTIRE)
		return;

	auto image = g_capture_sample.image;
	auto hdcSrc = image->GetDC();
	auto w = image->GetWidth();
	auto h = image->GetHeight();

	BitBlt(dis.hDC, 0, 0, w, h, hdcSrc, 0, 0, SRCCOPY);
}

static INT_PTR DlgProc(HWND hDialogWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) {
		case WM_INITDIALOG:
			InitDialog(hDialogWnd, *(const CaptureSample*)lParam);
			return true;

		case WM_DRAWITEM:
			if(wParam == IDC_STATIC_SAMPLE) {
				DrawSampleImage(*(const DRAWITEMSTRUCT*)lParam);
				return true;
			}
			break;

		case WM_CLOSE:
			EndDialog(hDialogWnd, wParam);
			return true;

		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDOK:
				case IDCANCEL:
					EndDialog(hDialogWnd, wParam);
					return true;
			}
	}

	return false;
}

void CaptureDialog::ShowDialog(const CaptureSample& capture_sample)
{
	DialogBoxParam(GetModuleHandle(0), MAKEINTRESOURCE(IDD_CAPTURE_SAMPLE), HWND_DESKTOP, DlgProc, reinterpret_cast<LPARAM>(&capture_sample));
}
