#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#include "Rect.hpp"

TEST_CLASS(RectTest) {
public:
	TEST_METHOD(MoveTo_Moved) {
		//Arrange
		Rect r {5, 10, 8, 7};

		//Act
		r.MoveTo({-3, -4});

		//Assert
		Assert::AreEqual(-3, r.x);
		Assert::AreEqual(-4, r.y);
		Assert::AreEqual(8, r.w);
		Assert::AreEqual(7, r.h);
	}

	TEST_METHOD(MoveBy_Moved) {
		//Arrange
		Rect r {5, 10, 8, 7};

		//Act
		r.MoveBy({-3, -4});

		//Assert
		Assert::AreEqual(2, r.x);
		Assert::AreEqual(6, r.y);
		Assert::AreEqual(8, r.w);
		Assert::AreEqual(7, r.h);
	}

	TEST_METHOD(ResizeTo_Bigger_Resized) {
		//Arrange
		Rect r{0, 0, 1280, 720};

		//Act
		r.ResizeTo({1664, 936});

		//Assert
		Assert::AreEqual(-192, r.x);
		Assert::AreEqual(-108, r.y);
		Assert::AreEqual(1664, r.w);
		Assert::AreEqual(936, r.h);
	}

	TEST_METHOD(ResizeTo_Smaller_Resized) {
		//Arrange
		Rect r{-192, -108, 1664, 936};

		//Act
		r.ResizeTo({1280, 720});

		//Assert
		Assert::AreEqual(0, r.x);
		Assert::AreEqual(0, r.y);
		Assert::AreEqual(1280, r.w);
		Assert::AreEqual(720, r.h);
	}

	TEST_METHOD(ResizeTo_SameSize_Resized) {
		//Arrange
		Rect r{0, 0, 1280, 720};

		//Act
		r.ResizeTo({1280, 720});

		//Assert
		Assert::AreEqual(0, r.x);
		Assert::AreEqual(0, r.y);
		Assert::AreEqual(1280, r.w);
		Assert::AreEqual(720, r.h);
	}

	TEST_METHOD(ZoomTo_Bigger_Resized) {
		//Arrange
		Rect r {0, 0, 1280, 720};

		//Act
		r.ZoomTo(1.3f);

		//Assert
		Assert::AreEqual(-192, r.x);
		Assert::AreEqual(-108, r.y);
		Assert::AreEqual(1664, r.w);
		Assert::AreEqual(936, r.h);
	}

	TEST_METHOD(ZoomTo_Smaller_Resized) {
		//Arrange
		Rect r{0, 0, 1280, 720};

		//Act
		r.ZoomTo(0.7f);

		//Assert
		Assert::AreEqual(192, r.x);
		Assert::AreEqual(108, r.y);
		Assert::AreEqual(896, r.w);
		Assert::AreEqual(504, r.h);
	}
};
