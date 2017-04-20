#include "AutoLevelUp.hpp"

AutoLevelUp::AutoLevelUp(IMenu* parentMenu, IUnit* player) :
	m_pPlayer(player)
{
	char text[32];
	sprintf_s(text, "Auto Level Up - %s", m_pPlayer->ChampionName());

	m_pMenu = parentMenu->AddMenu(text);
	m_pAutoLevelUp = m_pMenu->CheckBox("Enable", false);
	m_pStartLvl = m_pMenu->AddInteger("Start AutoLevelUp at", 1, 10, 3);
	m_pAutoLevelUpR = m_pMenu->CheckBox("Auto-level Ultimate", false);
	m_pFirst = m_pMenu->AddSelection("1st", 3, { "Q", "W", "E", "R" });
	m_pSecond = m_pMenu->AddSelection("2nd", 0, { "Q", "W", "E", "R" });
	m_pThird = m_pMenu->AddSelection("3rd", 0, { "Q", "W", "E", "R" });
	m_pFourth = m_pMenu->AddSelection("4th", 0, { "Q", "W", "E", "R" });
}

AutoLevelUp::~AutoLevelUp()
{
	m_pMenu->Remove();
}

auto AutoLevelUp::OnLevelUp(IUnit* source, int level) -> void
{
	if (source != m_pPlayer)
		return;

	if (m_pAutoLevelUpR->Enabled()
		&& GGame->CanLevelUpSpell(kSlotR)
		&& level == 6 || level == 11 || level == 16)
	{
		m_pPlayer->LevelUpSpell(kSlotR);
		return;
	}

	if (m_pAutoLevelUp->Enabled() && level >= m_pStartLvl->GetInteger())
	{
		std::array<int, 4> spells = {
			m_pFirst->GetInteger(),
			m_pSecond->GetInteger(),
			m_pThird->GetInteger(),
			m_pFourth->GetInteger()
		};

		if (std::count(spells.begin(), spells.end(), static_cast<int>(kSlotQ)) > 1
			|| std::count(spells.begin(), spells.end(), static_cast<int>(kSlotW)) > 1
			|| std::count(spells.begin(), spells.end(), static_cast<int>(kSlotE)) > 1
			|| std::count(spells.begin(), spells.end(), static_cast<int>(kSlotR)) > 1)
			return;

		for (auto spell : spells)
		{
			if (GGame->CanLevelUpSpell(spell))
				m_pPlayer->LevelUpSpell(spell);
		}
	}
}
