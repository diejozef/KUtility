#pragma once
#include <PluginSDK.h>
#include "Smite.hpp"
#include "Ignite.hpp"
#include "Cleanse.hpp"
#include "AutoBuy.hpp"
#include "AutoLevelUp.hpp"

class Activator
{
public:
	explicit Activator( IMenu* parentMenu, IUnit* player );
	~Activator();

	auto OnRender() -> void;
	auto OnUpdate() -> void;
	auto OnBuffAdd( IUnit* source, void* data ) -> void;
	auto OnLevelUp( IUnit* source, int level ) -> void;

private:
	IMenu* m_pMenu;
	IUnit* m_pPlayer;

private:
	bool m_bHasSmite;
	bool m_bHasIgnite;

private:
	Smite* m_pSmite;
	Ignite* m_pIgnite;
	Cleanse* m_pCleanse;
	AutoBuy* m_pAutoBuy;
	AutoLevelUp* m_pAutoLevelUp;

private:
	auto GetSmite() -> eSpellSlot;
	auto GetIgnite() -> eSpellSlot;
};