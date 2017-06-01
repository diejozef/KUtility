#pragma once
#include <PluginSDK.h>
#include "InputManager.hpp"

class AutoPing
{
public:
	explicit AutoPing(IMenu* parentMenu, InputManager* inputManager);
	~AutoPing();

public:
	auto OnUpdate() -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pEnable;
	IMenuOption* m_pKey;
	IMenuOption* m_pPingType;

private:
	float m_flNextPingTime;

private:
	InputManager* m_pInputManager;
};