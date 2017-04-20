#pragma once
#include <PluginSDK.h>

class AARange
{
public:
	explicit AARange(IMenu* parentMenu, IUnit* player);
	~AARange();

	auto OnRender() -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pEnable;
	IMenuOption* m_pOnlyWhileOrbwalking;
	IMenuOption* m_pInCombo;
	IMenuOption* m_pInFarm;

private:
	IUnit* m_pPlayer;
};