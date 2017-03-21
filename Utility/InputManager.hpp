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

class InputManager
{
public:
	explicit InputManager();
	~InputManager();

	auto IsKeyPressed( uint8_t key ) -> bool;
	auto IsMouseButtonPressed( eMouseButton button ) -> bool;

	auto OnWndProc( HWND wnd, UINT message, WPARAM wparam, LPARAM lparam ) -> bool;

private:
	std::unordered_map<uint8_t, bool> m_mapKeyStates;
	std::unordered_map<eMouseButton, bool> m_mapMouseButtonStates;
};