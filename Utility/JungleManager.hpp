#pragma once
#include "JungleTracker.hpp"

class JungleManager
{
public:
	explicit JungleManager( IMenu* parentMenu );
	~JungleManager();

	auto OnRender() -> void;
	auto OnUpdate() -> void;
	auto OnJungleNotify( JungleNotifyData* data ) -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pFowTracker;
	IMenuOption* m_pUsePings;

	float m_flNextResetCheckTime;

	std::vector<JungleTracker> m_vecJungleTrackers;
};