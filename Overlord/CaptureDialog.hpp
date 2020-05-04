#pragma once
#include "DialogBase.hpp"
#include "CaptureSample.hpp"

class CaptureDialog : public DialogBase {
public:
	static bool ShowDialog(const CaptureSample* capture_sample);
	void Initialize() override;
	INT_PTR DlgProc(HWND hDialogWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

private:
	const CaptureSample* _capture_sample;

	CaptureDialog(const CaptureSample* capture_sample) :
		_capture_sample{capture_sample} {}

	void DrawSampleImage(const DRAWITEMSTRUCT* dis);
};
