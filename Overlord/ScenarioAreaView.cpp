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
	BitmapGdiZoom zoomable;

	AreaViewWindow(int width, int height, BitmapGdi&& bitmap) :
		WindowGdi(TEXT("Area View"), width, height)
	{
		ms.SetWindow(window);
		av = AreaView({bitmap.Width(), bitmap.Height()}, {Width(), Height()});

		ms.ScrollEvent.SetHandler(OnMouseScrollEvent, this);

		this->zoomable = std::move(bitmap);
	}

	LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override
	{
		if(ms.ProcessMessage(msg, wparam))
			return 0;

		return WindowGdi::WndProc(msg, wparam, lparam);
	}

	bool OnKeyDown(WPARAM wparam) override
	{
		if(LOWORD(wparam) == VK_SUBTRACT) {
			av.OffsetCameraZoom(-.1f);
			ResizeBitmap();
		} else if(LOWORD(wparam) == VK_ADD) {
			av.OffsetCameraZoom(.1f);
			ResizeBitmap();
		} else
			return WindowGdi::OnKeyDown(wparam);

		DrawBitmap();
		return true;
	}

	void DrawBitmap()
	{
		Clear(RGB(64, 64, 64));
		InvalidateRect(window, 0, FALSE);

		const auto& camera = av.Camera();
		const auto& view = av.View();

		BitBlt(WindowGdi::dc, 0, 0, view.w, view.h, zoomable.GetDC(), camera.x, camera.y, SRCCOPY);
		AreaMinimap::DrawMinimap(GetDC(), av);

		std::wstringstream ss;
		ss << "Zoomed w=" << zoomable.Width() << " h=" << zoomable.Height();
		const auto& text = ss.str();
		TextOut(WindowGdi::dc, 0, 0, text.c_str(), (int)text.length());
	}

	void ResizeBitmap()
	{
		//const auto& camera = av.Camera();
		//const auto& view = av.View();
		//const auto& zoom = av.Zoom();

		zoomable.Zoom(av.Zoom());
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
