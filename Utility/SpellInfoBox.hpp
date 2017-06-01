#pragma once
#include <PluginSDK.h>
#include <KDrawing.hpp>
#include "InputManager.hpp"
#include "FowTracker.hpp"
#include "CooldownManager.hpp"

struct summoners_t
{
public:
	summoners_t() { }
	summoners_t(int networkId, std::array<const char*, 2> summoners) :
		m_iNetworkId(networkId),
		m_arrSummonerNames(summoners)
	{ }

	int m_iNetworkId;
	std::array<const char*, 2> m_arrSummonerNames;
};

class SpellInfoBox
{
public:
	explicit SpellInfoBox(IMenu* parentMenu,
						  std::vector<FowTracker>* trackers,
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
	std::vector<FowTracker>* m_pFowTrackers;
	std::vector<summoners_t> m_vecSummoners;
	std::unordered_map<std::string, ITexture*>* m_pTextures;

	auto HandleDrag() const -> void;
};