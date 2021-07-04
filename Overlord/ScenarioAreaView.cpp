#include "AreaMinimap.hpp"
#include "AreaView.hpp"
#include "BitmapGdiZoom.hpp"
#include "MouseScroll.hpp"
#include "WindowGdi.hpp"
#include <sstream>
#include <utility>

struct AreaViewWindow : public WindowGdi {
	MouseScroll ms;
	AreaView av;
	BitmapGdiZoom zoomable_bitmap;

	AreaViewWindow(int width, int height, BitmapGdi&& bitmap) :
		WindowGdi(TEXT("Area View"), width, height)
	{
		av = AreaView({bitmap.Width(), bitmap.Height()}, {Width(), Height()});

		ms.SetWindow(window);
		ms.ScrollEvent.SetHandler(OnMouseScrollEvent, this);

		this->zoomable_bitmap = std::move(bitmap);
	}

	LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override
	{
		if(ms.ProcessMessage(msg, wparam))
			return 0;

		return WindowGdi::WndProc(msg, wparam, lparam);
	}

	bool OnKeyDown(WPARAM wparam) override
	{
		switch(LOWORD(wparam)) {
			case VK_SUBTRACT:
				av.OffsetCameraZoom(-.1f);
				ResizeBitmap();
				break;
			case VK_ADD:
				av.OffsetCameraZoom(.1f);
				ResizeBitmap();
				break;
			case VK_DIVIDE:
				av.ResetZoom();
				ResizeBitmap();
				break;
			default:
				return WindowGdi::OnKeyDown(wparam);
		}

		DrawBitmap();
		return true;
	}

	void DrawDebug()
	{
		std::wstringstream ss;
		ss
			<< "Controls" << std::endl
			<< "    + and - to zoom" << std::endl
			<< "    / to reset zoom" << std::endl
			<< "    Mouse drag to move" << std::endl
			<< "Debug" << std::endl
			<< "    Area w=" << av.Area().w << " h=" << av.Area().h << std::endl
			<< "    Area View w=" << av.View().w << " h=" << av.View().h << std::endl
			<< "    Area Zoom =" << av.Zoom() << std::endl
			<< "    Zoomed w=" << zoomable_bitmap.Width() << " h=" << zoomable_bitmap.Height() << std::endl;

		const auto& text = ss.str();
		const auto& area_size = av.Area();
		RECT r{0, 0, area_size.w, area_size.h};
		DrawText(WindowGdi::dc, text.c_str(), (int)text.length(), &r, 0);
	}

	void DrawBitmap()
	{
		Clear(RGB(64, 64, 64));
		InvalidateRect(window, 0, FALSE);

		const auto& camera = av.Camera();
		const auto& view = av.View();
		const auto& zoom = av.Zoom();

		auto src_x = (int)std::roundf(camera.x * zoom);
		auto src_y = (int)std::roundf(camera.y * zoom);
		BitBlt(WindowGdi::dc, 0, 0, view.w, view.h, zoomable_bitmap.GetDC(), src_x, src_y, SRCCOPY);
		AreaMinimap::DrawMinimap(GetDC(), av);

		DrawDebug();
	}

	void ResizeBitmap()
	{
		zoomable_bitmap.Zoom(av.Zoom());
	}

	static void OnMouseScrollEvent(int offset_x, int offset_y, void* userdata)
	{
		auto& self = *(AreaViewWindow*)userdata;
		self.av.OffsetCameraPosition(offset_x, offset_y);
		self.DrawBitmap();
	}
};

int ScenarioAreaView()
{
	auto bitmap = BitmapGdi::CreateFromBits(ImageBits::LoadBits("C:\\out\\nature_huge.bits"));

	auto ss = GetScreenSize();
	AreaViewWindow window(ss.cx / 2, ss.cy / 2, std::move(bitmap));
	window.Show();
	window.DrawBitmap();

	while(window.Update()) {
		Sleep(1);
	}

	return 0;
}
