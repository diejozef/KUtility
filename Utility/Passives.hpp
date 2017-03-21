#pragma once
#include <PluginSDK.h>
#include <KDrawing.hpp>

class Passives
{
public:
	explicit Passives( IMenu* parentMenu );
	~Passives();

	auto OnRenderEnemy( IUnit* hero ) -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pEnable;
	IMenuOption* m_pDrawRebirthPassives;
	IMenuOption* m_pDrawPassives;
};