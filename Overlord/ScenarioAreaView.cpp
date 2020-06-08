#include "AreaView.hpp"
#include "BitmapGdi.hpp"
#include "WindowGdi.hpp"

int ScenarioAreaView()
{
	auto ss = GetScreenSize();
	WindowGdi window(TEXT("Area View"), ss.cx / 2, ss.cy / 2);
	window.Show();

	//auto bitmap = BitmapGdi::LoadPNG("C:\\out\\nature_huge.png");
	//auto bits = bitmap.GetBits();
	//auto bits = ImageBits::LoadPNG("C:\\out\\nature_huge.png");
	//bits.SaveBits("C:\\out\\nature_huge.bits");

	auto bits = ImageBits::LoadBits("C:\\out\\nature_huge.bits");
	auto bitmap = BitmapGdi::CreateFromBits(bits);

	AreaView av({bitmap.Width(), bitmap.Height()}, {window.Width(), window.Height()});

	auto p = av.Offset();
	window.DrawBitmap(bitmap, p.x, p.y);

	while(window.Update()) {
		Sleep(1);
	}

	return 0;
}
