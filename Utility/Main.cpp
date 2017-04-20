#include "PluginSDK.h"
#include "Utility.hpp"

/*
KUtility by Kätten
credits to Optix, Tsuhgi and jQuery
*/

PluginSetup("KUtility")

PLUGIN_EVENT(void) OnGameUpdate()
{
	g_pUtility->OnGameUpdate();
}

PLUGIN_EVENT(void) OnRender()
{
	g_pUtility->OnRender();
}

PLUGIN_EVENT(void) OnRender2()
{
	g_pUtility->OnRender2();
}

PLUGIN_EVENT(void) OnEnterFow(IUnit* unit)
{
	g_pUtility->OnEnterFow(unit);
}

PLUGIN_EVENT(void) OnExitFow(IUnit* unit)
{
	g_pUtility->OnExitFow(unit);
}

PLUGIN_EVENT(void) OnSpellCast(CastedSpell const& spell)
{
	g_pUtility->OnSpellCast(spell);
}

PLUGIN_EVENT(void) OnCreateObject(IUnit* object)
{
	g_pUtility->OnCreateObject(object);
}

PLUGIN_EVENT(void) OnDestroyObject(IUnit* object)
{
	g_pUtility->OnDestroyObject(object);
}

PLUGIN_EVENT(void) OnUnitDeath(IUnit* unit)
{
	g_pUtility->OnUnitDeath(unit);
}

PLUGIN_EVENT(void) OnBuffAdd(IUnit* source, void* data)
{
	g_pUtility->OnBuffAdd(source, data);
}

PLUGIN_EVENT(void) OnLevelUp(IUnit* source, int level)
{
	g_pUtility->OnLevelUp(source, level);
}

PLUGIN_EVENT(void) OnJungleNotify(JungleNotifyData* data)
{
	g_pUtility->OnJungleNotify(data);
}

PLUGIN_EVENT(void) OnTeleport(OnTeleportArgs* data)
{
	g_pUtility->OnTeleport(data);
}

PLUGIN_EVENT(bool) OnWndProc(HWND wnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	return g_pUtility->OnWndProc(wnd, message, wparam, lparam);
}

PLUGIN_API void OnLoad(IPluginSDK* sdk)
{
	PluginSDKSetup(sdk);

	SpellDb::LoadSpells();

	g_pUtility = new Utility(GEntityList->Player());

	GEventManager->AddEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->AddEventHandler(kEventOnRender, OnRender);
	GEventManager->AddEventHandler(kEventOnSpellCast, OnSpellCast);
	GEventManager->AddEventHandler(kEventOnCreateObject, OnCreateObject);
	GEventManager->AddEventHandler(kEventOnDestroyObject, OnDestroyObject);
	GEventManager->AddEventHandler(kEventOnUnitDeath, OnUnitDeath);
	GEventManager->AddEventHandler(kEventOnExitVisible, OnEnterFow);
	GEventManager->AddEventHandler(kEventOnEnterVisible, OnExitFow);
	GEventManager->AddEventHandler(kEventOnBuffAdd, OnBuffAdd);
	GEventManager->AddEventHandler(kEventOnLevelUp, OnLevelUp);
	GEventManager->AddEventHandler(kEventOnJungleNotification, OnJungleNotify);
	GEventManager->AddEventHandler(kEventOnWndProc, OnWndProc);
	GEventManager->AddEventHandler(kEventOnTeleport, OnTeleport);
}

PLUGIN_API void OnUnload()
{
	GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->RemoveEventHandler(kEventOnRender, OnRender);
	GEventManager->RemoveEventHandler(kEventOnSpellCast, OnSpellCast);
	GEventManager->RemoveEventHandler(kEventOnCreateObject, OnCreateObject);
	GEventManager->RemoveEventHandler(kEventOnDestroyObject, OnDestroyObject);
	GEventManager->RemoveEventHandler(kEventOnUnitDeath, OnUnitDeath);
	GEventManager->RemoveEventHandler(kEventOnExitVisible, OnEnterFow);
	GEventManager->RemoveEventHandler(kEventOnEnterVisible, OnExitFow);
	GEventManager->RemoveEventHandler(kEventOnBuffAdd, OnBuffAdd);
	GEventManager->RemoveEventHandler(kEventOnLevelUp, OnLevelUp);
	GEventManager->RemoveEventHandler(kEventOnJungleNotification, OnJungleNotify);
	GEventManager->RemoveEventHandler(kEventOnWndProc, OnWndProc);
	GEventManager->RemoveEventHandler(kEventOnTeleport, OnTeleport);

	_delete(g_pUtility);
}