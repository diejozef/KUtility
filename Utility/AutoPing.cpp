#include "AutoPing.hpp"

AutoPing::AutoPing(IMenu* parentMenu, InputManager* inputManager) :
	m_pInputManager(inputManager),
	m_flNextPingTime(0.0f)
{
	m_pMenu = parentMenu->AddMenu("ku_auto_ping");
	m_pEnable = m_pMenu->CheckBox("Enable", false);
	m_pPingType = m_pMenu->AddSelection("Ping Type", kPingEnemyMissing - 2, { "Danger", "Enemy Missing", "On My Way", "Fall Back", "Assist Me" });
	m_pKey = m_pMenu->AddKey("Key", 123);
}

AutoPing::~AutoPing()
{
	m_pMenu->Remove();
}

auto AutoPing::OnUpdate() -> void
{
	if (!m_pEnable->Enabled() || GGame->IsChatOpen() || GGame->IsShopOpen() || !GUtility->IsLeagueWindowFocused())
		return;

	if (GGame->Time() > m_flNextPingTime && m_pInputManager->IsKeyPressed(static_cast<uint8_t>(m_pKey->GetInteger())))
	{
		GGame->SendPing(m_pPingType->GetInteger() + 2, GGame->CursorPosition());
		m_flNextPingTime = GGame->Time() + 0.35f;
	}
}
