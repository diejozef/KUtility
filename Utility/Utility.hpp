#pragma once
#include "PluginSDK.h"
#include "KLib.hpp"
#include "SpellDatabase.hpp"
#include "InputManager.hpp"
#include "SpellManager.hpp"
#include "WardManager.hpp"
#include "DrawManager.hpp"
#include "Activator.hpp"
#include "AutoMute.hpp"
#include "JungleManager.hpp"
#include "SkinChanger.hpp"
#include "AutoPing.hpp"
#include "LastHitHelper.hpp"

class Utility
{
public:
	explicit Utility(IUnit* player);
	~Utility();

	auto OnRender() -> void;
	auto OnGameUpdate() -> void;
	auto OnSpellCast(CastedSpell const& spell) -> void;
	auto OnCreateObject(IUnit* object) -> void;
	auto OnDestroyObject(IUnit* object) -> void;
	auto OnUnitDeath(IUnit* unit) -> void;
	auto OnEnterFow(IUnit* unit) -> void;
	auto OnExitFow(IUnit* unit) -> void;
	auto OnBuffAdd(IUnit* source, void* data) -> void;
	auto OnLevelUp(IUnit* source, int level) -> void;
	auto OnJungleNotify(JungleNotifyData* data) -> void;
	auto OnTeleport(OnTeleportArgs* data) -> void;
	auto OnGetAutoAttackDamage(OnGetAutoAttackDamageArgs* data) -> float;
	auto OnWndProc(HWND wnd, UINT message, WPARAM wparam, LPARAM lparam) -> bool;

private:
	IMenu* m_pMenu;
	IUnit* m_pPlayer;

private:
	SpellManager* m_pSpellManager;
	WardManager* m_pWardManager;
	DrawManager* m_pDrawManager;
	JungleManager* m_pJungleManager;
	Activator* m_pActivator;
	AutoMute* m_pAutoMute;
	SkinChanger* m_pSkinChanger;
	InputManager* m_pInputManager;
	AutoPing* m_pAutoPing;
	LastHitHelper* m_pLastHitHelper;
};

extern Utility* g_pUtility;