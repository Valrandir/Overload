#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Image.hpp"
#include "ScrollHandler.hpp"

class ImageView {
public:
	using ScrollCallbackFunc = void (*)(int offset_x, int offset_y, void* userdata);
	using ZoomCallbackFunc = void (*)(bool is_zoom_out, void* userdata);

	ImageView(){};
	void Initialize(HWND hWndParent, int x, int y, int width, int height);
	~ImageView();
	ImageView(const ImageView&) = delete;
	ImageView& operator=(const ImageView&) = delete;

	void Show();
	bool Update();
	void Close();

	void SetImage(const Image* image);
	void ScrollImage(int offset_x, int offset_y);
	void SetScrollCallback(ScrollCallbackFunc callback, void* userdata);
	void SetZoomCallback(ZoomCallbackFunc callback, void* userdata);
	void ZoomIn();
	void ZoomOut();

	inline HWND GetHandle() const { return _hWnd; }
	inline int GetWidth() const { return _width; }
	inline int GetHeight() const { return _height; }
	inline HDC GetDC() const { return _hDC; }

private:
	HWND _hWnd;
	bool _destroyed{};
	int _width, _height;
	HDC _hDC;
	HBITMAP _hBitmap;

	const Image* _image{};

	ScrollBarHandler _sbh;

	bool _mouse_dragging{};
	POINT _mouse_origin{};
	ScrollCallbackFunc _scroll_cb_func{};
	void* _scroll_cb_userdata;

	const int ZOOM_LEVEL_MAX = 4;
	int _zoom_level{1};
	int _zoom_factor;
	ZoomCallbackFunc _zoom_cb_func{};
	void* _zoom_cb_userdata;

	void SetupScrollInfo(const Image* image);
	void DrawImage(const Image* image, int x, int y);
	void OnPaint();
	void OnScroll(UINT msg, WPARAM wParam, LPARAM lParam);
	void OnLMouseMove();
	void OnLMouseDown();
	void OnLMouseUp();
	void OnZoom(int direction);
	void UpdateZoom(int direction);
	void ClearBackground();
	static LRESULT CALLBACK WndProcStatic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
};
