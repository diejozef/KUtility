#pragma once
#include <PluginSDK.h>
#include <KDrawing.hpp>
#include "FowTracker.hpp"

class LastPositions
{
public:
	explicit LastPositions(IMenu* parentMenu,
						   std::vector<FowTracker>* trackers,
						   std::unordered_map<std::string, ITexture*>* textures);
	~LastPositions();

	auto OnRenderEnemy(IUnit* hero) -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pEnable;
	IMenuOption* m_pDrawIcons;
	IMenuOption* m_pDraw3D;

private:
	std::vector<FowTracker>* m_pFowTrackers;
	std::unordered_map<std::string, ITexture*>* m_pTextures;
};