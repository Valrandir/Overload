#include "AreaMinimap.hpp"
#include "AreaView.hpp"
#include "BitmapGdi.hpp"
#include "MouseScroll.hpp"
#include "WindowGdi.hpp"
#include <utility>

struct AreaViewWindow : public WindowGdi {
	MouseScroll ms;
	AreaView av;
	BitmapGdi bitmap;

	AreaViewWindow(int width, int height, BitmapGdi&& bitmap) :
		WindowGdi(TEXT("Area View"), width, height)
	{
		ms.SetWindow(window);
		av = AreaView({bitmap.Width(), bitmap.Height()}, {Width(), Height()});

		ms.ScrollEvent.SetHandler(OnMouseScrollEvent, this);

		this->bitmap = std::move(bitmap);
	}

	LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override
	{
		if(ms.ProcessMessage(msg, wparam))
			return 0;

		return WindowGdi::WndProc(msg, wparam, lparam);
	}

	bool OnKeyDown(WPARAM wparam) override
	{
		if(LOWORD(wparam) == VK_PRIOR)
			av.OffsetCameraZoom(-.1f);
		else if(LOWORD(wparam) == VK_NEXT)
			av.OffsetCameraZoom(.1f);
		else
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
		StretchBlt(WindowGdi::dc, 0, 0, view.w, view.h, bitmap.GetDC(), camera.x, camera.y, camera.w, camera.h, SRCCOPY);
		AreaMinimap::DrawMinimap(GetDC(), av);
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