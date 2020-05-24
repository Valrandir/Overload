#pragma once
#include "WindowGdi.hpp"
#include "CaptureSource.hpp"

class CaptureWnd : public WindowGdi {
	LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;
	POINT origin{};
	RECT sel_rect{};
	bool selecting{};
	Image* captured_image{};
	void OnMouseMove(int x, int y);
	void OnMouseDown(int x, int y);
	void OnMouseUp(int x, int y);
	CaptureWnd();

public:
	virtual ~CaptureWnd();
	CaptureWnd(const CaptureWnd&) = delete;
	CaptureWnd& operator=(const CaptureWnd&) = delete;
	static Image* Capture(CaptureSource* outcapture_source = nullptr);
	static Image* Recapture(const CaptureSource& capture_source);
};
