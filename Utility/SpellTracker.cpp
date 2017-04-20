#include "SpellTracker.hpp"

SpellTracker::SpellTracker(IMenu* parentMenu, IUnit* player, CooldownManager* cooldownManager) :
	m_pPlayer(player),
	m_pCooldownManager(cooldownManager)
{
	m_pMenu = parentMenu->AddMenu("Spell Tracker");
	m_pEnable = m_pMenu->CheckBox("Enable", false);
	m_pDrawTimer = m_pMenu->CheckBox("Draw Timer", false);
	m_pDrawAllies = m_pMenu->CheckBox("Draw Allies", false);
	m_pDrawXP = m_pMenu->CheckBox("Draw XP Progress", false);
	m_pDrawSummoners = m_pMenu->CheckBox("Draw Summoner Spells", false);

	char fileName[64]{ '\0' };
	auto summonerSpellName = std::string("");

	// load summoner spell icons with correct size
	auto enemies = GEntityList->GetAllHeros(false, true);
	std::for_each(enemies.begin(), enemies.end(), [&](IUnit* hero)
	{
		for (auto i = static_cast<int>(kSlotSummoner1); i <= static_cast<int>(kSlotSummoner2); i++)
		{
			summonerSpellName = std::string(hero->GetSpellName(i));
			sprintf_s(fileName, "%s.png", summonerSpellName.c_str());

			if (m_mapTextures.find(summonerSpellName) == m_mapTextures.end())
			{
				auto texture = GRender->CreateTextureFromFile(fileName);
				auto size = texture->GetSize();

				if (size.y < 1.0f || size.x < 1.0f)
				{
					char text[64];
					sprintf_s(text, "[KUtility/ST] Missing texture: %s", fileName);
					GGame->PrintChat(text);
				}
				else
				{
					texture->Resize(10, 10);
					m_mapTextures[summonerSpellName] = texture;
				}
			}
		}
	});

	std::array<std::pair<std::string, std::string>, 2> extraTextures{
		std::make_pair("S5_SummonerSmiteDuel", "S5_SummonerSmiteDuel.png"),
		std::make_pair("S5_SummonerSmitePlayerGanker", "S5_SummonerSmitePlayerGanker.png")
	};

	for (const auto& textureName : extraTextures)
	{
		if (m_mapTextures.find(std::get<0>(textureName)) != m_mapTextures.end())
			continue;

		auto texture = GRender->CreateTextureFromFile(std::get<1>(textureName).c_str());
		auto size = texture->GetSize();

		if (size.y < 1.0f || size.x < 1.0f)
		{
			char text[64];
			sprintf_s(text, "[KUtility/DM] Missing texture: %s", std::get<1>(textureName).c_str());
			GGame->PrintChat(text);
		}
		else
		{
			texture->Resize(10, 10);
			m_mapTextures[std::get<0>(textureName)] = texture;
		}
	}
}

SpellTracker::~SpellTracker()
{
	m_pMenu->Remove();
}

auto SpellTracker::OnRenderHero(IUnit* hero) -> void
{
	if (!m_pEnable->Enabled() || hero->IsDead() || !hero->IsVisible() || hero == m_pPlayer || !hero->IsHpBarBeingRendered())
		return;

	if (!m_pDrawAllies->Enabled() && !hero->IsEnemy(m_pPlayer))
		return;

	auto hpBarPos = Vec2(0.f, 0.f);
	if (hero->GetHPBarPosition(hpBarPos))
	{
		hpBarPos.x += 10;

		auto champName = hero->ChampionName();
		auto useOffset = !strcmp(champName, "Jhin") || !strcmp(champName, "Annie") || !strcmp(champName, "Corki");

		if (useOffset)
		{
			hpBarPos.x -= 8;
			hpBarPos.y -= 9;
		}

		if (m_pDrawXP->Enabled() && hero->GetLevel() < 18)
		{
			auto xp = hero->GetExperience();
			auto rxp = GUtility->GetExperienceRequiredForLevel(hero->GetLevel() + 1);
			auto pct = xp > 0.0f && rxp > 0.0f ? 1.0f - xp / rxp : 1.0f;

			GRender->DrawFilledBox(Vec2(hpBarPos.x, hpBarPos.y + 9), Vec2(120, 6), Color::Black().Get(200));
			GRender->DrawFilledBox(Vec2(hpBarPos.x + 1, hpBarPos.y + 10), Vec2(118 * pct, 3), Color::Yellow().Get());
		}

		if (m_pDrawSummoners->Enabled())
		{
			auto line = 0U;

			for (auto i = static_cast<int>(kSlotSummoner1); i <= static_cast<int>(kSlotSummoner2); i++)
			{
				auto spellName = std::string(hero->GetSpellName(i));
				if (m_mapTextures.find(spellName) != m_mapTextures.end())
				{
					auto pos = Vec2(hpBarPos.x + 132, hpBarPos.y + 18 + line * 11);
					if (useOffset)
						pos.y -= 7;

					auto cd = hero->GetSpellTotalCooldown(i);
					auto rcd = hero->GetSpellRemainingCooldown(i);
					auto rcd2 = m_pCooldownManager->GetRemainingCooldown(hero->GetNetworkId(), i);
					if (rcd2 > 0.0f)
						rcd = rcd2;

					auto pct = cd > 0.0f && rcd > 0.0f ? 1.0f - rcd / cd : 1.0f;

					m_mapTextures[spellName]->Draw(pos.x, pos.y);

					pos.x += 13;
					pos.y += 3;
					GRender->DrawFilledBox(Vec2(pos.x - 1, pos.y - 1), Vec2(30, 6), Color::Black().Get(200));

					if (rcd > 1.f)
					{
						GRender->DrawFilledBox(pos, Vec2(28 * pct, 4), Color::LightBlue().Get());
					}
					else
					{
						GRender->DrawFilledBox(pos, Vec2(28, 4), Color::LightGreen().Get());
					}
				}

				line++;
			}
		}

		hpBarPos.y += 38;

		auto column = 0;

		for (auto i = 0U; i < 4; i++)
		{
			auto cd = hero->GetSpellTotalCooldown(i);
			auto rcd = hero->GetSpellRemainingCooldown(i);

			auto spellbook = hero->GetSpellBook();
			if (spellbook->GetMaxAmmo(i) != -1 && spellbook->GetAmmo(i) == 0) // spell is recharging
				cd = spellbook->GetAmmoRechargeTime(i);

			auto percentage = cd > 0.0f && rcd > 0.0f ? 1.0f - rcd / cd : 1.0f;

			auto drawPos = hpBarPos;
			drawPos.x += column * 25;

			GRender->DrawFilledBox(Vec2(drawPos.x - 1, drawPos.y - 1), Vec2(23, 6), Color::Black().Get(200));

			if (hero->GetSpellLevel(i) > 0 && rcd > 1.f)
			{
				GRender->DrawFilledBox(drawPos, Vec2(21 * percentage, 4), Color::LightBlue().Get());

				if (m_pDrawTimer->Enabled())
				{
					auto offset = rcd < 10.0f ? 3 : rcd < 100.0f ? 9 : 11;
					KDrawing::DrawString(Vec2(drawPos.x + (11.5f - offset), drawPos.y + 6), Color::White(), true, "%.0f", rcd);
				}
			}
			else if (hero->GetSpellLevel(i) < 1)
			{
				GRender->DrawFilledBox(drawPos, Vec2(21, 4), Color::LightRed().Get());
			}
			else if (rcd < 1.f)
			{
				GRender->DrawFilledBox(drawPos, Vec2(21, 4), Color::LightGreen().Get());
			}

			column++;
		}
	}
}
