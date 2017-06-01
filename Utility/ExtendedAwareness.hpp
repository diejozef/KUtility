#pragma once
#include <PluginSDK.h>
#include <KDrawing.hpp>
#include "RecallManager.hpp"
#include "FowTracker.hpp"

class ExtendedAwareness
{
public:
	explicit ExtendedAwareness(IMenu* parentMenu, IUnit* player,
							   std::vector<FowTracker>* trackers,
							   std::unordered_map<std::string, ITexture*>* textures);
	~ExtendedAwareness();

public:
	auto OnRenderEnemy(IUnit* hero) -> void;
	auto OnRender() -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pEnable;
	IMenuOption* m_pMaxDistance;
	IMenuOption* m_pMinDistance;
	IMenuOption* m_pIndicatorDistance;
	IMenuOption* m_pWarningDistance;
	IMenuOption* m_pUseIcons;
	IMenuOption* m_pDrawLines;
	IMenuOption* m_pDrawIndicatorRange;
	IMenuOption* m_pIconMaxSize;
	IMenuOption* m_pIconMinSize;

private:
	IUnit* m_pPlayer;
	RecallManager* m_pRecallManager;

private:
	std::vector<FowTracker>* m_pFowTrackers;
	std::unordered_map<std::string, ITexture*>* m_pTextures;
};