#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Image.hpp"

class WindowGdi {
	void AdjustAndCenter(int& x, int& y, int& width, int& height, DWORD style);
	void OnPaint();
	static LRESULT CALLBACK WndProcStatic(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);

protected:
	HWND window;
	bool destroyed;
	int width, height;
	HCURSOR cursor{};
	HDC dc;
	HBITMAP bitmap;

	virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

public:
	WindowGdi(LPCTSTR title, int width, int height, DWORD style = 0);
	virtual ~WindowGdi();
	WindowGdi(const WindowGdi&) = delete;
	WindowGdi& operator=(const WindowGdi&) = delete;

	virtual void SetTitle(LPCTSTR title);
	virtual void SetStyle(DWORD style);
	virtual void SetCursor(LPTSTR cursor);
	virtual void Show();
	virtual void Hide();
	virtual bool Update();
	virtual void Close();

	virtual void DrawImage(const Image* image, int x, int y);

	inline HWND GetHandle() const { return window; }
	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }
	inline HDC GetDC() const { return dc; }
};
