#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vector>

class KeyOut {
public:
	static void SendText(const char* text);
	static void SendKeyInRandomOrder(std::vector<UINT> vk_list);
	static void SendKey(UINT virtual_key_code);
};
