#pragma once
#include "WindowGdi.hpp"
#include "CaptureSource.hpp"
#include "BitmapGdi.hpp"

class CaptureWnd : public WindowGdi {
	LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;
	POINT origin{};
	RECT sel_rect{};
	bool selecting{};
	BitmapGdi* captured_image{};
	void OnMouseMove(int x, int y);
	void OnMouseDown(int x, int y);
	void OnMouseUp(int x, int y);
	CaptureWnd();

public:
	virtual ~CaptureWnd();
	CaptureWnd(const CaptureWnd&) = delete;
	CaptureWnd& operator=(const CaptureWnd&) = delete;
	static BitmapGdi* Capture(CaptureSource* out_capture_source = nullptr);
	static BitmapGdi Recapture(const CaptureSource& capture_source);
};
