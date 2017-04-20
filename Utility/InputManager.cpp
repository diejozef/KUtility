#include "InputManager.hpp"

#pragma warning(disable : 4800)

InputManager::InputManager()
{
	m_pEventManager = new InputEventManager();
}

InputManager::~InputManager()
{
	_delete(m_pEventManager);
}

auto InputManager::IsKeyPressed(uint8_t key) -> bool
{
	return m_mapKeyStates[key];
}

auto InputManager::IsMouseButtonPressed(eMouseButton button) -> bool
{
	if (button >= kMouseButtonMax)
		return false;

	return m_mapMouseButtonStates[button];
}

auto InputManager::GetEventManager() -> InputEventManager*
{
	return m_pEventManager;
}

auto InputManager::OnWndProc(HWND wnd, UINT message, WPARAM wparam, LPARAM lparam) -> bool
{
	switch (message)
	{
	case WM_KEYDOWN:
	{
		if (wparam > 255 || wparam < 0)
			break;

		const auto key = static_cast<uint8_t>(wparam);
		m_mapKeyStates[key] = true;

		// MSDN: 30th bit: The previous key state. The value is 1 if the key is down before the message is sent, or it is zero if the key is up.
		const auto prevKeyState = static_cast<bool>(lparam & (1 << 30));
		if (!prevKeyState)
			m_pEventManager->OnKeyInput(key, kInputEventTypeKeyDown);

		break;
	}
	case WM_KEYUP:
	{
		if (wparam > 255 || wparam < 0)
			break;

		const auto key = static_cast<uint8_t>(wparam);
		m_mapKeyStates[key] = false;
		m_pEventManager->OnKeyInput(key, kInputEventTypeKeyUp);

		break;
	}
	case WM_LBUTTONDOWN:
		m_mapMouseButtonStates[kLButton] = true;
		break;

	case WM_LBUTTONUP:
		m_mapMouseButtonStates[kLButton] = false;
		break;

	case WM_RBUTTONDOWN:
		m_mapMouseButtonStates[kRButton] = true;
		break;

	case WM_RBUTTONUP:
		m_mapMouseButtonStates[kRButton] = false;
		break;

	case WM_MBUTTONDOWN:
		m_mapMouseButtonStates[kMButton] = true;
		break;

	case WM_MBUTTONUP:
		m_mapMouseButtonStates[kMButton] = false;
		break;

	default:
		break;
	}

	return true;
}

InputEventManager::InputEventManager()
{
}

InputEventManager::~InputEventManager()
{
}

auto InputEventManager::AddKeyEventListener(uint8_t key, eInputEventType type, const std::function<void()>& fn, size_t& id) -> void
{
	m_mapKeyEvents[key].emplace_back(inputevent_t(type, fn));
	id = m_mapKeyEvents[key].size() - 1;
}

auto InputEventManager::RemoveKeyEventListener(uint8_t key, size_t id) -> void
{
	m_mapKeyEvents[key].at(id).m_bActive = false;
}

auto InputEventManager::AddMouseEventListener(eMouseButton button, eInputEventType type, const std::function<void()>& fn, size_t& id) -> void
{
	m_mapMouseEvents[button].emplace_back(inputevent_t(type, fn));
	id = m_mapMouseEvents[button].size() - 1;
}

auto InputEventManager::RemoveMouseEventListener(eMouseButton button, size_t id) -> void
{
	m_mapMouseEvents[button].at(id).m_bActive = false;
}

auto InputEventManager::OnKeyInput(uint8_t key, eInputEventType type) -> void
{
	auto entry = m_mapKeyEvents[key];

	if (entry.empty())
		return;

	std::for_each(entry.begin(), entry.end(), [&](const inputevent_t& e)
	{
		if (e.m_bActive && e.m_type == type)
			e.m_fn();
	});
}

auto InputEventManager::OnMouseInput(eMouseButton button, eInputEventType type) -> void
{
	auto entry = m_mapMouseEvents[button];

	if (entry.empty())
		return;

	std::for_each(entry.begin(), entry.end(), [&](const inputevent_t& e)
	{
		if (e.m_bActive && e.m_type == type)
			e.m_fn();
	});
}
