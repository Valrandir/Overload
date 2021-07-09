#include "ControlFactory.hpp"
#include <cassert>

HWND ControlFactory::CreateLabel(HWND parent, LPCTSTR text, const Layout& layout)
{
	return CreateControl(parent, TEXT("STATIC"), text, layout, 0, ES_READONLY);
}

HWND ControlFactory::CreateNumericBox(HWND parent, LPCTSTR text, const Layout& layout)
{
	return CreateControl(parent, TEXT("EDIT"), text, layout, WS_EX_STATICEDGE, ES_NUMBER);
}

HWND ControlFactory::CreateTextBox(HWND parent, LPCTSTR text, const Layout& layout)
{
	return CreateControl(parent, TEXT("EDIT"), text, layout, WS_EX_STATICEDGE, 0);
}

HWND ControlFactory::CreateControl(HWND parent, LPCTSTR class_name, LPCTSTR text, const Layout& layout, DWORD ex_style, DWORD style)
{
	HWND control =
		CreateWindowEx(
			ex_style,
			class_name,
			text,
			style | WS_CHILD | WS_TABSTOP | WS_VISIBLE,
			layout.Location.x,
			layout.Location.y,
			layout.Size.cx,
			layout.Size.cy,
			parent,
			NULL,
			GetModuleHandle(0),
			0);

	assert(control);
	return control;
}
