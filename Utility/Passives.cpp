#include "Passives.hpp"

Passives::Passives(IMenu* parentMenu)
{
	m_pMenu = parentMenu->AddMenu("ku_passive_tracker");
	m_pEnable = m_pMenu->CheckBox("Enable", false);
	m_pDrawPassives = m_pMenu->CheckBox("Draw Passives", false);
	m_pDrawRebirthPassives = m_pMenu->CheckBox("Draw Rebirth Passives", false);
	m_pDrawItemCooldowns = m_pMenu->CheckBox("Draw Item Cooldowns", false);
}

Passives::~Passives()
{
	m_pMenu->Remove();
}

auto Passives::OnRenderEnemy(IUnit* hero) -> void
{
	if (!m_pEnable->Enabled() || hero->IsDead() || !hero->IsVisible())
		return;

	static std::array<std::pair<const char*, const char*>, 4> passives{
		std::make_pair("ChronoShift",			"[CS: %.1f]"),
		std::make_pair("zhonyasringshield",		"[Zh: %.1f]"),
		std::make_pair("undyingrage",			"[R: %.1f]"),
		std::make_pair("chronorevive",			"[Revive: %.1f]")
	};

	static std::array<std::tuple<const char*, const char*, const char*>, 3> rebirthPassives{
		std::make_tuple("Anivia",		"rebirthready",			"rebirthcooldown"),
		std::make_tuple("Aatrox",		"aatroxpassiveready",	"aatroxpassiveactivate"),
		std::make_tuple("Zac",			"zacrebirthready",		"zacrebirthcooldown")
	};

	static std::array<std::pair<const char*, const char*>, 1> items{
		std::make_pair("GuardianAngel",		"[GA: %.1f]")
	};

	auto line = 0U;

	if (m_pDrawPassives->Enabled())
	{
		for (const auto& passive : passives)
		{
			if (hero->HasBuff(std::get<0>(passive)))
			{
				auto buff = hero->GetBuffDataByName(std::get<0>(passive));
				auto remainingTime = GBuffData->GetEndTime(buff) - GGame->Time();

				if (remainingTime > 0.1f)
				{
					auto w2s = Vec2(0.f, 0.f);
					if (GGame->Projection(hero->GetPosition(), &w2s))
					{
						KDrawing::DrawString(Vec2(w2s.x, w2s.y), Color::White(), true, std::get<1>(passive), remainingTime);
						line++;
					}
				}
			}
		}
	}

	if (m_pDrawItemCooldowns->Enabled())
	{
		for (const auto& item : items)
		{
			for (auto i = static_cast<int>(kSlotItem1); i <= static_cast<int>(kSlotItem6); i++)
			{
				if (!strcmp(hero->GetSpellName(i), std::get<0>(item)))
				{
					auto rcd = hero->GetSpellRemainingCooldown(i);
					if (rcd > 1.0f)
					{
						auto w2s = Vec2(0.f, 0.f);
						if (GGame->Projection(hero->GetPosition(), &w2s))
							KDrawing::DrawString(Vec2(w2s.x, w2s.y + line * 12), Color::Yellow(), true, std::get<1>(item), rcd);
					}
				}
			}
		}
	}

	if (m_pDrawRebirthPassives->Enabled())
	{
		for (const auto& passive : rebirthPassives)
		{
			if (!strcmp(hero->ChampionName(), std::get<0>(passive)))
			{
				auto w2s = Vec2(0.f, 0.f);
				if (GGame->Projection(hero->GetPosition(), &w2s))
				{
					auto passiveUp = hero->HasBuff(std::get<1>(passive));

					if (passiveUp)
					{
						KDrawing::DrawString(Vec2(w2s.x, w2s.y + line * 12), Color::LightGreen(), true, "[P]");
					}
					else
					{
						auto remainingTime = GBuffData->GetEndTime(hero->GetBuffDataByName(std::get<2>(passive))) - GGame->Time();
						KDrawing::DrawString(Vec2(w2s.x, w2s.y + line * 12), Color::Yellow(), true, "[P: %.0f]", remainingTime);
					}
				}
			}
		}
	}
}
