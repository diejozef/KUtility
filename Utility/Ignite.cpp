#include "Ignite.hpp"

Ignite::Ignite(IMenu* parentMenu, eSpellSlot igniteSlot, IUnit* player) :
	m_pPlayer(player)
{
	m_pMenu = parentMenu->AddMenu("ku_auto_ignite");
	m_pAutoIgnite = m_pMenu->CheckBox("Auto Use", false);
	m_pAutoIgnitePercentage = m_pMenu->AddInteger("Auto Use on % HP", 0, 30, 0);

	m_pIgnite = GPluginSDK->CreateSpell2(igniteSlot, kTargetCast, false, false, kCollidesWithNothing);
	m_pIgnite->SetOverrideRange(600.0f);
}

Ignite::~Ignite()
{
	m_pMenu->Remove();
}

auto Ignite::OnUpdate() -> void
{
	if (m_pAutoIgnite->Enabled())
		AutoIgnite();
}

auto Ignite::AutoIgnite() -> void
{
	for (auto hero : GEntityList->GetAllHeros(false, true))
	{
		if (hero == nullptr || !m_pPlayer->IsValidTarget(hero, m_pIgnite->Range() + hero->BoundingRadius()))
			continue;

		auto menuPercentage = m_pAutoIgnitePercentage->GetInteger();

		if (menuPercentage > 0 && hero->HealthPercent() <= menuPercentage && m_pIgnite->CastOnUnit(hero))
			break;

		auto dmg = static_cast<float>(GDamage->GetSummonerSpellDamage(m_pPlayer, hero, kSummonerSpellIgnite));
		auto health = GHealthPrediction->GetPredictedHealth(hero, kLastHitPrediction, 250, 250);

		if (dmg > health && m_pIgnite->CastOnUnit(hero))
			break;
	}
}
