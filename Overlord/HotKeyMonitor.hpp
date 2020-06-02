#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cassert>
#include <vector>

using HotKeyFunc = void (*)(int, void*);

class HotkeyMonitor {
public:
	int AddHotKey(HotKeyFunc callback, void* userdata, UINT modifiers, UINT virtual_key_code);
	void RemoveHotKey(int id);
	void Dispatch();
	~HotkeyMonitor();

private:
	struct HotKey {
		int id;
		HotKeyFunc callback;
		void* userdata;

		HotKey(int id, HotKeyFunc callback, void* userdata) :
			id{id}, callback{callback}, userdata{userdata} {}
	};

	HWND receiver_window{};
	std::vector<HotKey> registered_hotkeys;
	int last_id{};

public:
	void SetReceiverWindow(HWND receiver_window);
	void OnHotKey(int id);
};
