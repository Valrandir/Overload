#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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

		auto width = bm.GetWidth();
		auto height = bm.GetHeight();
		auto dc = bm.GetDC();
		auto bitmap = bm.GetBitmap();

		//Act
		BitmapGdi moved(std::move(bm));

		//Assert
		Assert::AreEqual(0, bm.GetWidth());
		Assert::AreEqual(0, bm.GetHeight());
		Assert::IsNull(bm.GetDC());
		Assert::IsNull(bm.GetBitmap());

		Assert::AreEqual(width, moved.GetWidth());
		Assert::AreEqual(height, moved.GetHeight());
		Assert::IsTrue(dc == moved.GetDC());
		Assert::IsTrue(bitmap == moved.GetBitmap());
	}

	TEST_METHOD(MoveAssigned_Success) {
		//Arrange
		BitmapGdi bm(32, 32);

		auto width = bm.GetWidth();
		auto height = bm.GetHeight();
		auto dc = bm.GetDC();
		auto bitmap = bm.GetBitmap();

		//Act
		BitmapGdi moved(16, 16);
		moved =std::move(bm);

		//Assert
		Assert::AreEqual(0, bm.GetWidth());
		Assert::AreEqual(0, bm.GetHeight());
		Assert::IsNull(bm.GetDC());
		Assert::IsNull(bm.GetBitmap());

		Assert::AreEqual(width, moved.GetWidth());
		Assert::AreEqual(height, moved.GetHeight());
		Assert::IsTrue(dc == moved.GetDC());
		Assert::IsTrue(bitmap == moved.GetBitmap());
	}
};
