#pragma once
#include <PluginSDK.h>
#include <KDrawing.hpp>
#include "InputManager.hpp"
#include "FowTracker.hpp"

class SpellInfoBox
{
public:
	explicit SpellInfoBox( IMenu* parentMenu, std::vector<FowTracker>* trackers, InputManager* inputManager );
	~SpellInfoBox();

	auto OnUpdate() -> void;
	auto OnRender() -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pDraw;
	IMenuOption* m_pPosX;
	IMenuOption* m_pPosY;

	int m_iWidth;
	int m_iHeight;
	int m_iLineHeight;

	Vec2 m_vecScreenSize;

	InputManager* m_pInputManager;

	std::vector<FowTracker>* m_pTrackers;
	std::unordered_map<std::string, ITexture*> m_mapTextures;

	auto HandleDrag() -> void;
};