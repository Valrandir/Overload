#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../Overlord/Image.hpp"
#include "../Overlord/CaptureWnd.hpp"
#include "../Overlord/CaptureDialog.hpp"
#include <shellapi.h>

#define IMAGE_TESTS_IGNORE

TEST_CLASS(ImageTests) {
public:
#ifdef IMAGE_TESTS_IGNORE
	BEGIN_TEST_METHOD_ATTRIBUTE(Image_Capture_SaveToFile_OpenFolder)
	TEST_IGNORE()
	END_TEST_METHOD_ATTRIBUTE()
#endif
	TEST_METHOD(Image_Capture_SaveToFile_OpenFolder) {
		//Arrange
		Image* image = CaptureWnd::Capture().image;

		if(!image) {
			Assert::Fail();
			return;
		}

		//Act
		image->SaveToFile("captured.png");
		delete image;

		//Assert
		ShellExecute(0, TEXT("explore"), TEXT("."), 0, 0, SW_SHOW);
	}

#ifdef IMAGE_TESTS_IGNORE
	BEGIN_TEST_METHOD_ATTRIBUTE(Image_CreateFranceFlag_ShellStart)
	TEST_IGNORE()
	END_TEST_METHOD_ATTRIBUTE()
#endif
	TEST_METHOD(Image_CreateFranceFlag_ShellStart) {
		//Arrange
		int width = 240 * 4;
		int height = 120 * 4;

		//Act
		auto img = Image::CreateBlank(width, height, RGB(255, 255, 255));
		img->FillRect(0, 0, width / 3, height, RGB(0, 0, 255));
		img->FillRect(width / 3 * 2, 0, width, height, RGB(255, 0, 0));
		img->ClearCachedBits();
		img->SaveToFile("france.png");
		delete img;

		//Assert
		ShellExecute(0, TEXT("open"), TEXT("france.png"), 0, 0, SW_SHOW);
	}

#ifdef IMAGE_TESTS_IGNORE
	BEGIN_TEST_METHOD_ATTRIBUTE(Image_LoadImage_Test)
	TEST_IGNORE()
	END_TEST_METHOD_ATTRIBUTE()
#endif
	TEST_METHOD(Image_LoadImage_Test) {
		auto img = Image::LoadFile("france.png");

		WindowGdi w(0, 1000, 800);
		w.Show();
		w.DrawImage(img, 0, 0);
		while(w.Update()) {
			Sleep(1);
		}
	}
};
