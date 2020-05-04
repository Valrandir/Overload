#pragma once
#include "WindowGdi.hpp"
#include "CaptureSample.hpp"

class CaptureWnd : public WindowGdi {
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam) override;
	POINT _origin{};
	RECT _sel_rect{};
	bool _selecting{};
	Image* _capturedImage{};
	void OnMouseMove(int x, int y);
	void OnMouseDown(int x, int y);
	void OnMouseUp(int x, int y);
	CaptureWnd();

public:
	virtual ~CaptureWnd();
	CaptureWnd(const CaptureWnd&) = delete;
	CaptureWnd& operator=(const CaptureWnd&) = delete;
	static CaptureSample Capture();
	static CaptureSample Recapture(const CaptureSource& capture_source);
};
