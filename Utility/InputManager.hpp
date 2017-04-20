#pragma once
#include <PluginSDK.h>

enum eMouseButton : unsigned int
{
	kRButton,
	kLButton,
	kMButton,
	kX1Button,
	kX2Button,
	kMouseButtonMax
};

enum eInputEventType : unsigned int
{
	kInputEventTypeKeyDown,
	kInputEventTypeKeyUp
};

struct inputevent_t
{
public:
	explicit inputevent_t(eInputEventType type, const std::function<void()> fn) :
		m_type(type),
		m_fn(fn),
		m_bActive(true)
	{ }

	eInputEventType m_type;
	std::function<void()> m_fn;
	bool m_bActive;
};

class InputEventManager
{
public:
	explicit InputEventManager();
	~InputEventManager();

public:
	auto AddKeyEventListener(uint8_t key, eInputEventType type, const std::function<void()>& fn, size_t& id) -> void;
	auto RemoveKeyEventListener(uint8_t key, size_t id) -> void;
	auto AddMouseEventListener(eMouseButton button, eInputEventType type, const std::function<void()>& fn, size_t& id) -> void;
	auto RemoveMouseEventListener(eMouseButton button, size_t id) -> void;

public:
	auto OnKeyInput(uint8_t key, eInputEventType type) -> void;
	auto OnMouseInput(eMouseButton button, eInputEventType type) -> void;

private:
	std::unordered_map<uint8_t, std::vector<inputevent_t>> m_mapKeyEvents;
	std::unordered_map<eMouseButton, std::vector<inputevent_t>> m_mapMouseEvents;
};

class InputManager
{
public:
	explicit InputManager();
	~InputManager();

public:
	auto IsKeyPressed(uint8_t key) -> bool;
	auto IsMouseButtonPressed(eMouseButton button) -> bool;
	auto GetEventManager()->InputEventManager*;

public:
	auto OnWndProc(HWND wnd, UINT message, WPARAM wparam, LPARAM lparam) -> bool;

private:
	std::unordered_map<uint8_t, bool> m_mapKeyStates;
	std::unordered_map<eMouseButton, bool> m_mapMouseButtonStates;

private:
	InputEventManager* m_pEventManager;
};