#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "BitmapGdi.hpp"
#include "CaptureWnd.hpp"
#include "OverlordTest.hpp"

static void ShowImage(const BitmapGdi& bitmap_gdi)
{
	WindowGdi w(0, bitmap_gdi.Width(), bitmap_gdi.Height());
	w.Show();
	w.DrawBitmap(bitmap_gdi, 0, 0);
	while(w.Update())
		Sleep(1);
}

TEST_CLASS(CaptureWndTest) {
public:
	TEST_METHOD(Capture_SaveToFile_OpenFolder) {
		//Arrange
		auto filename = "captured.png";
		auto bitmap_gdi = CaptureWnd::Capture();

		if(!bitmap_gdi) {
			Assert::Fail();
			return;
		}

		//Act
		bitmap_gdi->SavePNG(filename);
		delete bitmap_gdi;

		//Assert
		ShellOpen(filename);
	}
};
