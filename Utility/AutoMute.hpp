#pragma once
#include <PluginSDK.h>

class AutoMute
{
public:
	explicit AutoMute() { }
	explicit AutoMute(IMenu* parentMenu);
	~AutoMute();

private:
	IMenu* m_pMenu;
	IMenuOption* m_pAutoMuteEnemies;
	IMenuOption* m_pAutoMuteAllies;
};