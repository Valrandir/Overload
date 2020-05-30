#pragma once
#include "CaptureSample.hpp"
#include "DialogBase.hpp"
#include "BitmapGdi.hpp"
#include "ImageView.hpp"

class CaptureDialog : private DialogBase {
public:
	static bool ShowDialog(const BitmapGdi* bitmap_gdi, const CaptureSource* capture_source = nullptr);
	static bool ShowDialog(const CaptureSample* capture_sample);
	void Initialize() override;
	INT_PTR DlgProc(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
	const CaptureSource* capture_source;
	const BitmapGdi* bitmap_gdi;
	ImageView imageview;

	CaptureDialog(const BitmapGdi* bitmap_gdi, const CaptureSource* capture_source) :
		bitmap_gdi{bitmap_gdi},
		capture_source{capture_source} {}
};
