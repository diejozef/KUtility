#include "Support.hpp"
#include "KLib.hpp"

Support::Support(IMenu* parentMenu, IUnit* player) :
	m_pPlayer(player)
{
	m_pMenu = parentMenu->AddMenu("ku_support");
	m_pUseMikaels = m_pMenu->CheckBox("Use Mikaels", false);
	m_pUseLocket = m_pMenu->CheckBox("Use Locket", false);
	m_pUseFotm = m_pMenu->CheckBox("Use Face of the Mountain", false);
	m_pShieldHp = m_pMenu->AddInteger("Auto-FoTM % HP", 1, 60, 25);

	m_pMikaels = GPluginSDK->CreateItemForId(kMikaelsCrucible, 750.0f);
	m_pLocket = GPluginSDK->CreateItemForId(kLocketOfTheIronSolari, 600.0f);
	m_pFotm = GPluginSDK->CreateItemForId(kFaceOfTheMountain, 650.0f);

	m_pTargetChampions = m_pMenu->AddMenu("target_champions");
	auto allies = GEntityList->GetAllHeros(true, false);
	std::for_each(allies.begin(), allies.end(), [&](IUnit* ally)
	{
		m_pTargetChampions->CheckBox(ally->ChampionName(), false);
	});

	m_pSourceChampions = m_pMenu->AddMenu("source_champions");
	auto enemies = GEntityList->GetAllHeros(false, true);
	std::for_each(enemies.begin(), enemies.end(), [&](IUnit* enemy)
	{
		m_pSourceChampions->CheckBox(enemy->ChampionName(), false);
	});
}

Support::~Support()
{
	m_pMenu->Remove();
}

auto Support::OnBuffAdd(IUnit* source, void* data) -> void
{
	if (source == nullptr || data == nullptr || !m_pUseMikaels->Enabled())
		return;

	if (!m_pPlayer->HasItemId(kMikaelsCrucible) || !m_pMikaels->IsReady())
		return;

	if (!source->IsHero() || source->IsDead() || source->IsEnemy(m_pPlayer))
		return;

	auto type = GBuffData->GetBuffType(data);
	if (type != BUFF_Stun && type != BUFF_Snare &&
		type != BUFF_Taunt && type != BUFF_Flee &&
		type != BUFF_Fear && type != BUFF_Silence)
		return;

	if (!KLib::InRange(m_pPlayer, source, 750.0f, true))
		return;

	if (!m_pTargetChampions->GetOption(source->ChampionName())->Enabled())
		return;

	auto caster = GBuffData->GetCaster(data);
	if (caster == nullptr || !caster->IsHero() || !caster->IsEnemy(m_pPlayer) ||
		!m_pSourceChampions->GetOption(caster->ChampionName())->Enabled())
		return;

	m_pMikaels->CastOnTarget(source);
}

auto Support::OnUpdate() -> void
{
	if (m_pUseFotm->Enabled() && m_pPlayer->HasItemId(kFaceOfTheMountain) && m_pFotm->IsReady())
	{
		for (auto ally : GEntityList->GetAllHeros(true, false))
		{
			if (ally->HealthPercent() <= static_cast<float>(m_pShieldHp->GetInteger()) &&
				KLib::InRange(m_pPlayer, ally, 650.0f, true))
			{
				m_pFotm->CastOnTarget(ally);
				break;
			}
		}
	}

	if (m_pUseLocket->Enabled() && m_pPlayer->HasItemId(kLocketOfTheIronSolari) && m_pLocket->IsReady())
	{
		auto shieldStrength = 35.0f + m_pPlayer->GetLevel() * 35.0f;

		for (auto ally : GEntityList->GetAllHeros(true, false))
		{
			if (GHealthPrediction->GetPredictedHealth(ally, kLastHitPrediction, 200, 0) <= shieldStrength &&
				KLib::InRange(m_pPlayer, ally, 600.0f, true))
			{
				m_pLocket->CastOnPlayer();
				break;
			}
		}
	}
}
