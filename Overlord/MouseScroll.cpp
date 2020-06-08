#include "MouseScroll.hpp"

bool MouseScroll::ProcessMessage(UINT msg, WPARAM wparam)
{
	if(!window)
		return false;

	switch(msg) {
		case WM_MOUSEMOVE:
			if(wparam & MK_LBUTTON) {
				OnLMouseMove();
				return true;
			}
			break;
		case WM_LBUTTONDOWN:
			OnLMouseDown();
			return true;
		case WM_LBUTTONUP:
			OnLMouseUp();
			return true;
		case WM_CAPTURECHANGED:
			mouse_dragging = false;
			return true;
	}

	return false;
}

void MouseScroll::OnLMouseMove()
{
	if(!mouse_dragging)
		return;

	POINT screen_pos;
	GetCursorPos(&screen_pos);

	auto offset_x = mouse_origin.x - screen_pos.x;
	auto offset_y = mouse_origin.y - screen_pos.y;
	mouse_origin.x = screen_pos.x;
	mouse_origin.y = screen_pos.y;

	ScrollEvent(offset_x, offset_y);
}

void MouseScroll::OnLMouseDown()
{
	mouse_dragging = true;

	POINT screen_pos;
	GetCursorPos(&screen_pos);

	mouse_origin.x = screen_pos.x;
	mouse_origin.y = screen_pos.y;

	SetCapture(window);
}

void MouseScroll::OnLMouseUp()
{
	ReleaseCapture();
}
