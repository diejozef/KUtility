#pragma once
#include <PluginSDK.h>

class Cleanse
{
public:
	explicit Cleanse( IMenu* parentMenu, IUnit* player );
	~Cleanse();

	auto OnUpdate() -> void;
	auto OnBuffAdd( IUnit* source, void* data ) -> void;

private:
	IMenu* m_pMenu;
	IMenu* m_pChampionFilter;
	IMenuOption* m_pAutoUse;
	IMenuOption* m_pDelay;

private:
	bool m_bHasCleanse;
	ISpell2* m_pCleanse;
	IUnit* m_pPlayer;

private:
	IInventoryItem* m_pQSS;
	IInventoryItem* m_pMerc;

private:
	std::function<void()> m_fnCastQss;
	std::function<void()> m_fnCastMerc;
	std::function<void()> m_fnCastCleanse;

private:
	auto AutoUse( void* data ) -> void;
	auto CanBeCleansed( eBuffType type ) -> bool;
	auto GetCleanse() -> eSpellSlot;
};