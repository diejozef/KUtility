#include "AARange.hpp"

AARange::AARange(IMenu* parentMenu, IUnit* player) :
	m_pPlayer(player)
{
	m_pMenu = parentMenu->AddMenu("AutoAttack Range");
	m_pEnable = m_pMenu->CheckBox("Enable", false);
	m_pOnlyWhileOrbwalking = m_pMenu->CheckBox("Draw Only while Orbwalking", false);
}

AARange::~AARange()
{
	m_pMenu->Remove();
}

auto AARange::OnRender() -> void
{
	if (!m_pEnable->Enabled() || m_pPlayer->IsDead() || m_pPlayer->IsMelee())
		return;

	if (m_pOnlyWhileOrbwalking->Enabled() && GOrbwalker->GetOrbwalkingMode() == kModeNone)
		return;

	auto radius = m_pPlayer->BoundingRadius() + m_pPlayer->AttackRange();
	GRender->DrawCircle(m_pPlayer->GetPosition(), radius, Color::Yellow().Get(100), 2.0f, false, false);
}
