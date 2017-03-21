#pragma once
#include <PluginSDK.h>
#include <KDrawing.hpp>
#include "InputManager.hpp"
#include "FowTracker.hpp"
#include "SpellInfoBox.hpp"
#include "SpellTracker.hpp"
#include "Passives.hpp"
#include "LastPositions.hpp"
#include "BoundingBox.hpp"
#include "TurretRanges.hpp"
#include "Clicks.hpp"
#include "Clones.hpp"
#include "AARange.hpp"
#include "RecallManager.hpp"

class DrawManager
{
public:
	explicit DrawManager( IMenu* parentMenu, IUnit* player, InputManager* inputManager );
	~DrawManager();

	auto OnRender() -> void;
	auto OnUpdate() -> void;
	auto OnEnterFow( IUnit* unit ) -> void;
	auto OnExitFow( IUnit* unit ) -> void;
	auto OnUnitDeath( IUnit* unit ) -> void;
	auto OnTeleport( OnTeleportArgs* data ) -> void;
	auto OnRender2() -> void;

private:
	IMenu* m_pMenu;
	IUnit* m_pPlayer;

private:
	RecallManager* m_pRecallManager;
	InputManager* m_pInputManager;
	SpellInfoBox* m_pSpellInfoBox;
	SpellTracker* m_pSpellTracker;
	Passives* m_pPassives;
	LastPositions* m_pLastPositions;
	BoundingBox* m_pBoundingBox;
	TurretRanges* m_pTurretRanges;
	Clicks* m_pClicks;
	Clones* m_pClones;
	AARange* m_pAARange;

private:
	std::vector<FowTracker> m_vecFowTrackers;
};