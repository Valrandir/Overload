#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class UserControl {
	static LRESULT OnCtlColorStatic(WPARAM wparam);
	static LRESULT CALLBACK WndProcStatic(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);

protected:
	HWND window{};
	int width{}, height{};
	bool destroyed = false;

	UserControl() {}
	void Initialize(HWND parent_window, int x, int y, int width, int height);
	virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

public:
	virtual ~UserControl();
	UserControl(const UserControl&) = delete;
	UserControl& operator=(const UserControl&) = delete;

	virtual void GetSize(int& width, int &height) const;
	virtual void Close();
};
