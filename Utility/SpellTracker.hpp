#pragma once
#include <PluginSDK.h>
#include <KDrawing.hpp>
#include "CooldownManager.hpp"

class SpellTracker
{
public:
	explicit SpellTracker(IMenu* parentMenu, IUnit* player, CooldownManager* cooldownManager);
	~SpellTracker();

	auto OnRenderHero(IUnit* hero) -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pEnable;
	IMenuOption* m_pDrawTimer;
	IMenuOption* m_pDrawAllies;
	IMenuOption* m_pDrawXP;
	IMenuOption* m_pDrawSummoners;

private:
	IUnit* m_pPlayer;
	CooldownManager* m_pCooldownManager;

private:
	std::unordered_map<std::string, ITexture*> m_mapTextures;
};