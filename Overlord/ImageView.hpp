#pragma once
#include "BitmapGdi.hpp"
#include "Event.hpp"
#include "ScrollHandler.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class ImageView : public BitmapGdi {
public:
	Event<int, int> ScrollEvent;
	Event<bool> ZoomEvent;

	ImageView() = default;
	void Initialize(HWND parent_window, HWND placeholder, const BitmapGdi* bitmap_gdi);
	~ImageView();
	ImageView(const ImageView&) = delete;
	ImageView& operator=(const ImageView&) = delete;

	void Show();
	bool Update();
	void UpdateLayout();
	void Close();

	void Scroll(int offset_x, int offset_y);
	void ZoomIn();
	void ZoomOut();

	inline HWND GetHandle() const { return window; }

private:
	HWND window{};
	HWND placeholder{};
	bool destroyed{};

	const BitmapGdi* bitmap_gdi{}; //Not Owned

	ScrollBarHandler sbh;
	bool mouse_dragging{};
	POINT mouse_origin{};

	const int ZOOM_LEVEL_MAX = 4;
	int zoom_level{1};
	int zoom_factor{};

	void SetupScrollInfo(const BitmapGdi& bitmap_gdi);
	void ClearBackground();
	void DrawImage(const BitmapGdi& bitmap_gdi, int x, int y);

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
