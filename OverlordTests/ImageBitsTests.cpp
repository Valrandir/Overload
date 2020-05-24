#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../Overlord/ImageBits.hpp"
#include <type_traits>

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
		ib.Alloc(16, 16);

		auto width = ib.GetWidth();
		auto height = ib.GetHeight();
		auto begin = ib.begin();
		auto end = ib.end();

		//Act
		ImageBits moved(std::move(ib));

		//Assert
		Assert::AreEqual(0, ib.GetWidth());
		Assert::AreEqual(0, ib.GetHeight());
		Assert::IsNull(ib.begin());
		Assert::IsNull(ib.end());

		Assert::AreEqual(width, moved.GetWidth());
		Assert::AreEqual(height, moved.GetHeight());
		Assert::IsTrue(begin == moved.begin());
		Assert::IsTrue(end== moved.end());
	}

	TEST_METHOD(MoveAssigned_Success) {
		//Arrange
		ImageBits ib;
		ib.Alloc(16, 16);

		auto width = ib.GetWidth();
		auto height = ib.GetHeight();
		auto begin = ib.begin();
		auto end = ib.end();

		//Act
		ImageBits moved;
		moved = std::move(ib);

		//Assert
		Assert::AreEqual(0, ib.GetWidth());
		Assert::AreEqual(0, ib.GetHeight());
		Assert::IsNull(ib.begin());
		Assert::IsNull(ib.end());

		Assert::AreEqual(width, moved.GetWidth());
		Assert::AreEqual(height, moved.GetHeight());
		Assert::IsTrue(begin == moved.begin());
		Assert::IsTrue(end == moved.end());
	}
};
