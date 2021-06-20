#include "AreaMinimap.hpp"
#include "GdiPen.hpp"
#include <cmath>
#include <sstream>

static std::wstring GetAreaCaption(const AreaView& areaview)
{
	std::wstringstream ss;
	const auto& area = areaview.Area();
	ss << L"Area w=" << area.w << " h=" << area.h;
	return ss.str();
}

static std::wstring GetCameraCaption(const AreaView& areaview)
{
	std::wstringstream ss;
	const auto& camera = areaview.Camera();
	ss << L"Camera x=" << camera.x << " y=" << camera.y << " w=" << camera.w << " h=" << camera.h;
	return ss.str();
}

static std::wstring GetZoomCaption(const AreaView& areaview)
{
	std::wstringstream ss;
	const auto& zoom = areaview.Zoom();
	ss << L"Zoom=" << zoom;
	return ss.str();
}

void AreaMinimap::DrawMinimap(HDC hdc, const AreaView& areaview)
{
	GdiPen pen(hdc, RGB(224, 0, 0));

	Rect map, camera;
	GetMinimapInfo(areaview, map, camera);

	Rectangle(hdc, map.x, map.y, map.x + map.w, map.y + map.h);
	Rectangle(hdc, camera.x, camera.y, camera.x + camera.w, camera.y + camera.h);

	auto text = GetAreaCaption(areaview);
	TextOut(hdc, map.x, map.y - 48, text.c_str(), (int)text.length());

	text = GetCameraCaption(areaview);
	TextOut(hdc, map.x, map.y - 32, text.c_str(), (int)text.length());

	text = GetZoomCaption(areaview);
	TextOut(hdc, map.x, map.y - 16, text.c_str(), (int)text.length());
}

void AreaMinimap::GetMinimapInfo(const AreaView& areaview, Rect& out_miniarea, Rect& out_minicamera)
{
	const int scale = 10;

	auto area = areaview.Area();
	auto view = areaview.View();
	auto camera = areaview.Camera();

	int ma_w = view.w / scale;
	int ma_h = view.h / scale;
	int ma_x = ma_w / 2;
	int ma_y = view.h - ma_h - ma_h / 2;
	out_miniarea = {ma_x, ma_y, ma_w, ma_h};

	float rx = (float)ma_w / area.w;
	float ry = (float)ma_h / area.h;
	int mc_x = (int)std::roundf(camera.x * rx);
	int mc_y = (int)std::roundf(camera.y * ry);
	int mc_w = (int)std::roundf(camera.w * rx);
	int mc_h = (int)std::roundf(camera.h * ry);

	out_minicamera = {ma_x + mc_x, ma_y + mc_y, mc_w, mc_h};
}
