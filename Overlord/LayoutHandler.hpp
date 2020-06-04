#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vector>

class LayoutHandler {
public:
	struct LayoutRatio {
		/// <summary>Percents 0 - 100</summary>
		int x_ratio{};

		/// <summary>Percents 0 - 100</summary>
		int y_ratio{};

		//bool IsHorizontal() const { return x_ratio > 0; }
		//bool IsVertical() const { return y_ratio > 0; }
		//bool IsNone() const { return !IsHorizontal() && !IsVertical(); }
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
	SIZE dialog_size{};
	SIZE dialog_size_min{};
	std::vector<Layout> layouts;

	static BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lparam);
	void LayoutChild(HWND child, const Layout& layout, const SIZE& offset);
};
