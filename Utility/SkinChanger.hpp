#pragma once
#include <PluginSDK.h>

class SkinChanger
{
public:
	explicit SkinChanger(IMenu* parentMenu, IUnit* player);
	~SkinChanger();

public:
	auto OnUpdate() -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pEnable;
	IMenuOption* m_pSkinId;
	IUnit* m_pPlayer;
};