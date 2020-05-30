#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "OverlordTests.hpp"
#include "../Overlord/ImageBits.hpp"
#include <type_traits>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h>

inline bool operator==(const ImageBits::Pixel& a, const ImageBits::Pixel& b)
{
	return a.b == b.b && a.g == b.g && a.r == b.r && a.a == b.a;
}

static ImageBits CreateTestFlag(int w, int h)
{
	auto flag = ImageBits::CreateEmpty(w, h);
	flag.Fill(0, 0, w / 3, h, ImageBits::CreatePixel(255, 0, 0));
	flag.Fill(w / 3, 0, w / 3, h, ImageBits::CreatePixel(255, 255, 255));
	flag.Fill(w / 3 * 2, 0, w / 3, h, ImageBits::CreatePixel(0, 0, 255));
	flag.Fill(w / 3 - w / 12, h / 2 - h / 4, w / 3 * 2 - w / 6, h / 6, ImageBits::CreatePixel(64, 160, 128, 224), ImageBits::BlendMode::Alpha);
	flag.Fill(w / 3 - w / 6, h / 2 + h / 4, w / 3 * 2, h / 6, ImageBits::CreatePixel(255, 255, 0, 160), ImageBits::BlendMode::Alpha);

	auto border_color = ImageBits::CreatePixel(255, 0, 255);
	auto border_size = 4;
	flag.Fill(0, 0, w, border_size, border_color); //Top
	flag.Fill(0, h - border_size, w, h, border_color); //Bottom
	flag.Fill(0, 0, border_size, h, border_color); //Left
	flag.Fill(w - border_size, 0, w, h, border_color); //Right
	return flag;
}

static void AssertPixel(const ImageBits& actual, int x, int y, const ImageBits::Pixel& color)
{
	Assert::IsTrue(*actual.GetPixel(x, y) == color);
}

static void AssertPixel(const ImageBits& actual, int x, int y, ImageBits::Byte r, ImageBits::Byte g, ImageBits::Byte b, ImageBits::Byte a = 255)
{
	AssertPixel(actual, x, y, ImageBits::CreatePixel(r, g, b, a));
}

TEST_CLASS(ImageBitsTests) {
public:
	TEST_METHOD(IsCopyConstructible_False) {
		//Act
		auto actual = std::is_copy_constructible<ImageBits>::value;

		//Assert
		Assert::IsFalse(actual);
	}

	TEST_METHOD(IsCopyAssignable_False) {
		//Act
		auto actual = std::is_copy_assignable<ImageBits>::value;

		//Assert
		Assert::IsFalse(actual);
	}

	TEST_METHOD(IsMoveConstructible_True) {
		//Act
		auto actual = std::is_move_constructible<ImageBits>::value;

		//Assert
		Assert::IsTrue(actual);
	}

	TEST_METHOD(IsMoveAssignable_True) {
		//Act
		auto actual = std::is_move_assignable<ImageBits>::value;

		//Assert
		Assert::IsTrue(actual);
	}

	TEST_METHOD(MoveConstructed_Success) {
		//Arrange
		ImageBits ib;
		ib.AllocBits(16, 16);

		auto begin = ib.begin();
		auto end = ib.end();
		auto width = ib.Width();
		auto height = ib.Height();

		//Act
		ImageBits moved(std::move(ib));

		//Assert
		Assert::IsNull(ib.begin());
		Assert::IsNull(ib.end());
		Assert::AreEqual(0, ib.Width());
		Assert::AreEqual(0, ib.Height());

		Assert::IsTrue(begin == moved.begin());
		Assert::IsTrue(end == moved.end());
		Assert::AreEqual(width, moved.Width());
		Assert::AreEqual(height, moved.Height());
	}

	TEST_METHOD(MoveAssigned_Success) {
		//Arrange
		ImageBits ib;
		ib.AllocBits(16, 16);

		auto width = ib.Width();
		auto height = ib.Height();
		auto begin = ib.begin();
		auto end = ib.end();

		//Act
		ImageBits moved;
		moved = std::move(ib);

		//Assert
		Assert::IsNull(ib.begin());
		Assert::IsNull(ib.end());
		Assert::AreEqual(0, ib.Width());
		Assert::AreEqual(0, ib.Height());

		Assert::IsTrue(begin == moved.begin());
		Assert::IsTrue(end == moved.end());
		Assert::AreEqual(width, moved.Width());
		Assert::AreEqual(height, moved.Height());
	}

	TEST_METHOD(Create_Clear_ColorMatch) {
		//Arrange
		auto filename = "Create_Clear_ColorMatch.png";
		auto w = 25;
		auto h = 16;
		auto clear_color = ImageBits::CreatePixel(64, 32, 128);

		//Act
		auto image_bits = ImageBits::CreateEmpty(w, h);
		image_bits.Clear(clear_color);
		image_bits.SaveFile(filename);

		//Assert
		auto picked_pixel = image_bits.GetPixel(w - 1, h - 1);
		Assert::IsTrue(*picked_pixel == clear_color);
		//ShellOpen(filename);
	}

	TEST_METHOD(Create_FillColor_ColorMatch) {
		//Arrange
		auto filename = "Create_FillColor_ColorMatch.png";
		auto w = 256;
		auto h = 160;
		auto clear_color = ImageBits::CreatePixel(0, 0, 0, 100);
		auto fill_color1 = ImageBits::CreatePixel(255, 0, 0, 200);
		auto fill_color2 = ImageBits::CreatePixel(0, 255, 0, 200);

		//Act
		auto image_bits = ImageBits::CreateEmpty(w, h);
		image_bits.Clear(clear_color);
		image_bits.Fill(w / 10, h / 10, w / 3, h / 3, fill_color1);
		image_bits.Fill(w / 5, h / 5, w / 3, h / 3, fill_color2, ImageBits::BlendMode::Alpha);
		image_bits.SaveFile(filename);

		//Assert
		auto actual = ImageBits::LoadFile(filename);
		AssertPixel(actual, 32, 32, 255, 0, 0, 200);
		AssertPixel(actual, 128, 48, 0, 230, 0, 221);
		AssertPixel(actual, 64, 48, 45, 209, 0, 243);
		//ShellOpen(filename);
	}

	TEST_METHOD(Create_FillImage_ColorMatch) {
		//Arrange
		auto flag_w = 300, flag_h = 150;
		auto bg_w = 600, bg_h = 300;
		auto filename = "Create_FillImage_ColorMatch.png";
		auto flag = CreateTestFlag(flag_w, flag_h);
		auto background = ImageBits::CreateEmpty(bg_w, bg_h);
		background.Clear(ImageBits::Pixel(64, 64, 64));

		//Act
		background.Fill((bg_w - flag_w) >> 1, (bg_h - flag_h) >> 1, flag); //Center
		background.Fill(flag_w / -2, flag_h / -2, flag); //Top Left
		background.Fill(bg_w - flag_w / 2, flag_h / -2, flag); //Top Right
		background.Fill(flag_w / -2, bg_h - flag_h / 2, flag); //Bottom Left
		background.Fill(bg_w - flag_w / 2, bg_h - flag_h / 2, flag); //Bottom Right
		background.SaveFile(filename);

		//Assert
		//ShellOpen(filename);
		auto pink = ImageBits::Pixel(255, 0, 255);
		auto red = ImageBits::Pixel(255, 0, 0);
		auto blue = ImageBits::Pixel(0, 0, 255);
		auto actual = ImageBits::LoadFile(filename);
		AssertPixel(actual, 145, 70, red);
		AssertPixel(actual, 146, 71, pink);
		AssertPixel(actual, 149, 74, pink);
		AssertPixel(actual, 150, 75, pink);
		AssertPixel(actual, 153, 78, pink);
		AssertPixel(actual, 154, 79, blue);

		AssertPixel(actual, 445, 220, red);
		AssertPixel(actual, 446, 221, pink);
		AssertPixel(actual, 449, 224, pink);
		AssertPixel(actual, 450, 225, pink);
		AssertPixel(actual, 453, 228, pink);
		AssertPixel(actual, 454, 229, blue);

		AssertPixel(actual, 225, 113, ImageBits::Pixel(112, 140, 87));
		AssertPixel(actual, 250, 137, ImageBits::Pixel(143, 171, 87));
		AssertPixel(actual, 374, 137, ImageBits::Pixel(143, 140, 56));
	}
};
