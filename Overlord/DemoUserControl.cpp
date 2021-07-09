#include "DemoUserControl.hpp"
#include "ControlFactory.hpp"
#include "GdiPen.hpp"
#include "GdiBrush.hpp"
#include <cstdlib>

struct Cursor {
	ControlFactory::Layout Layout;
	LONG htab, vtab;

	Cursor(int x, int y, int width, int height) :
		Layout{x, y, width, height},
		htab{height / 2},
		vtab{height / 2}
	{
	}

	Cursor& Offset(int x, int y)
	{
		Layout.Location.x += x * (Layout.Size.cx + htab);
		Layout.Location.y += y * (Layout.Size.cy + vtab);
		return *this;
	}
};

void DemoUserControl::Initialize(HWND parent_window, int x, int y)
{
	UserControl::Initialize(parent_window, x, y, 200, 105);

	Cursor c{10, 10, 48, 20};
	ControlFactory::CreateLabel(window, TEXT("Red: "), c.Layout);

	c.Offset(1, 0);
	edit_red = ControlFactory::CreateNumericBox(window, TEXT("0"), c.Layout);
	SendMessage(edit_red, EM_SETLIMITTEXT, 3, 0);

	c.Offset(-1, 1);
	ControlFactory::CreateLabel(window, TEXT("Green: "), c.Layout);

	c.Offset(1, 0);
	edit_green = ControlFactory::CreateNumericBox(window, TEXT("0"), c.Layout);
	SendMessage(edit_green, EM_SETLIMITTEXT, 3, 0);

	c.Offset(-1, 1);
	ControlFactory::CreateLabel(window, TEXT("Blue: "), c.Layout);

	c.Offset(1, 0);
	edit_blue = ControlFactory::CreateNumericBox(window, TEXT("0"), c.Layout);
	SendMessage(edit_blue, EM_SETLIMITTEXT, 3, 0);

	initialized = true;
}

int GetNumericValue(HWND numeric_box)
{
	const size_t BUFFER_SIZE = 4;
	TCHAR buffer[BUFFER_SIZE];
	GetWindowText(numeric_box, buffer, BUFFER_SIZE);

#ifdef UNICODE
	return _wtoi(buffer);
#else
	return atoi(buffer);
#endif
}

void SetNumericBox(HWND numeric_box, LPCTSTR text)
{
	SetWindowLongPtr(numeric_box, GWLP_USERDATA, 1);
	SetWindowText(numeric_box, text);
}

void ClampNumericBox(HWND numeric_box)
{
	int v = GetNumericValue(numeric_box);
	if(v < 0)
		SetNumericBox(numeric_box, TEXT("0"));
	else if(v > 255)
		SetNumericBox(numeric_box, TEXT("255"));
}

void DemoUserControl::UpdateColor()
{
	int red = GetNumericValue(edit_red);
	int green = GetNumericValue(edit_green);
	int blue = GetNumericValue(edit_blue);
	bg_color = RGB(red, green, blue);
	InvalidateRect(window, 0, FALSE);
}

void DemoUserControl::OnPaint()
{
	PAINTSTRUCT ps;
	BeginPaint(window, &ps);

	GdiPen pen(ps.hdc, bg_color);
	GdiBrush brush(ps.hdc, bg_color);

	int x = width - width / 3;
	int y = height / 4;
	int s = height / 2;
	Ellipse(ps.hdc, x, y, x + s, y + s);

	EndPaint(window, &ps);
}

LRESULT DemoUserControl::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
	if(initialized)
		switch(msg) {
			case WM_PAINT:
				OnPaint();
				break;
			case WM_COMMAND:
				auto ctrl_id = LOWORD(wparam);
				auto ctrl_wnd = (HWND)lparam;
				switch(HIWORD(wparam)) {
					case EN_CHANGE:
						if(GetWindowLongPtr(ctrl_wnd, GWLP_USERDATA))
							SetWindowLongPtr(ctrl_wnd, GWLP_USERDATA, 0);
						else
						{
							ClampNumericBox(ctrl_wnd);
							UpdateColor();
						}
						return 0;
				}
		}

	return UserControl::WndProc(msg, wparam, lparam);
}

