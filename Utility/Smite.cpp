#include "Smite.hpp"
#include <KLib.hpp>

Smite::Smite(IMenu* parentMenu, eSpellSlot smiteSlot, IUnit* player, InputManager* inputManager) :
	m_pPlayer(player),
	m_pInputManager(inputManager)
{
	m_pMenu = parentMenu->AddMenu("Smite");
	m_pAutoSmite = m_pMenu->CheckBox("Auto Use", false);
	m_pSmiteMajorBuffs = m_pMenu->CheckBox("Smite Major Buffs", false);
	m_pSmiteMajorJungleMobs = m_pMenu->CheckBox("Smite Major Mobs", false);
	m_pDrawToggle = m_pMenu->CheckBox("Draw Toggle Status", false);
	m_pToggleKey = m_pMenu->AddKey("Toggle Key", 123);
	m_pUseSmiteCombo = m_pMenu->CheckBox("[beta] Use Combo", false);

	m_iToggleKey = m_pToggleKey->GetInteger();
	m_pInputManager->GetEventManager()->AddKeyEventListener(m_iToggleKey, kInputEventTypeKeyDown, [&]() -> void
	{
		m_pAutoSmite->UpdateInteger(!m_pAutoSmite->GetInteger());
	}, m_iToggleEventId);

	m_pSmite = GPluginSDK->CreateSpell2(smiteSlot, kTargetCast, false, false, kCollidesWithNothing);
	m_pSmite->SetOverrideRange(m_pPlayer->BoundingRadius() + 500.0f); // temporary fix

	m_bHasSmiteCombo = SpellDb::GetSmiteSpell(m_pPlayer->ChampionName(), m_smiteComboInfo);
	if (m_bHasSmiteCombo)
	{
		m_pSmiteCombo = GPluginSDK->CreateSpell2(m_smiteComboInfo.m_slot, m_smiteComboInfo.m_type, false, false, kCollidesWithNothing);
		m_pSmiteCombo->SetOverrideRange(m_smiteComboInfo.m_flRange);
	}
}

Smite::~Smite()
{
	m_pMenu->Remove();
}

auto Smite::OnUpdate() -> void
{
	if (m_pAutoSmite->Enabled() && m_pSmite->IsReady())
		AutoSmite();

	// Handle hotkey change
	if (m_iToggleKey != m_pToggleKey->GetInteger())
	{
		m_pInputManager->GetEventManager()->RemoveKeyEventListener(m_iToggleKey, m_iToggleEventId);
		m_iToggleKey = m_pToggleKey->GetInteger();

		m_pInputManager->GetEventManager()->AddKeyEventListener(m_iToggleKey, kInputEventTypeKeyDown, [&]() -> void
		{
			m_pAutoSmite->UpdateInteger(!m_pAutoSmite->GetInteger());
		}, m_iToggleEventId);
	}
}

auto Smite::OnRender() -> void
{
	if (m_pDrawToggle->Enabled() && m_pAutoSmite->Enabled())
	{
		auto w2s = Vec2(0.f, 0.f);
		if (GGame->Projection(m_pPlayer->GetPosition(), &w2s))
		{
			w2s.y += 15.0f; // reserved the first line for evade or w/e
			KDrawing::DrawString(w2s, Color::White(), true, "[ AutoSmite ]");
		}
	}
}

auto Smite::AutoSmite() -> void
{
	for (auto minion : GEntityList->GetAllMinions(false, false, true))
	{
		if (minion == nullptr || minion->IsDead() || !KLib::InRange(m_pPlayer, minion, m_pSmite->Range(), true))
			continue;

		auto name = minion->GetObjectName();
		if (name == nullptr || name[0] == '\0')
			continue;

		auto useOnMajorBuff = m_pSmiteMajorBuffs->Enabled() && IsMajorBuff(name);
		auto useOnMajorJungleMob = m_pSmiteMajorJungleMobs->Enabled() && IsMajorMob(name);

		if (strstr(name, "SRU_Dragon") || strstr(name, "SRU_Baron") || strstr(name, "SRU_RiftHerald")
			|| useOnMajorBuff || useOnMajorJungleMob)
		{
			auto dmg = static_cast<float>(GDamage->GetSummonerSpellDamage(m_pPlayer, minion, kSummonerSpellSmite));
			auto health = minion->GetHealth();
			auto extraDmg = GetExtraSmiteDamage(minion);

			if (extraDmg < 1.0f && dmg >= health)
			{
				m_pSmite->CastOnUnit(minion);
				break;
			}
			else if (dmg + extraDmg > health)
			{
				m_smiteComboInfo.m_type == kTargetCast ? m_pSmiteCombo->CastOnUnit(minion) :
					m_pSmiteCombo->CastOnPosition(minion->GetPosition());
				break;
			}
		}
	}
}

auto Smite::GetExtraSmiteDamage(IUnit* target) -> float
{
	if (m_pUseSmiteCombo->Enabled()
		&& m_bHasSmiteCombo
		&& m_pSmiteCombo->IsReady()
		&& target->IsValidTarget() && KLib::InRange(m_pPlayer, target, m_pSmiteCombo->Range(), true))
		return static_cast<float>(GDamage->GetSpellDamage(m_pPlayer, target, m_smiteComboInfo.m_slot));
	else
		return 0.0f;
}

auto Smite::IsMajorBuff(const char* objectName) -> bool
{
	return strstr(objectName, "SRU_Blue") || strstr(objectName, "SRU_Red");
}

auto Smite::IsMajorMob(const char* objectName) -> bool
{
	if (strstr(objectName, "Mini"))
		return false;

	return strstr(objectName, "SRU_Krug") || strstr(objectName, "SRU_Murkwolf")
		|| strstr(objectName, "SRU_Gromp") || strstr(objectName, "SRU_Razorbeak");
}
