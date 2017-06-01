#include "TurretRanges.hpp"

TurretRanges::TurretRanges(IMenu* parentMenu, IUnit* player) :
	m_pPlayer(player)
{
	m_pMenu = parentMenu->AddMenu("ku_turret_range");
	m_pEnable = m_pMenu->CheckBox("Enable", false);
	m_pDynamicColor = m_pMenu->CheckBox("Dynamic Color", false);
}

TurretRanges::~TurretRanges()
{
	m_pMenu->Remove();
}

auto TurretRanges::OnRender() -> void
{
	if (!m_pEnable->Enabled() || m_pPlayer->IsDead())
		return;

	for (auto turret : GEntityList->GetAllTurrets(false, true))
	{
		if (turret == nullptr || turret->IsDead() || turret->GetHealth() < 1.f)
			continue;

		auto range = turret->BoundingRadius() + 775.0f;
		auto pos = turret->GetPosition();

		if (strstr(turret->GetObjectName(), "Shrine"))
		{
			pos.y = 90.0f;
			range += 500.0f;
		}

		if (m_pDynamicColor->Enabled())
		{
			auto alpha = static_cast<uint8_t>(0);
			auto distanceSqr = (m_pPlayer->ServerPosition() - turret->GetPosition()).Length2DSqr();
			auto outerRangeSqr = _sqr(range * 2.1f);
			auto rangeSqr = _sqr(range);

			if (distanceSqr <= rangeSqr)
			{
				alpha = static_cast<uint8_t>(255);
			}
			else if (distanceSqr < outerRangeSqr)
			{
				auto scale = 1.0f - ((distanceSqr - rangeSqr) / (outerRangeSqr - rangeSqr));
				alpha = static_cast<uint8_t>(254 * scale);
			}

			GRender->DrawOutlinedCircle(pos, Color::Orange2().Get(alpha), range);
		}
		else
			GRender->DrawOutlinedCircle(pos, Color::Orange2().Get(100), range);
	}
}
