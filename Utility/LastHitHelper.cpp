#include "LastHitHelper.hpp"

LastHitHelper::LastHitHelper(IMenu* parentMenu, IUnit* player) :
	m_pPlayer(player)
{
	m_pMenu = parentMenu->AddMenu("ku_lasthit_helper");
	m_pEnable = m_pMenu->CheckBox("Enable", false);
}

LastHitHelper::~LastHitHelper()
{
	m_pMenu->Remove();
}

auto LastHitHelper::OnGetAutoAttackDamage(OnGetAutoAttackDamageArgs* data) -> float
{
	if (!m_pEnable->Enabled())
		return data->OriginalDamage;

	auto target = data->Target;
	auto source = data->Source;

	if (source->GetNetworkId() != m_pPlayer->GetNetworkId() || !target->IsCreep())
		return data->OriginalDamage;

	auto dmg = data->OriginalDamage;

	if (m_pPlayer->HasBuff("LucianPassiveBuff"))
		dmg *= 1.9f;

	if (m_pPlayer->HasBuff("sheen"))
	{
		if (m_pPlayer->HasItemId(kTrinityForce))
			dmg += m_pPlayer->PhysicalDamage() * 1.9f;
		else if (m_pPlayer->HasItemId(kSheen))
			dmg += m_pPlayer->PhysicalDamage();
	}
	else if (m_pPlayer->HasBuff("ItemFrozenFist"))
		dmg += m_pPlayer->PhysicalDamage();
	else if (m_pPlayer->HasBuff("LichBane"))
		dmg += m_pPlayer->PhysicalDamage() * 0.75f + m_pPlayer->TotalMagicDamage() * 0.5f;

	if (m_pPlayer->HasItemId(kDoransShield))
		dmg += 5.0f;

	if (m_pPlayer->HasItemId(kRecurveBow))
		dmg += 15.0f;

	if(m_pPlayer->HasItemId(kGuinsoosRageblade))
		dmg += 15.0f;

	if(m_pPlayer->HasItemId(kBladeOfTheRuinedKing))
		dmg += 15.0f;

	if (m_pPlayer->HasItemId(kNashorsTooth))
		dmg += 15.0f;

	if (m_pPlayer->HasItemId(kWitsEnd))
		dmg += 40.0f;

	if (m_pPlayer->GetBuffCount("itemstatikshankcharge") == 100)
	{
		if (m_pPlayer->HasItemId(kKircheisShard) || m_pPlayer->HasItemId(kStatikkShiv) ||
			m_pPlayer->HasItemId(kRapidFirecannon))
			dmg += 35.0f;
	}

	std::vector<HeroMastery> masteries;
	m_pPlayer->GetMasteries(masteries);

	if (std::find_if(masteries.begin(), masteries.end(), [](const HeroMastery& mastery)
	{
		return mastery.PageId == 150 && mastery.MasteryId == 66;
	}) != masteries.end())
		dmg += 5.0f;

	return dmg;
}
