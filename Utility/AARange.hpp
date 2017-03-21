#pragma once
#include <PluginSDK.h>

class AARange
{
public:
	explicit AARange( IMenu* parentMenu, IUnit* player );
	~AARange();

	auto OnRender() -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pEnable;

	IUnit* m_pPlayer;
};