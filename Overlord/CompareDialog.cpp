#include "CompareDialog.hpp"
#include "resource.h"
#include <cassert>

static void ScrollCallback(int offset_x, int offset_y, void* userdata)
{
	auto linked_image_view = (ImageView*)userdata;
	linked_image_view->Scroll(offset_x, offset_y);
}

static void ZoomCallback(bool is_zoom_out, void* userdata)
{
	auto linked_image_view = (ImageView*)userdata;

	if(is_zoom_out)
		linked_image_view->ZoomOut();
	else
		linked_image_view->ZoomIn();
}

bool CompareDialog::ShowDialog(const Image* img_l, const Image* img_r)
{
	assert(img_l);
	assert(img_r);

	CompareDialog compare_dialog(img_l, img_r);
	return compare_dialog.ShowModal(IDD_DIALOG_COMPARE);
}

void CompareDialog::Initialize()
{
	int x, y, w, h;

	GetDlgItemPoint(_hDialogWnd, IDC_STATIC_IMG_1, x, y, w, h);
	_image_view_l.Initialize(_hDialogWnd, x, y, w, h, _img_l);
	_image_view_l.ScrollEvent.SetHandler(ScrollCallback, &_image_view_r);
	_image_view_l.ZoomEvent.SetHandler(ZoomCallback, &_image_view_r);

	GetDlgItemPoint(_hDialogWnd, IDC_STATIC_IMG_2, x, y, w, h);
	_image_view_r.Initialize(_hDialogWnd, x, y, w, h, _img_r);
	_image_view_r.ScrollEvent.SetHandler(ScrollCallback, &_image_view_l);
	_image_view_r.ZoomEvent.SetHandler(ZoomCallback, &_image_view_l);
}

INT_PTR CompareDialog::DlgProc(HWND hDialogWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) {
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDCOMPARE:
					OnCompare();
					return true;
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

#include "CaptureDialog.hpp"
#include <cstdlib>
#include <exception>
#include <sstream>

void CompareDialog::OnCompare()
{
	Image::Pixel *img_l_begin, *img_r_begin;
	auto img_l_pixel_count = _img_l->GetPixels(img_l_begin);
	auto img_r_pixel_count = _img_r->GetPixels(img_r_begin);
	auto img_l_it = img_l_begin;
	auto img_r_it = img_r_begin;

	if(img_l_pixel_count != img_r_pixel_count)
		throw new std::exception("pixel_count differ between both images");

	auto comp_result_pixels = new Image::Pixel[img_l_pixel_count];
	auto comp_it = comp_result_pixels;

	int max_diff = 0;
	int avg_diff = 0;
	auto comp = [&](BYTE& c, BYTE& l, BYTE& r) -> void {
		c = abs(l - r);
		if(max_diff < c)
			max_diff = c;
		avg_diff += c;
	};

	for(size_t i = 0; i < img_l_pixel_count; ++i, ++img_l_it, ++img_r_it, ++comp_it) {
		comp(comp_it->r, img_l_it->r, img_r_it->r);
		comp(comp_it->g, img_l_it->g, img_r_it->g);
		comp(comp_it->b, img_l_it->b, img_r_it->b);
		comp_it->a = 0xff;
	}

	avg_diff /= (int)img_l_pixel_count;

	Image::FreePixels(img_l_begin);
	Image::FreePixels(img_r_begin);

	auto comp_img = Image::CreateFromPixels(comp_result_pixels, _img_l->GetWidth(), _img_l->GetHeight());

	CaptureDialog::ShowDialog(comp_img);

	delete[] comp_result_pixels;

	std::wstringstream wss;
	wss << L"max_diff = " << max_diff << std::endl << L"avg_diff = " << avg_diff;
	MessageBox(_hDialogWnd, wss.str().c_str(), TEXT("Compare"), MB_OK | MB_ICONINFORMATION);
}
