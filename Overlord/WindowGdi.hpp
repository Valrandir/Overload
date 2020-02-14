#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Image.hpp"

class WindowGdi
{
	void AdjustAndCenter(int& x, int& y, int& width, int& height, DWORD style);
	void OnPaint();
	static LRESULT CALLBACK WndProcStatic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	HWND _hwnd;
	bool _destroyed;
	int _width, _height;
	HDC _hDC;
	HBITMAP _hBitmap;

	virtual LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);

public:
	WindowGdi(LPCTSTR title, int width, int height, DWORD style = 0);
	virtual ~WindowGdi();
	WindowGdi(const WindowGdi&) = delete;
	WindowGdi& operator=(const WindowGdi&) = delete;

	virtual void SetTitle(LPCTSTR title);
	virtual void SetStyle(DWORD style);
	virtual void Show();
	virtual void Hide();
	virtual bool Update();
	virtual void Close();

	virtual void DrawImage(const Image* image, int x, int y);

	inline HWND GetHandle() const { return _hwnd; }
	inline int GetWidth() const { return _width; }
	inline int GetHeight() const { return _height; }
	inline HDC GetDC() const { return _hDC; }
};
