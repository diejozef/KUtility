#pragma once
#include <PluginSDK.h>

class Ignite
{
public:
	explicit Ignite( IMenu* parentMenu, eSpellSlot igniteSlot, IUnit* player );
	~Ignite();

	auto OnUpdate() -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pAutoIgnite;
	IMenuOption* m_pAutoIgnitePercentage;

private:
	ISpell2* m_pIgnite;
	IUnit* m_pPlayer;

private:
	auto AutoIgnite() -> void;
};