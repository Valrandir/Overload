#pragma once
#include "AreaView.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class AreaMinimap {
public:
	static void DrawMinimap(HDC hdc, const AreaView& areaview);

private:
	static void GetMinimapInfo(const AreaView& areaview, Rect& out_miniarea, Rect& out_minicamera);
};
