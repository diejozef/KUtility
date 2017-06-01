#pragma once
#include <PluginSDK.h>

enum eDefensiveItemId
{
	kSeraphsEmbrace = 3040,
	kZhonyasHourglass = 3157
};

class Defensives
{
public:
	explicit Defensives(IMenu* parentMenu, IUnit* player);
	~Defensives();

public:
	auto OnSpellCast(const CastedSpell& spell) -> void;
	auto OnUpdate() -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pUseZhonyas;
	IMenuOption* m_pUseSeraphs;
	IMenuOption* m_pUseHeal;
	IMenuOption* m_pUseBarrier;
	IMenuOption* m_pHp;

private:
	IUnit* m_pPlayer;
	IInventoryItem* m_pZhonyas;
	IInventoryItem* m_pSeraphs;
	ISpell2* m_pHeal;
	ISpell2* m_pBarrier;

private:
	bool m_bHasHeal;
	bool m_bHasBarrier;

private:
	auto GetHeal() -> eSpellSlot;
	auto GetBarrier() -> eSpellSlot;
	auto UseDefensives() -> void;
};