#include "HotKeyMonitor.hpp"

int HotKeyMonitor::AddHotKey(HotKeyFunc callback, void* userdata, UINT virtualKeyCode)
{
	assert(callback);

	if(0 == RegisterHotKey(NULL, ++last_id, 0, virtualKeyCode))
		throw std::exception("HotKey registration failed"); //TODO: Reconsider using exceptions

	registered_hotkeys.emplace_back(last_id, callback, userdata);

	return last_id;
}

void HotKeyMonitor::RemoveHotKey(int id)
{
	auto it = std::find_if(registered_hotkeys.cbegin(), registered_hotkeys.cend(), [id](const HotKey& hk) { return hk.id == id; });
	if(it != registered_hotkeys.cend()) {
		UnregisterHotKey(NULL, it->id);
		registered_hotkeys.erase(it);
	}
}

void HotKeyMonitor::Dispatch()
{
	const HWND CURRENT_THREAD_MESSAGE_QUEUE = (HWND)-1;
	MSG msg;

	while(PeekMessage(&msg, CURRENT_THREAD_MESSAGE_QUEUE, WM_HOTKEY, WM_HOTKEY, PM_REMOVE))
		OnHotKey(LOWORD(msg.wParam));
}

HotKeyMonitor::~HotKeyMonitor()
{
	for(const auto& it : registered_hotkeys)
		UnregisterHotKey(NULL, it.id);
}

void HotKeyMonitor::OnHotKey(int id)
{
	auto it = std::find_if(registered_hotkeys.cbegin(), registered_hotkeys.cend(), [id](const HotKey& hk) { return hk.id == id; });
	if(it != registered_hotkeys.cend())
		it->callback(it->id, it->userdata);
}
