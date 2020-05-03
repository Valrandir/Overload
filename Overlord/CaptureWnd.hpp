#pragma once
#include "WindowGdi.hpp"
#include "CaptureSample.hpp"

class CaptureWnd : public WindowGdi {
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam) override;
	POINT _origin{};
	RECT _selRect{};
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
	static Image* Capture();
	static CaptureSample Sample();
};