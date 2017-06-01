#include "Potions.hpp"

Potions::Potions(IMenu* parentMenu, IUnit* player) :
	m_pPlayer(player)
{
	m_pMenu = parentMenu->AddMenu("ku_auto_potions");
	m_pEnable = m_pMenu->CheckBox("Enable", false);
	m_pAutoUseHp = m_pMenu->AddInteger("AutoUse % HP", 1, 70, 40);

	m_arrItems = {
		std::make_tuple(kHealthPotion, "RegenerationPotion", GPluginSDK->CreateItemForId(kHealthPotion, 0.0f)),
		std::make_tuple(kTotalBiscuitOfRejuvenation, "ItemMiniRegenPotion", GPluginSDK->CreateItemForId(kTotalBiscuitOfRejuvenation, 0.0f)),
		std::make_tuple(kRefillablePotion, "ItemCrystalFlask", GPluginSDK->CreateItemForId(kRefillablePotion, 0.0f)),
		std::make_tuple(kHuntersPotion, "ItemCrystalFlaskJungle", GPluginSDK->CreateItemForId(kHuntersPotion, 0.0f)),
		std::make_tuple(kCorruptingPotion, "ItemDarkCrystalFlask", GPluginSDK->CreateItemForId(kCorruptingPotion, 0.0f)),
	};
}

Potions::~Potions()
{
	m_pMenu->Remove();
}

auto Potions::OnUpdate() -> void
{
	if (!m_pEnable->Enabled() || m_pPlayer->IsDead())
		return;

	if (m_pPlayer->HealthPercent() > static_cast<float>(m_pAutoUseHp->GetInteger()))
		return;

	for (const auto& item : m_arrItems)
	{
		if (m_pPlayer->HasItemId(std::get<0>(item)) && !m_pPlayer->HasBuff(std::get<1>(item)) && std::get<2>(item)->IsReady())
		{
			std::get<2>(item)->CastOnPlayer();
			break;
		}
	}
}
