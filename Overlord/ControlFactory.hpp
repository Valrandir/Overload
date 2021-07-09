#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

struct ControlFactory {
	struct Layout {
		POINT Location;
		SIZE Size;
	};

	static HWND CreateLabel(HWND parent, LPCTSTR text, const Layout& layout);
	static HWND CreateNumericBox(HWND parent, LPCTSTR text, const Layout& layout);
	static HWND CreateTextBox(HWND parent, LPCTSTR text, const Layout& layout);
	static HWND CreateControl(HWND parent, LPCTSTR class_name, LPCTSTR text, const Layout& layout, DWORD ex_style, DWORD style);
};
