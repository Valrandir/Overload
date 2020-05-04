#pragma once
#include "DialogBase.hpp"
#include "CaptureSample.hpp"

class CaptureDialog : public DialogBase {
public:
	static bool ShowDialog(const CaptureSample* capture_sample);
	INT_PTR DlgProc(HWND hDialogWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

private:
	CaptureDialog(const CaptureSample* capture_sample) :
		_capture_sample{capture_sample} {}
	const CaptureSample* _capture_sample;

	void InitDialog(HWND hDialogWnd);
	void DrawSampleImage(const DRAWITEMSTRUCT* dis);
};
