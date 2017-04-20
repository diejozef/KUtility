#pragma once
#include <PluginSDK.h>

class Clicks
{
public:
	explicit Clicks(IMenu* parentMenu);
	~Clicks();

	auto OnRenderEnemy(IUnit* hero) -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pEnable;
};