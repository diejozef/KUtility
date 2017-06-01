#pragma once
#include <PluginSDK.h>
#include "Smite.hpp"
#include "Ignite.hpp"
#include "Cleanse.hpp"
#include "AutoBuy.hpp"
#include "AutoLevelUp.hpp"
#include "Lantern.hpp"
#include "SpellBlock.hpp"
#include "Potions.hpp"
#include "InputManager.hpp"
#include "Support.hpp"
#include "Defensives.hpp"

class Activator
{
public:
	explicit Activator(IMenu* parentMenu, IUnit* player, InputManager* inputManager);
	~Activator();

	auto OnRender() -> void;
	auto OnUpdate() -> void;
	auto OnBuffAdd(IUnit* source, void* data) -> void;
	auto OnLevelUp(IUnit* source, int level) -> void;
	auto OnCreateObject(IUnit* object) -> void;
	auto OnDestroyObject(IUnit* object) -> void;
	auto OnSpellCast(const CastedSpell& spell) -> void;

private:
	IMenu* m_pMenu;
	IUnit* m_pPlayer;
	InputManager* m_pInputManager;

private:
	bool m_bHasSmite;
	bool m_bHasIgnite;
	bool m_bThresh;

private:
	Smite* m_pSmite;
	Ignite* m_pIgnite;
	Cleanse* m_pCleanse;
	AutoBuy* m_pAutoBuy;
	AutoLevelUp* m_pAutoLevelUp;
	Lantern* m_pLantern;
	SpellBlock* m_pSpellBlock;
	Potions* m_pPotions;
	Support* m_pSupport;
	Defensives* m_pDefensives;

private:
	auto GetSmite()->eSpellSlot;
	auto GetIgnite()->eSpellSlot;
};