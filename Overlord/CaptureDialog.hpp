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
	INT_PTR DlgProc(HWND hDialogWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

private:
	const CaptureSource* _capture_source;
	const Image* _image;
	ImageView _image_view;

	CaptureDialog(const Image* image, const CaptureSource* capture_source) :
		_image{image},
		_capture_source{capture_source} {}
};
