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

	inline HWND GetHandle() const { return window; }
	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }
	inline HDC GetDC() const { return dc; }

private:
	HWND window{};
	bool destroyed{};
	int width, height;
	HDC dc;
	HBITMAP bitmap;

	const Image* image{};

	ScrollBarHandler sbh;
	bool mouse_dragging{};
	POINT mouse_origin{};

	const int ZOOM_LEVEL_MAX = 4;
	int zoom_level{1};
	int zoom_factor{};

	void SetupScrollInfo(const Image* image);
	void ClearBackground();
	void DrawImage(const Image* image, int x, int y);

	void OnPaint();
	void OnScroll(UINT msg, WPARAM wparam, LPARAM lparam);
	void OnLMouseMove();
	void OnLMouseDown();
	void OnLMouseUp();
	void OnZoom(int direction);
	void UpdateZoom(int direction);

	static LRESULT CALLBACK WndProcStatic(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);
};
