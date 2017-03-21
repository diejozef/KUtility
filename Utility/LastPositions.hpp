#pragma once
#include <PluginSDK.h>
#include <KDrawing.hpp>
#include "FowTracker.hpp"

class LastPositions
{
public:
	explicit LastPositions( IMenu* parentMenu, std::vector<FowTracker>* trackers );
	~LastPositions();

	auto OnRenderEnemy( IUnit* hero ) -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pEnable;
	IMenuOption* m_pDrawTimer;
	IMenuOption* m_pDrawIcons;

	std::vector<FowTracker>* m_pFowTrackers;
};