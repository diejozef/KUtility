#pragma once
#include <PluginSDK.h>

class AutoLevelUp
{
public:
	explicit AutoLevelUp( IMenu* parentMenu, IUnit* player );
	~AutoLevelUp();

	auto OnLevelUp( IUnit* source, int level ) -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pAutoLevelUp;
	IMenuOption* m_pAutoLevelUpR;
	IMenuOption* m_pFirst;
	IMenuOption* m_pSecond;
	IMenuOption* m_pThird;
	IMenuOption* m_pFourth;

private:
	IUnit* m_pPlayer;
};