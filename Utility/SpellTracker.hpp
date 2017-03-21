#pragma once
#include <PluginSDK.h>
#include <KDrawing.hpp>

class SpellTracker
{
public:
	explicit SpellTracker( IMenu* parentMenu, IUnit* player );
	~SpellTracker();

	auto OnRenderHero( IUnit* hero ) -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pEnable;
	IMenuOption* m_pDrawTimer;
	IMenuOption* m_pDrawAllies;

	IUnit* m_pPlayer;
};