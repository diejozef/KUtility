#include "Cleanse.hpp"

Cleanse::Cleanse(IMenu* parentMenu, IUnit* player) :
	m_pPlayer(player)
{
	m_pMenu = parentMenu->AddMenu("Cleanse/QSS");
	m_pAutoUse = m_pMenu->CheckBox("Auto Use", false);
	m_pUseInCombo = m_pMenu->CheckBox("Use Only in Combo", false);
	m_pDelay = m_pMenu->AddInteger("Delay (ms)", 0, 500, 10);

	m_pChampionFilter = m_pMenu->AddMenu("Champion Filter");

	auto enemies = GEntityList->GetAllHeros(false, true);
	std::for_each(enemies.begin(), enemies.end(), [&](IUnit* hero)
	{
		if (hero != nullptr)
			m_pChampionFilter->CheckBox(hero->ChampionName(), true);
	});

	m_pQSS = GPluginSDK->CreateItemForId(3140, 10.0f);
	m_pMerc = GPluginSDK->CreateItemForId(3139, 10.0f);

	auto slot = GetCleanse();
	m_bHasCleanse = (slot != kSlotUnknown);
	if (m_bHasCleanse)
		m_pCleanse = GPluginSDK->CreateSpell2(slot, kTargetCast, false, false, kCollidesWithNothing);
}

Cleanse::~Cleanse()
{
	m_pMenu->Remove();
}

auto Cleanse::OnUpdate() -> void
{

}

auto Cleanse::OnBuffAdd(IUnit* source, void* data) -> void
{
	if (source != m_pPlayer || !m_pAutoUse->Enabled())
		return;

	if (m_pUseInCombo->Enabled() && GOrbwalker->GetOrbwalkingMode() != kModeCombo)
		return;

	AutoUse(data);
}

auto Cleanse::AutoUse(void* data) -> void
{
	if (data == nullptr)
		return;

	auto hasQss = m_pPlayer->HasItemId(3140) && m_pQSS->IsReady();
	auto hasMerc = m_pPlayer->HasItemId(3139) && m_pMerc->IsReady();
	auto hasCleanse = m_bHasCleanse && m_pCleanse->IsReady();

	if (!hasQss && !hasMerc && !hasCleanse)
		return;

	auto caster = GBuffData->GetCaster(data);

	if (caster == nullptr || !caster->IsHero() || !m_pPlayer->IsEnemy(caster)
		|| !m_pChampionFilter->GetOption(caster->ChampionName())->Enabled())
		return;

	static std::array<std::string, 3> invalidStunCasters{
		"Hecarim", "Blitzcrank", "Thresh"
	};

	static std::array<std::string, 3> invalidSnareCasters{
		"Leona", "Lissandra", "Zyra"
	};

	auto type = GBuffData->GetBuffType(data);

	if (!CanBeCleansed(type))
		return;

	if (type == BUFF_Stun
		&& std::find(invalidStunCasters.begin(), invalidStunCasters.end(), caster->ChampionName()) != invalidStunCasters.end())
		return;

	if (type == BUFF_Snare
		&& std::find(invalidSnareCasters.begin(), invalidSnareCasters.end(), caster->ChampionName()) != invalidSnareCasters.end())
		return;

	auto delay = m_pDelay->GetInteger();

	if (hasQss)
	{
		if (delay < 1)
			m_pQSS->CastOnPlayer();
		else
			GPluginSDK->DelayFunctionCall(delay, [&]() -> void { m_pQSS->CastOnPlayer(); });
	}
	else if (hasMerc)
	{
		if (delay < 1)
			m_pMerc->CastOnPlayer();
		else
			GPluginSDK->DelayFunctionCall(delay, [&]() -> void { m_pMerc->CastOnPlayer(); });
	}
	else if (hasCleanse)
	{
		if (delay < 1)
			m_pCleanse->CastOnPlayer();
		else
			GPluginSDK->DelayFunctionCall(delay, [&]() -> void { m_pCleanse->CastOnPlayer(); });
	}
}

auto Cleanse::CanBeCleansed(eBuffType type) -> bool
{
	return (type == BUFF_Stun
			|| type == BUFF_Snare
			|| type == BUFF_Charm
			|| type == BUFF_Fear
			|| type == BUFF_Flee
			|| type == BUFF_Polymorph
			|| type == BUFF_Blind
			|| type == BUFF_Suppression);
}

auto Cleanse::GetCleanse() -> eSpellSlot
{
	return m_pPlayer->GetSpellSlot("SummonerBoost");
}
