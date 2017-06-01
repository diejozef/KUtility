#include "Defensives.hpp"

Defensives::Defensives(IMenu* parentMenu, IUnit* player) :
	m_pPlayer(player)
{
	m_pMenu = parentMenu->AddMenu("ku_defensives");
	m_pUseZhonyas = m_pMenu->CheckBox("Use Zhonyas", false);
	m_pUseSeraphs = m_pMenu->CheckBox("Use Seraphs", false);
	m_pHp = m_pMenu->AddInteger("Use Items/Spells % HP", 1, 70, 20);

	m_pSeraphs = GPluginSDK->CreateItemForId(kSeraphsEmbrace, 0.0f);
	m_pZhonyas = GPluginSDK->CreateItemForId(kZhonyasHourglass, 0.0f);

	auto healSlot = GetHeal();
	m_bHasHeal = healSlot != kSlotUnknown;
	if (m_bHasHeal)
	{
		m_pUseHeal = m_pMenu->CheckBox("Use Heal", false);
		m_pHeal = GPluginSDK->CreateSpell2(healSlot, kTargetCast, false, false, kCollidesWithNothing);
	}

	auto barrierSlot = GetBarrier();
	m_bHasBarrier = barrierSlot != kSlotUnknown;
	if(m_bHasBarrier)
	{
		m_pUseBarrier = m_pMenu->CheckBox("Use Barrier", false);
		m_pBarrier = GPluginSDK->CreateSpell2(barrierSlot, kTargetCast, false, false, kCollidesWithNothing);
	}
}

Defensives::~Defensives()
{
	m_pMenu->Remove();
}

auto Defensives::OnSpellCast(const CastedSpell& spell) -> void
{
	if (m_pPlayer->IsDead())
		return;

	auto caster = spell.Caster_;
	auto target = spell.Target_;

	if (caster == nullptr || target == nullptr || !caster->IsHero() || target->GetNetworkId() != m_pPlayer->GetNetworkId())
		return;

	if (!caster->IsEnemy(m_pPlayer))
		return;

	auto slot = GSpellData->GetSlot(spell.Data_);
	if (slot >= kSlotQ && slot <= kSlotR)
	{
		auto dmg = GDamage->GetSpellDamage(caster, m_pPlayer, GSpellData->GetSlot(spell.Data_));
		if (dmg >= m_pPlayer->GetHealth())
			UseDefensives();
	}
	else if (spell.AutoAttack_)
	{
		auto dmg = GDamage->GetAutoAttackDamage(caster, m_pPlayer, true);
		if (dmg >= m_pPlayer->GetHealth())
			UseDefensives();
	}
}

auto Defensives::OnUpdate() -> void
{
	if (m_pPlayer->IsDead())
		return;

	if (!m_pUseZhonyas->Enabled() && !m_pUseSeraphs->Enabled())
		return;

	if (m_pPlayer->HealthPercent() <= static_cast<float>(m_pHp->GetInteger()))
		UseDefensives();
}

auto Defensives::GetHeal() -> eSpellSlot
{
	if (!strcmp(m_pPlayer->GetSpellName(kSlotSummoner1), "SummonerHeal"))
		return kSlotSummoner1;
	else if (!strcmp(m_pPlayer->GetSpellName(kSlotSummoner2), "SummonerHeal"))
		return kSlotSummoner2;

	return kSlotUnknown;
}

auto Defensives::GetBarrier() -> eSpellSlot
{
	if (!strcmp(m_pPlayer->GetSpellName(kSlotSummoner1), "SummonerBarrier"))
		return kSlotSummoner1;
	else if (!strcmp(m_pPlayer->GetSpellName(kSlotSummoner2), "SummonerBarrier"))
		return kSlotSummoner2;

	return kSlotUnknown;
}

auto Defensives::UseDefensives() -> void
{
	if (m_pPlayer->HasItemId(kZhonyasHourglass) && m_pZhonyas->IsReady() && m_pUseZhonyas->Enabled())
	{
		if (m_pZhonyas->CastOnPlayer())
			return;
	}

	if (m_pPlayer->HasItemId(kSeraphsEmbrace) && m_pSeraphs->IsReady() && m_pUseSeraphs->Enabled())
	{
		if (m_pSeraphs->CastOnPlayer())
			return;
	}

	if (m_bHasHeal && m_pHeal->IsReady() && m_pUseHeal->Enabled())
	{
		if (m_pHeal->CastOnPlayer())
			return;
	}

	if (m_bHasBarrier && m_pBarrier->IsReady() && m_pUseBarrier->Enabled())
	{
		if (m_pBarrier->CastOnPlayer())
			return;
	}
}
