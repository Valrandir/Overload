#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vector>

class LayoutHandler {
public:
	enum class LayoutType {
		None,
		Horizontal,
		Vertical,
		Both
	};

	struct LayoutElement {
		LayoutType type;
		SIZE sizing_x{100, 100};
		SIZE sizing_y{100, 100};
	};

	struct Layout {
		LayoutElement moving_type;
		LayoutElement sizing_type;
	};

	void Initialize(HWND dialog_wnd, SIZE initial_dialog_size);
	void DefineLayout(Layout layout, HWND dialog_wnd, int dlgitemid);
	void OnSizing(RECT& new_rect, WPARAM wparam);
	void OnSize(int width, int height);

private:
	HWND dialog_wnd{};
	SIZE dialog_size{};
	SIZE dialog_size_min{};
	std::vector<Layout> layouts;

	static BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lparam);
	void MoveChild(HWND child, const Layout& layout, const SIZE& offset);
};
