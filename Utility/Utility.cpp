#include "Utility.hpp"

Utility* g_pUtility = nullptr;

Utility::Utility(IUnit* player) :
	m_pPlayer(player)
{
	m_pMenu = GPluginSDK->AddMenu("KUtility");

	m_pInputManager = new InputManager();

	m_pWardManager = new WardManager(m_pMenu, m_pPlayer);
	m_pDrawManager = new DrawManager(m_pMenu, m_pPlayer, m_pInputManager);
	m_pJungleManager = new JungleManager(m_pMenu);
	m_pSpellManager = new SpellManager(m_pMenu, m_pPlayer);
	m_pActivator = new Activator(m_pMenu, m_pPlayer, m_pInputManager);
	m_pAutoMute = new AutoMute(m_pMenu);
	m_pSkinChanger = new SkinChanger(m_pMenu, m_pPlayer);
	m_pAutoPing = new AutoPing(m_pMenu, m_pInputManager);
	m_pLastHitHelper = new LastHitHelper(m_pMenu, m_pPlayer);

	GRender->NotificationEx(Color::LightBlue().Get(), 2, true, true, "KUtility loaded!");

	if (!GUtility->DoesDirectoryExist("Textures\\KUtility"))
	{
		GGame->PrintChat("[KUtility/Ctor] Texture folder not found: Textures/KUtility");
		GGame->PrintChat("[KUtility/Ctor] Trying to create folder...");

		if (GUtility->CreateNewDirectory("Textures\\KUtility"))
		{
			GGame->PrintChat("[KUtility/Ctor] Folder has been successfully created.");
			GGame->PrintChat("[KUtility/Ctor] Please make sure you have all the texture files.");
		}
		else
			GGame->PrintChat("[KUtility/Ctor] Couldn't create folder, you'll have to do it manually.");
	}
}

Utility::~Utility()
{
	_delete(m_pWardManager);
	_delete(m_pDrawManager);
	_delete(m_pJungleManager);
	_delete(m_pSpellManager);
	_delete(m_pActivator);
	_delete(m_pAutoMute);
	_delete(m_pSkinChanger);
	_delete(m_pAutoPing);
	_delete(m_pLastHitHelper);
	_delete(m_pInputManager);

	m_pMenu->SaveSettings();
	m_pMenu->Remove();
}

auto Utility::OnGameUpdate() -> void
{
	m_pActivator->OnUpdate();
	m_pSpellManager->OnUpdate();
	m_pDrawManager->OnUpdate();
	m_pJungleManager->OnUpdate();
	m_pSkinChanger->OnUpdate();
	m_pAutoPing->OnUpdate();
}

auto Utility::OnRender() -> void
{
	m_pSpellManager->OnRender();
	m_pWardManager->OnRender();
	m_pDrawManager->OnRender();
	m_pActivator->OnRender();
	m_pJungleManager->OnRender();
}

auto Utility::OnCreateObject(IUnit* object) -> void
{
	if (object == nullptr)
		return;

	m_pSpellManager->OnCreateObject(object);
	m_pWardManager->OnCreateObject(object);
	m_pActivator->OnCreateObject(object);
}

auto Utility::OnDestroyObject(IUnit* object) -> void
{
	if (object == nullptr)
		return;

	m_pWardManager->OnDestroyObject(object);
	m_pSpellManager->OnDestroyObject(object);
	m_pActivator->OnDestroyObject(object);
}

auto Utility::OnUnitDeath(IUnit* unit) -> void
{
	if (unit == nullptr)
		return;

	m_pWardManager->OnUnitDeath(unit);
	m_pDrawManager->OnUnitDeath(unit);
}

auto Utility::OnEnterFow(IUnit* unit) -> void
{
	if (unit == nullptr)
		return;

	m_pWardManager->OnEnterFow(unit);
	m_pDrawManager->OnEnterFow(unit);
}

auto Utility::OnExitFow(IUnit* unit) -> void
{
	if (unit == nullptr)
		return;

	m_pDrawManager->OnExitFow(unit);
}

auto Utility::OnBuffAdd(IUnit* source, void* data) -> void
{
	if (source == nullptr || data == nullptr)
		return;

	m_pActivator->OnBuffAdd(source, data);
}

auto Utility::OnLevelUp(IUnit* source, int level) -> void
{
	if (source == nullptr)
		return;

	m_pActivator->OnLevelUp(source, level);
}

auto Utility::OnJungleNotify(JungleNotifyData* data) -> void
{
	m_pJungleManager->OnJungleNotify(data);
}

auto Utility::OnTeleport(OnTeleportArgs* data) -> void
{
	m_pDrawManager->OnTeleport(data);
}

auto Utility::OnGetAutoAttackDamage(OnGetAutoAttackDamageArgs* data) -> float
{
	return m_pLastHitHelper->OnGetAutoAttackDamage(data);
}

auto Utility::OnWndProc(HWND wnd, UINT message, WPARAM wparam, LPARAM lparam) -> bool
{
	return m_pInputManager->OnWndProc(wnd, message, wparam, lparam);
}

auto Utility::OnSpellCast(const CastedSpell& spell) -> void
{
	m_pActivator->OnSpellCast(spell);
	m_pSpellManager->OnSpellCast(spell);
	m_pWardManager->OnSpellCast(spell);
}