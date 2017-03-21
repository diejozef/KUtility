#pragma once
#include <PluginSDK.h>
#include <SpellDatabase.hpp>

class Smite
{
public:
	explicit Smite( IMenu* parentMenu, eSpellSlot smiteSlot, IUnit* player );
	~Smite();

	auto OnUpdate() -> void;
	auto OnRender() -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pAutoSmite;
	IMenuOption* m_pUseSmiteCombo;
	IMenuOption* m_pDrawToggle;

private:
	SmiteInfo m_smiteComboInfo;
	bool m_bHasSmiteCombo;

private:
	ISpell2* m_pSmiteCombo;
	ISpell2* m_pSmite;
	IUnit* m_pPlayer;

private:
	auto AutoSmite() -> void;
	auto GetExtraSmiteDamage( IUnit* target ) -> float;
};