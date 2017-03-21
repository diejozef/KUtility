#pragma once
#include "RecallTracker.hpp"
#include "FowTracker.hpp"

class RecallManager
{
public:
	explicit RecallManager( IMenu* parentMenu, IUnit* player, InputManager* inputManager, std::vector<FowTracker>* trackers );
	~RecallManager();

	auto OnRender() -> void;
	auto OnUpdate() -> void;
	auto OnTeleport( OnTeleportArgs* data ) -> void;

	auto HandleDrag() -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pEnable;
	IMenuOption* m_pUseChat;
	IMenuOption* m_pDrawHp;
	IMenuOption* m_pPosX;
	IMenuOption* m_pPosY;

private:
	InputManager* m_pInputManager;
	RecallTrackerBar m_bar;
	Vec2 m_vecScreenSize;
	IUnit* m_pPlayer;

private:
	std::vector<FowTracker>* m_pFowTrackers;
	std::vector<RecallTracker> m_vecTrackers;
};