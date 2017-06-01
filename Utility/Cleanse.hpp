#pragma once
#include <PluginSDK.h>

class Cleanse
{
public:
	explicit Cleanse(IMenu* parentMenu, IUnit* player);
	~Cleanse();

	auto OnUpdate() -> void;
	auto OnBuffAdd(IUnit* source, void* data) -> void;

private:
	IMenu* m_pMenu;
	IMenu* m_pChampionFilter;
	IMenuOption* m_pAutoUse;
	IMenuOption* m_pDelay;
	IMenuOption* m_pUseInCombo;

private:
	bool m_bHasCleanse;
	bool m_bHasOranges;
	ISpell2* m_pCleanse;
	ISpell2* m_pOranges;
	IUnit* m_pPlayer;

private:
	IInventoryItem* m_pQSS;
	IInventoryItem* m_pMerc;

private:
	auto AutoUse(void* data) -> void;
	auto CanBeCleansed(eBuffType type) -> bool;
	auto GetCleanse()->eSpellSlot;
};