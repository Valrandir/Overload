#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../Overlord/HotKeyMonitor.hpp"

void HotKeyCallback(int id, void* userdata)
{
	auto cb_count = reinterpret_cast<int*>(userdata);
	++*cb_count;
}

TEST_CLASS(HotKeyMonitorTests) {
public:
	TEST_METHOD(Dispatch_TwoHotKeyPost_CallbackTwice) {
		//Arrange
		HotKeyMonitor hkm;
		int cb_count{};
		auto id = hkm.AddHotKey(HotKeyCallback, &cb_count, VK_NUMPAD5);
		PostMessage(NULL, WM_HOTKEY, id, 0);
		PostMessage(NULL, WM_HOTKEY, id, 0);

		//Act
		hkm.Dispatch();

		//Assert
		Assert::AreEqual(2, cb_count);
	}

	TEST_METHOD(Dispatch_HotKeyRemoved_DoNotCallback) {
		//Arrange
		HotKeyMonitor hkm;
		int cb_count{};
		auto id = hkm.AddHotKey(HotKeyCallback, &cb_count, VK_NUMPAD5);
		hkm.RemoveHotKey(id);
		PostMessage(NULL, WM_HOTKEY, id, 0);

		//Act
		hkm.Dispatch();

		//Assert
		Assert::AreEqual(0, cb_count);
	}
};
