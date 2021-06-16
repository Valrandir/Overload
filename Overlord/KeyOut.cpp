#include "KeyOut.hpp"
#include "Random.hpp"
#include <cctype>

void KeyOut::SendText(const char* text)
{
	UINT vk;
	while(*text) {
		switch (*text)
		{
			case '/': vk = VK_DIVIDE; break;
			case '*': vk = VK_MULTIPLY; break;
			case '-': vk = VK_SUBTRACT; break;
			case '+': vk = VK_ADD; break;
			default: vk = std::toupper(*text);
		}
		SendKey(vk);
		++text;
	}
}

void KeyOut::SendKeyInRandomOrder(std::vector<UINT> vk_list)
{
	Random::Shuffle(vk_list.begin(), vk_list.end());
	for(auto& it : vk_list)
		SendKey(it);
}

void KeyOut::SendKey(UINT virtual_key_code)
{
	INPUT in{INPUT_KEYBOARD};
	in.type = INPUT_KEYBOARD;
	in.ki.wVk = virtual_key_code;

	::SendInput(1, &in, sizeof(in));
	Sleep(Random::GetNext(5, 15));

	in.ki.dwFlags = KEYEVENTF_KEYUP;
	::SendInput(1, &in, sizeof(in));
	Sleep(Random::GetNext(5, 15));
}
