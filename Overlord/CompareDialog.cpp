#include "CompareDialog.hpp"
#include "resource.h"
#include <cassert>

static void ScrollCallback(int offset_x, int offset_y, void* userdata)
{
	auto linkedimageview = (ImageView*)userdata;
	linkedimageview->Scroll(offset_x, offset_y);
}

static void ZoomCallback(bool is_zoom_out, void* userdata)
{
	auto linkedimageview = (ImageView*)userdata;

	if(is_zoom_out)
		linkedimageview->ZoomOut();
	else
		linkedimageview->ZoomIn();
}

bool CompareDialog::ShowDialog(const Image* image_a, const Image* image_b)
{
	assert(image_a);
	assert(image_b);

	CompareDialog compare_dialog(image_a, image_b);
	return compare_dialog.ShowModal(IDD_DIALOG_COMPARE);
}

void CompareDialog::Initialize()
{
	int x, y, w, h;

	GetDlgItemPoint(dialog_wnd, IDC_STATIC_IMG_1, x, y, w, h);
	imageview_a.Initialize(dialog_wnd, x, y, w, h, image_a);
	imageview_a.ScrollEvent.SetHandler(ScrollCallback, &imageview_b);
	imageview_a.ZoomEvent.SetHandler(ZoomCallback, &imageview_b);

	GetDlgItemPoint(dialog_wnd, IDC_STATIC_IMG_2, x, y, w, h);
	imageview_b.Initialize(dialog_wnd, x, y, w, h, image_b);
	imageview_b.ScrollEvent.SetHandler(ScrollCallback, &imageview_a);
	imageview_b.ZoomEvent.SetHandler(ZoomCallback, &imageview_a);
}

INT_PTR CompareDialog::DlgProc(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg) {
		case WM_COMMAND:
			switch(LOWORD(wparam)) {
				case IDCOMPARE:
					OnCompare();
					return true;
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

#include "CaptureDialog.hpp"
#include <cstdlib>
#include <exception>
#include <sstream>

void CompareDialog::OnCompare()
{
	Image::Pixel *image_a_begin, *image_b_begin;
	auto image_a_pixel_count = image_a->GetPixels(image_a_begin);
	auto image_b_pixel_count = image_b->GetPixels(image_b_begin);
	auto image_a_it = image_a_begin;
	auto image_b_it = image_b_begin;

	if(image_a_pixel_count != image_b_pixel_count)
		throw new std::exception("pixel_count differ between both images");

	auto comp_result_pixels = new Image::Pixel[image_a_pixel_count];
	auto comp_it = comp_result_pixels;

	int max_diff = 0;
	int avg_diff = 0;
	auto comp = [&](BYTE& c, BYTE& l, BYTE& r) -> void {
		c = abs(l - r);
		if(max_diff < c)
			max_diff = c;
		avg_diff += c;
	};

	for(size_t i = 0; i < image_a_pixel_count; ++i, ++image_a_it, ++image_b_it, ++comp_it) {
		comp(comp_it->r, image_a_it->r, image_b_it->r);
		comp(comp_it->g, image_a_it->g, image_b_it->g);
		comp(comp_it->b, image_a_it->b, image_b_it->b);
		comp_it->a = 0xff;
	}

	avg_diff /= (int)image_a_pixel_count;

	Image::FreePixels(image_a_begin);
	Image::FreePixels(image_b_begin);

	auto comp_img = Image::CreateFromPixels(comp_result_pixels, image_a->GetWidth(), image_a->GetHeight());

	CaptureDialog::ShowDialog(comp_img);

	delete[] comp_result_pixels;

	std::wstringstream wss;
	wss << L"max_diff = " << max_diff << std::endl
		<< L"avg_diff = " << avg_diff;
	MessageBox(dialog_wnd, wss.str().c_str(), TEXT("Compare"), MB_OK | MB_ICONINFORMATION);
}
