#pragma once
#include <PluginSDK.h>
#include <SpellDatabase.hpp>
#include <KDrawing.hpp>
#include "InputManager.hpp"

class Smite
{
public:
	explicit Smite(IMenu* parentMenu, eSpellSlot smiteSlot, IUnit* player, InputManager* inputManager);
	~Smite();

	auto OnUpdate() -> void;
	auto OnRender() -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pAutoSmite;
	IMenuOption* m_pUseSmiteCombo;
	IMenuOption* m_pDrawToggle;
	IMenuOption* m_pToggleKey;
	IMenuOption* m_pSmiteMajorBuffs;
	IMenuOption* m_pSmiteMajorJungleMobs;

private:
	SmiteInfo m_smiteComboInfo;
	bool m_bHasSmiteCombo;
	size_t m_iToggleEventId;
	int m_iToggleKey;

private:
	ISpell2* m_pSmiteCombo;
	ISpell2* m_pSmite;
	IUnit* m_pPlayer;
	InputManager* m_pInputManager;

private:
	auto AutoSmite() -> void;
	auto GetExtraSmiteDamage(IUnit* target) -> float;
	auto IsMajorBuff(const char* objectName) -> bool;
	auto IsMajorMob(const char* objectName) -> bool;
};