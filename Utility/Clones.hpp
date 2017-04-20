#pragma once
#include <PluginSDK.h>

class Clones
{
public:
	explicit Clones(IMenu* parentMenu);
	~Clones();

	auto OnRenderEnemy(IUnit* hero) -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pEnable;
};