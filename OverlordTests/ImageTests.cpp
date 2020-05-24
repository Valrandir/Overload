#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../Overlord/Image.hpp"
#include "../Overlord/CaptureWnd.hpp"
#include <shellapi.h>

//#define IMAGE_TESTS_IGNORE

static Image* CreateFranceFlag()
{
	int width = 240 * 4;
	int height = 120 * 4;
	auto img = Image::CreateBlank(width, height, RGB(255, 255, 255));
	img->Fill(0, 0, width / 3, height, RGB(0, 0, 255));
	img->Fill(width / 3 * 2, 0, width, height, RGB(255, 0, 0));
	return img;
}

static void ShowImage(const Image* img)
{
	WindowGdi w(0, img->GetWidth(), img->GetHeight());
	w.Show();
	w.DrawImage(img, 0, 0);
	while(w.Update())
		Sleep(1);
}

TEST_CLASS(ImageTests) {
public:
#ifdef IMAGE_TESTS_IGNORE
	BEGIN_TEST_METHOD_ATTRIBUTE(Image_Capture_SaveToFile_OpenFolder)
	TEST_IGNORE()
	END_TEST_METHOD_ATTRIBUTE()
#endif
	TEST_METHOD(Image_Capture_SaveToFile_OpenFolder) {
		//Arrange
		Image* img = CaptureWnd::Capture();

		if(!img) {
			Assert::Fail();
			return;
		}

		//Act
		img->SaveFile("captured.png");
		Image::FreeImage(img);

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
		auto img = CreateFranceFlag();

		//Act
		img->SaveFile("france.png");
		Image::FreeImage(img);

		//Assert
		ShellExecute(0, TEXT("open"), TEXT("france.png"), 0, 0, SW_SHOW);
	}

#ifdef IMAGE_TESTS_IGNORE
	BEGIN_TEST_METHOD_ATTRIBUTE(Image_LoadImage_ShowImage)
	TEST_IGNORE()
	END_TEST_METHOD_ATTRIBUTE()
#endif
	TEST_METHOD(Image_LoadImage_ShowImage) {
		//Arrange

		//Act
		auto img = Image::LoadFile("france.png");

		//Assert
		ShowImage(img);
		Image::FreeImage(img);
	}

	TEST_METHOD(Image_GetPixels_GreenTint) {
		//Arrange
		auto img = CreateFranceFlag();
		auto w = img->GetWidth();
		auto h = img->GetHeight();
		Image::Pixel *begin{}, *end{};
		img->GetPixels(begin, end);
		Image::FreeImage(img);

		//Act
		for(auto p = begin; p < end; ++p) {
			if(p->r > 127)
				p->r = 127;
			p->g = 127;
			if(p->b > 127)
				p->b = 127;
		}

		img = Image::CreateFromPixels(begin, w, h);
		Image::FreePixels(begin);
		img->SaveFile("green.png");
		Image::FreeImage(img);

		//Assert
		img = Image::LoadFile("green.png");
		ShellExecute(0, TEXT("open"), TEXT("green.png"), 0, 0, SW_SHOW);
		Image::FreeImage(img);
	}
};
