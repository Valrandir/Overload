#pragma once
#include "CaptureSample.hpp"
#include "DialogBase.hpp"
#include "Image.hpp"
#include "ImageView.hpp"

class CaptureDialog : private DialogBase {
public:
	static bool ShowDialog(const Image* image, const CaptureSource* capture_source = nullptr);
	static bool ShowDialog(const CaptureSample* capture_sample);
	void Initialize() override;
	INT_PTR DlgProc(HWND dialog_wnd, UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
	const CaptureSource* capture_source;
	const Image* image;
	ImageView imageview;

	CaptureDialog(const Image* image, const CaptureSource* capture_source) :
		image{image},
		capture_source{capture_source} {}
};
