#include "Lantern.hpp"

Lantern::Lantern(IMenu* parentMenu, IUnit* player, InputManager* inputManager) :
	m_pPlayer(player),
	m_pInputManager(inputManager),
	m_pLantern(nullptr)
{
	m_pMenu = parentMenu->AddMenu("ku_auto_lantern");
	m_pEnable = m_pMenu->CheckBox("Enable", false);
	m_pAutoGrab = m_pMenu->CheckBox("AutoGrab", false);
	m_pDrawStatus = m_pMenu->CheckBox("Draw AutoGrab Status", false);
	m_pAutoGrabHpPct = m_pMenu->AddInteger("AutoGrab % HP", 1, 50, 15);
	m_pKey = m_pMenu->AddKey("Grab Key", 123);

	m_pInteract = GPluginSDK->CreateSpell2(kSlotInteract, kTargetCast, false, false, kCollidesWithNothing);
}

Lantern::~Lantern()
{
	m_pMenu->Remove();
}

auto Lantern::OnUpdate() -> void
{
	if (!m_pEnable->Enabled() || m_pPlayer->IsDead() || m_pLantern == nullptr
		|| !m_pLantern->IsValidObject() || !m_pInteract->IsReady())
		return;

	auto shouldGrab = m_pAutoGrab->Enabled() &&
		m_pPlayer->HealthPercent() < static_cast<float>(m_pAutoGrabHpPct->GetInteger());

	if (shouldGrab || m_pInputManager->IsKeyPressed(static_cast<uint8_t>(m_pKey->GetInteger())))
	{
		auto distanceSqr = (m_pLantern->GetPosition() - m_pPlayer->ServerPosition()).Length2DSqr();
		if (distanceSqr > _sqr(500.0f))
			return;

		m_pInteract->CastOnUnit(m_pLantern);
	}
}

auto Lantern::OnRender() -> void
{
	if (!m_pEnable->Enabled() || !m_pDrawStatus->Enabled())
		return;

	auto w2s = Vec2(0.f, 0.f);
	if (GGame->Projection(m_pPlayer->GetPosition(), &w2s))
	{
		w2s.y += 15.0f * 2.0f;
		KDrawing::DrawString(w2s, Color::White(), true, "[ AutoLantern ]");
	}
}

auto Lantern::OnCreateObject(IUnit* object) -> void
{
	if (m_pLantern != nullptr)
		return;

	auto name = object->GetObjectName();
	if (name == nullptr || name[0] == '\0')
		return;

	if (!strcmp(name, "ThreshLantern"))
		m_pLantern = object;
}

auto Lantern::OnDestroyObject(IUnit* object) -> void
{
	if (m_pLantern == nullptr)
		return;

	if (object->GetNetworkId() == m_pLantern->GetNetworkId())
		m_pLantern = nullptr;
}
