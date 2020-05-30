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

bool CompareDialog::ShowDialog(const BitmapGdi* bitmap_gdi_a, const BitmapGdi* bitmap_gdi_b)
{
	assert(bitmap_gdi_a);
	assert(bitmap_gdi_b);

	CompareDialog compare_dialog(bitmap_gdi_a, bitmap_gdi_b);
	return compare_dialog.ShowModal(IDD_DIALOG_COMPARE);
}

void CompareDialog::Initialize()
{
	int x, y, w, h;

	GetDlgItemPoint(dialog_wnd, IDC_STATIC_IMG_1, x, y, w, h);
	imageview_a.Initialize(dialog_wnd, x, y, w, h, bitmap_gdi_a);
	imageview_a.ScrollEvent.SetHandler(ScrollCallback, &imageview_b);
	imageview_a.ZoomEvent.SetHandler(ZoomCallback, &imageview_b);

	GetDlgItemPoint(dialog_wnd, IDC_STATIC_IMG_2, x, y, w, h);
	imageview_b.Initialize(dialog_wnd, x, y, w, h, bitmap_gdi_b);
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
{ /*
	ImageBits::Pixel *bitmap_gdi_a_begin, *bitmap_gdi_b_begin;
	auto bitmap_gdi_a_pixel_count = bitmap_gdi_a->GetPixels(bitmap_gdi_a_begin);
	auto bitmap_gdi_b_pixel_count = bitmap_gdi_b->GetPixels(bitmap_gdi_b_begin);
	auto bitmap_gdi_a_it = bitmap_gdi_a_begin;
	auto bitmap_gdi_b_it = bitmap_gdi_b_begin;

	if(bitmap_gdi_a_pixel_count != bitmap_gdi_b_pixel_count)
		throw new std::exception("pixel_count differ between both images");

	auto comp_result_pixels = new ImageBits::Pixel[bitmap_gdi_a_pixel_count];
	auto comp_it = comp_result_pixels;

	int max_diff = 0;
	int avg_diff = 0;
	auto comp = [&](BYTE& c, BYTE& l, BYTE& r) -> void {
		c = abs(l - r);
		if(max_diff < c)
			max_diff = c;
		avg_diff += c;
	};

	for(size_t i = 0; i < bitmap_gdi_a_pixel_count; ++i, ++bitmap_gdi_a_it, ++bitmap_gdi_b_it, ++comp_it) {
		comp(comp_it->r, bitmap_gdi_a_it->r, bitmap_gdi_b_it->r);
		comp(comp_it->g, bitmap_gdi_a_it->g, bitmap_gdi_b_it->g);
		comp(comp_it->b, bitmap_gdi_a_it->b, bitmap_gdi_b_it->b);
		comp_it->a = 0xff;
	}

	avg_diff /= (int)bitmap_gdi_a_pixel_count;
	avg_diff /= 3;

	Image::FreePixels(bitmap_gdi_a_begin);
	Image::FreePixels(bitmap_gdi_b_begin);

	auto comp_img = Image::CreateFromPixels(comp_result_pixels, bitmap_gdi_a->GetWidth(), bitmap_gdi_a->GetHeight());

	CaptureDialog::ShowDialog(comp_img);

	delete[] comp_result_pixels;

	std::wstringstream wss;
	wss << L"max_diff = " << max_diff << std::endl
		<< L"avg_diff = " << avg_diff;
	MessageBox(dialog_wnd, wss.str().c_str(), TEXT("Compare"), MB_OK | MB_ICONINFORMATION);
	*/
}
