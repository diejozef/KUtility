#pragma once
#include <PluginSDK.h>

class TurretRanges
{
public:
	explicit TurretRanges( IMenu* parentMenu, IUnit* player );
	~TurretRanges();

	auto OnRender() -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pEnable;
	IMenuOption* m_pDynamicColor;

	IUnit* m_pPlayer;
};