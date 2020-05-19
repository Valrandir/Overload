#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Image.hpp"
#include "ScrollHandler.hpp"
#include "Event.hpp"

class ImageView {
public:
	Event<int, int> ScrollEvent;
	Event<bool> ZoomEvent;

	ImageView() = default;
	void Initialize(HWND hWndParent, int x, int y, int width, int height, const Image* image);
	~ImageView();
	ImageView(const ImageView&) = delete;
	ImageView& operator=(const ImageView&) = delete;

	void Show();
	bool Update();
	void Close();

	void Scroll(int offset_x, int offset_y);
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

	const int ZOOM_LEVEL_MAX = 4;
	int _zoom_level{1};
	int _zoom_factor;

	void SetupScrollInfo(const Image* image);
	void ClearBackground();
	void DrawImage(const Image* image, int x, int y);

	void OnPaint();
	void OnScroll(UINT msg, WPARAM wParam, LPARAM lParam);
	void OnLMouseMove();
	void OnLMouseDown();
	void OnLMouseUp();
	void OnZoom(int direction);
	void UpdateZoom(int direction);

	static LRESULT CALLBACK WndProcStatic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
};
