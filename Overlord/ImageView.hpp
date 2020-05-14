#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Image.hpp"
#include "ScrollHandler.hpp"

class ImageView {
public:
	ImageView(){};
	void Initialize(HWND hWndParent, int x, int y, int width, int height);
	~ImageView();
	ImageView(const ImageView&) = delete;
	ImageView& operator=(const ImageView&) = delete;

	void Show();
	bool Update();
	void Close();

	void SetImage(const Image* image);

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

	void SetupScrollInfo(const Image* image);
	void DrawImage(const Image* image, int x, int y);
	void OnPaint();
	void OnScroll(UINT msg, WPARAM wParam, LPARAM lParam);
	void OnMouseMove(int x, int y);
	void OnMouseDown(int x, int y);
	void OnMouseUp(int x, int y);
	static LRESULT CALLBACK WndProcStatic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
};
