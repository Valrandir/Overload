#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "OverlordTests.hpp"
#include "../Overlord/BitmapGdi.hpp"
#include <type_traits>

TEST_CLASS(BitmapGdiTests) {
public:
	TEST_METHOD(IsCopyConstructible_False) {
		//Act
		auto actual = std::is_copy_constructible<BitmapGdi>::value;

		//Assert
		Assert::IsFalse(actual);
	}

	TEST_METHOD(IsCopyAssignable_False) {
		//Act
		auto actual = std::is_copy_assignable<BitmapGdi>::value;

		//Assert
		Assert::IsFalse(actual);
	}

	TEST_METHOD(IsMoveConstructible_True) {
		//Act
		auto actual = std::is_move_constructible<BitmapGdi>::value;

		//Assert
		Assert::IsTrue(actual);
	}

	TEST_METHOD(IsMoveAssignable_True) {
		//Act
		auto actual = std::is_move_assignable<BitmapGdi>::value;

		//Assert
		Assert::IsTrue(actual);
	}

	TEST_METHOD(MoveConstructed_Success) {
		//Arrange
		BitmapGdi bm(32, 32);

		auto width = bm.Width();
		auto height = bm.Height();
		auto dc = bm.GetDC();
		//auto bitmap = bm.GetBitmap();

		//Act
		BitmapGdi moved(std::move(bm));

		//Assert
		Assert::AreEqual(0, bm.Width());
		Assert::AreEqual(0, bm.Height());
		Assert::IsNull(bm.GetDC());

		Assert::AreEqual(width, moved.Width());
		Assert::AreEqual(height, moved.Height());
		Assert::IsTrue(dc == moved.GetDC());
	}

	TEST_METHOD(MoveAssigned_Success) {
		//Arrange
		BitmapGdi bm(32, 32);

		auto width = bm.Width();
		auto height = bm.Height();
		auto dc = bm.GetDC();

		//Act
		BitmapGdi moved(16, 16);
		moved = std::move(bm);

		//Assert
		Assert::AreEqual(0, bm.Width());
		Assert::AreEqual(0, bm.Height());
		Assert::IsNull(bm.GetDC());

		Assert::AreEqual(width, moved.Width());
		Assert::AreEqual(height, moved.Height());
		Assert::IsTrue(dc == moved.GetDC());
	}

	TEST_METHOD(Create_Save_Load_Show) {
		//Arrange
		auto filename = "Create_Save_Load_ShellOpen.png";
		COLORREF c[] = {
			{RGB(255, 0, 0)},
			{RGB(0, 255, 0)},
			{RGB(0, 0, 255)},
			{RGB(255, 255, 0)},
			{RGB(0, 255, 255)},
			{RGB(255, 0, 255)},
		};

		constexpr int width = 640, height = 640;
		BitmapGdi bm(width, height);
		constexpr int cc = sizeof(c) / sizeof(COLORREF);
		constexpr auto step_x = width / cc / 2;
		constexpr auto step_y = height / cc / 2;
		int x = 0, y = 0;
		int w = width, h = height;
		for(auto i = 0; i < cc; ++i) {
			bm.Fill(x, y, w, h, c[i]);
			x += step_x;
			y += step_y;
			w -= step_x * 2;
			h -= step_y * 2;
		}

		//Act
		bm.SavePNG(filename);
		BitmapGdi loaded = BitmapGdi::LoadPNG(filename);

		HDC hdc = GetDC(0);
		BitBlt(hdc, 0, 0, width, height, loaded.GetDC(), 0, 0, SRCCOPY);

		//Assert
	}
};
