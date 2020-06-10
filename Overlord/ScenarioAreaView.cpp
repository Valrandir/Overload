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
		av.CenterView();

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
			av.OffsetZoom(-.1f);
		else if(LOWORD(wparam) == VK_NEXT)
			av.OffsetZoom(.1f);
		else
			return WindowGdi::OnKeyDown(wparam);

		DrawBitmap();
		return true;
	}

	void DrawMinimap()
	{
		Rect area, view;
		av.GetMinimapInfo(area, view);
		Rectangle(GetDC(), area.x, area.y, area.x + area.w, area.y + area.h);
		Rectangle(GetDC(), view.x, view.y, view.x + view.w, view.y + view.h);
	}

	void DrawBitmap()
	{
		auto v = av.ViewSize();
		auto o = av.Offset();

		Clear(RGB(64, 64, 64));
		InvalidateRect(window, 0, FALSE);

		//StretchBlt(WindowGdi::dc, -v.x, -v.y, v.w, v.h, bitmap.GetDC(), o.x, o.y, v.w, v.h, SRCCOPY);
		DrawMinimap();
	}

	static void OnMouseScrollEvent(int offset_x, int offset_y, void* userdata)
	{
		auto& self = *(AreaViewWindow*)userdata;
		self.av.Offset({offset_x, offset_y});
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
