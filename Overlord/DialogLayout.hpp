#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <map>

class DialogLayout {
public:
	struct LayoutRatio {
		float x_ratio{};
		float y_ratio{};

		bool Horizontal() const { return x_ratio > 0; }
		bool Vertical() const { return y_ratio > 0; }
	};

	struct Layout {
		LayoutRatio Move;
		LayoutRatio Size;
		RECT Initial{};
	};

	void Initialize(UINT dialog_resource_id, HWND dialog_wnd);
	void AddLayout(Layout layout, HWND child_wnd);
	void OnSizing(RECT& new_rect, WPARAM wparam);
	void OnSize(int width, int height);

private:
	HWND dialog_wnd{};
	SIZE dialog_size_initial{};
	SIZE dialog_size_min{};
	std::map<HWND, Layout> layouts;

	static BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lparam);
	void LayoutChild(HWND child_wnd, const Layout& layout, const SIZE& offset);
};
