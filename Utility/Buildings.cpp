#include "Buildings.hpp"

Buildings::Buildings(IMenu* parentMenu)
{
	m_pMenu = parentMenu->AddMenu("ku_buildings");
	m_pEnable = m_pMenu->CheckBox("Enable", false);
	m_pDrawTurrets = m_pMenu->CheckBox("Draw Turrets", false);
	m_pDrawInhibitors = m_pMenu->CheckBox("Draw Inhibitors", false);
	m_pOnlyEnemyTurrets = m_pMenu->CheckBox("Draw Only Enemy Turrets", false);

	auto inhibitors = GEntityList->GetAllInhibitors(true, true);
	std::for_each(inhibitors.begin(), inhibitors.end(), [&](IUnit* inhib)
	{
		m_vecInhibitors.emplace_back(inhibitor_t(inhib, inhib->GetHealth()));
	});

	m_flNextCheckTime = GGame->Time() + 3.0f;
}

Buildings::~Buildings()
{
	m_pMenu->Remove();
}

auto Buildings::OnUpdate() -> void
{
	if (!m_pEnable->Enabled())
		return;

	if (GGame->Time() > m_flNextCheckTime)
	{
		for (auto& inhib : m_vecInhibitors)
		{
			if (inhib.m_pSelf->GetHealth() <= 0.0f && inhib.m_flLastHp > 0.0f && !inhib.m_bDestroyed) // destroyed
			{
				inhib.m_bDestroyed = true;
				inhib.m_flRespawnTime = GGame->Time() + 300.0f;

			}
			else if (inhib.m_pSelf->GetHealth() > 0.0f && inhib.m_bDestroyed) // respawned
			{
				inhib.m_bDestroyed = false;
				inhib.m_flLastHp = inhib.m_pSelf->GetHealth();
			}
			else // alive
			{
				inhib.m_flLastHp = inhib.m_pSelf->GetHealth();
			}
		}

		m_flNextCheckTime = GGame->Time() + 3.0f;
	}
}

auto Buildings::OnRender() -> void
{
	if (!m_pEnable->Enabled())
		return;
	
	if (m_pDrawInhibitors->Enabled())
	{
		for (const auto& inhib : m_vecInhibitors)
		{
			if (inhib.m_bDestroyed)
			{
				auto w2s = Vec2(0.0f, 0.0f);
				if (GGame->Projection(inhib.m_pSelf->GetPosition(), &w2s))
				{
					auto timeUntilRespawn = inhib.m_flRespawnTime - GGame->Time();
					KDrawing::DrawString(w2s, Color::White(), true, "[%.0f]", timeUntilRespawn);
				}
			}
		}
	}

	if (m_pDrawTurrets->Enabled())
	{
		for (auto turret : GEntityList->GetAllTurrets(!m_pOnlyEnemyTurrets->Enabled(), true))
		{
			if (turret == nullptr || turret->IsInvulnerable())
				continue;

			auto w2m = Vec2(0.0f, 0.0f);
			if (GGame->WorldToMinimap(turret->GetPosition(), w2m))
			{
				w2m.x -= 3.0f;
				KDrawing::DrawString(w2m, Color::White(), true, "%.0f%%", turret->HealthPercent());
			}
		}
	}
}
