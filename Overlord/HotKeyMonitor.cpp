#include "HotkeyMonitor.hpp"

int HotkeyMonitor::AddHotKey(HotKeyFunc callback, void* userdata, UINT modifiers, UINT virtual_key_code)
{
	assert(callback);

	if(0 == RegisterHotKey(receiver_window, ++last_id, modifiers, virtual_key_code))
		throw std::exception("HotKey registration failed"); //TODO: Reconsider using exceptions

	registered_hotkeys.emplace_back(last_id, callback, userdata);

	return last_id;
}

void HotkeyMonitor::RemoveHotKey(int id)
{
	auto it = std::find_if(registered_hotkeys.cbegin(), registered_hotkeys.cend(), [id](const HotKey& hk) { return hk.id == id; });
	if(it != registered_hotkeys.cend()) {
		UnregisterHotKey(NULL, it->id);
		registered_hotkeys.erase(it);
	}
}

void HotkeyMonitor::Dispatch()
{
	const HWND CURRENT_THREAD_MESSAGE_QUEUE = (HWND)-1;
	MSG msg;

	while(PeekMessage(&msg, CURRENT_THREAD_MESSAGE_QUEUE, WM_HOTKEY, WM_HOTKEY, PM_REMOVE))
		OnHotKey(LOWORD(msg.wParam));
}

HotkeyMonitor::~HotkeyMonitor()
{
	for(const auto& it : registered_hotkeys)
		UnregisterHotKey(NULL, it.id);
}

void HotkeyMonitor::SetReceiverWindow(HWND receiver_window)
{
	this->receiver_window = receiver_window;
}

void HotkeyMonitor::OnHotKey(int id)
{
	auto it = std::find_if(registered_hotkeys.cbegin(), registered_hotkeys.cend(), [id](const HotKey& hk) { return hk.id == id; });
	if(it != registered_hotkeys.cend())
		it->callback(it->id, it->userdata);
}
