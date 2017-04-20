#pragma once
#include <PluginSDK.h>
#include <KDrawing.hpp>
#include "InputManager.hpp"
#include "FowTracker.hpp"
#include "CooldownManager.hpp"

class SpellInfoBox
{
public:
	explicit SpellInfoBox(IMenu* parentMenu,
						  std::unordered_map<int, FowTracker>* trackers,
						  std::unordered_map<std::string, ITexture*>* textures,
						  InputManager* inputManager,
						  CooldownManager* cooldownManager);
	~SpellInfoBox();

	auto OnUpdate() -> void;
	auto OnRender() -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pDraw;
	IMenuOption* m_pPosX;
	IMenuOption* m_pPosY;

private:
	int m_iWidth;
	int m_iHeight;
	int m_iLineHeight;

private:
	Vec2 m_vecScreenSize;
	InputManager* m_pInputManager;
	CooldownManager* m_pCooldownManager;

private:
	std::unordered_map<int, FowTracker>* m_pFowTrackers;
	std::unordered_map<std::string, ITexture*>* m_pTextures;
	std::unordered_map<int, std::array<const char*, 2>> m_mapSummoners;

	auto HandleDrag() const -> void;
};