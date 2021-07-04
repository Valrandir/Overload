#include "CompareDialog.hpp"
#include "BitmapGdi.hpp"
#include "Overlord.rc.h"
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
	HWND placeholder = GetDlgItem(dialog_wnd, IDC_STATIC_IMG_1);
	imageview_a.Initialize(dialog_wnd, placeholder, bitmap_gdi_a);
	imageview_a.ScrollEvent.SetHandler(ScrollCallback, &imageview_b);
	imageview_a.ZoomEvent.SetHandler(ZoomCallback, &imageview_b);

	placeholder = GetDlgItem(dialog_wnd, IDC_STATIC_IMG_2);
	imageview_b.Initialize(dialog_wnd, placeholder, bitmap_gdi_b);
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

	return Dialog::DlgProc(dialog_wnd, msg, wparam, lparam);
}

#include "CaptureDialog.hpp"
#include <cstdlib>
#include <exception>
#include <sstream>

void CompareDialog::OnCompare()
{
	auto image_bits_a = bitmap_gdi_a->GetBits();
	auto image_bits_b = bitmap_gdi_b->GetBits();

	if(image_bits_a.ByteSize() != image_bits_b.ByteSize())
		throw new std::exception("pixel_count differ between both images");

	auto comp_result = ImageBits::CreateEmpty(image_bits_a.Width(), image_bits_a.Height());

	int max_diff = 0;
	int avg_diff = 0;
	auto comp = [&](BYTE& c, BYTE& l, BYTE& r) -> void {
		c = abs(l - r);
		if(max_diff < c)
			max_diff = c;
		avg_diff += c;
	};

	auto comp_r = comp_result.begin();
	auto comp_a = image_bits_a.begin();
	auto comp_b = image_bits_b.begin();
	for(; comp_r < comp_result.end(); ++comp_a, ++comp_b, ++comp_r) {
		comp(comp_r->r, comp_a->r, comp_b->r);
		comp(comp_r->g, comp_a->g, comp_b->g);
		comp(comp_r->b, comp_a->b, comp_b->b);
		comp_r->a = 0xff;
	}

	avg_diff /= image_bits_a.PixelCount();
	avg_diff /= 3;

	auto comp_bitmap = BitmapGdi::CreateFromBits(comp_result);
	CaptureDialog::ShowDialog(&comp_bitmap);

	std::wstringstream wss;
	wss << L"max_diff = " << max_diff << std::endl
		<< L"avg_diff = " << avg_diff;
	MessageBox(dialog_wnd, wss.str().c_str(), TEXT("Compare"), MB_OK | MB_ICONINFORMATION);
}
