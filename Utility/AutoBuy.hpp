#pragma once
#include <PluginSDK.h>

enum eWardItemId
{
	kWardingTotem = 3340,
	kFarsightAlteration = 3363,
	kSweepingLens = 3341,
	kOracleAlteration = 3364
};

class AutoBuy
{
public:
	explicit AutoBuy( IMenu* parentMenu, IUnit* player );
	~AutoBuy();

	auto OnUpdate() -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pAutoBuyBlueWard;
	IMenuOption* m_pAutoBuySweeper;
	IMenuOption* m_pUpgrade;

private:
	IUnit* m_pPlayer;

private:
	auto InFountain() -> bool;
};