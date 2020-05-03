#include "HotKeyMonitor.hpp"

int HotKeyMonitor::AddHotKey(HotKeyFunc callback, void* userdata, UINT virtualKeyCode)
{
	assert(callback);

	if(0 == RegisterHotKey(NULL, ++_last_id, 0, virtualKeyCode))
		throw std::exception("HotKey registration failed");

	_registeredHotKeys.emplace_back(_last_id, callback, userdata);

	return _last_id;
}

void HotKeyMonitor::RemoveHotKey(int id)
{
	auto it = std::find_if(_registeredHotKeys.cbegin(), _registeredHotKeys.cend(), [id](const HotKey& hk) { return hk.id == id; });
	if(it != _registeredHotKeys.cend()) {
		UnregisterHotKey(NULL, it->id);
		_registeredHotKeys.erase(it);
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
	for(const auto& it : _registeredHotKeys)
		UnregisterHotKey(NULL, it.id);
}

void HotKeyMonitor::OnHotKey(int id)
{
	auto it = std::find_if(_registeredHotKeys.cbegin(), _registeredHotKeys.cend(), [id](const HotKey& hk) { return hk.id == id; });
	if(it != _registeredHotKeys.cend())
		it->callback(it->id, it->userdata);
}
