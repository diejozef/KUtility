#pragma once
#include <PluginSDK.h>
#include <KDrawing.hpp>
#include "InputManager.hpp"

class Lantern
{
public:
	explicit Lantern(IMenu* parentMenu, IUnit* player, InputManager* inputManager);
	~Lantern();

public:
	auto OnUpdate() -> void;
	auto OnRender() -> void;
	auto OnCreateObject(IUnit* object) -> void;
	auto OnDestroyObject(IUnit* object) -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pEnable;
	IMenuOption* m_pKey;
	IMenuOption* m_pAutoGrab;
	IMenuOption* m_pAutoGrabHpPct;
	IMenuOption* m_pDrawStatus;

private:
	IUnit* m_pPlayer;
	IUnit* m_pLantern;
	ISpell2* m_pInteract;
	InputManager* m_pInputManager;
};