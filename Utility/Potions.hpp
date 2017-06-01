#pragma once
#include <PluginSDK.h>

enum ePotionItemId
{
	kHealthPotion = 2003,
	kTotalBiscuitOfRejuvenation = 2010,
	kRefillablePotion = 2031,
	kHuntersPotion,
	kCorruptingPotion
};

class Potions
{
public:
	explicit Potions(IMenu* parentMenu, IUnit* player);
	~Potions();

public:
	auto OnUpdate() -> void;

private:
	IUnit* m_pPlayer;
	std::array<std::tuple<ePotionItemId, const char*, IInventoryItem*>, 5> m_arrItems;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pEnable;
	IMenuOption* m_pAutoUseHp;
};