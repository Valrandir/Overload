#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vector>
#include <cassert>

using HotKeyFunc = void (*)(int, void*);

class HotKeyMonitor {
public:
	int AddHotKey(HotKeyFunc callback, void* userdata, UINT virtualKeyCode);
	void RemoveHotKey(int id);
	void Dispatch();
	~HotKeyMonitor();

private:
	struct HotKey {
		int id;
		HotKeyFunc callback;
		void* userdata;

		HotKey(int id, HotKeyFunc callback, void* userdata)
			: id{id}, callback{callback}, userdata{userdata} {}
	};

	std::vector<HotKey> registeredHotKeys;
	int last_id{};

	void OnHotKey(int id);
};
