#include "SpellBlock.hpp"

static std::array<const char[3], 4> spellSlotNames{
	" Q", " W", " E", " R"
};

SpellBlock::SpellBlock(IMenu* parentMenu, IUnit* player, eSpellBlockType blockType) :
	m_pPlayer(player),
	m_blockType(blockType)
{
	char text[64]{ '\0' };
	strcpy(text, "ku_spell_blocker - ");
	strcat(text, player->ChampionName());
	m_pMenu = parentMenu->AddMenu(text);

	strcpy(text, "Enable - ");
	strcat(text, m_blockType == kSpellBlockFiora ? "Fiora" : "Sivir");
	m_pEnable = m_pMenu->CheckBox(text, false);
	
	m_pSpell = GPluginSDK->CreateSpell2(m_blockType == kSpellBlockFiora ? kSlotW : kSlotE, kTargetCast, false, false, kCollidesWithNothing);
	m_pBlockDatabase = new SpellBlockDatabase();

	auto enemies = GEntityList->GetAllHeros(false, true);
	std::for_each(enemies.begin(), enemies.end(), [&](IUnit* hero)
	{
		for (auto i = static_cast<int>(kSlotQ); i < static_cast<int>(kSlotSummoner1); i++)
		{
			auto spellName = hero->GetSpellName(i);

			if (m_pBlockDatabase->IsBlockableSpell(spellName) || 
				m_pBlockDatabase->IsBlockableAoESpell(spellName) || 
				IsSpecialSpell(spellName))
			{
				char text[32]{ '\0' };
				strcpy(text, hero->ChampionName());
				strcat(text, spellSlotNames[i]);

				m_pMenu->CheckBox(text, false);
			}
		}
	});

	m_vecBuffDatabase.emplace_back(std::make_tuple("Zed R", "zedrdeathmark", 0.5f)); // this buff expires at the right time
	m_vecBuffDatabase.emplace_back(std::make_tuple("Nocturne E", "NocturneUnspeakableHorror", 1.5f)); // internal duration is 3s, flee applies at around 0.9s
	m_vecBuffDatabase.emplace_back(std::make_tuple("Kled Q", "kledqmark", 0.5f));
	m_vecBuffDatabase.emplace_back(std::make_tuple("Fizz R", "fizzrbomb", 0.5f));
}

SpellBlock::~SpellBlock()
{
	_delete(m_pBlockDatabase);
}

auto SpellBlock::OnUpdate() -> void
{
	if (!m_pEnable->Enabled() || m_pPlayer->IsDead() || !m_pSpell->IsReady())
		return;

	for (auto missile : GEntityList->GetAllMissiles(false, true))
	{
		if (missile == nullptr || !GMissileData->GetCaster(missile)->IsHero() || GMissileData->IsAutoAttack(missile))
			continue;

		auto name = GMissileData->GetName(missile);
		if (m_pBlockDatabase->IsBlockableSpell(name))
		{
			auto info = m_pBlockDatabase->GetBlockableSpell(name);
			if (info.m_flWidth > 0.0f)
			{
				char text[32]{ '\0' };
				strcpy(text, info.m_strChampionName);
				strcat(text, spellSlotNames[info.m_slot]);

				if (!m_pMenu->GetOption(text)->Enabled())
					continue;

				auto distance = (m_pPlayer->ServerPosition() - missile->GetPosition()).Length2DSqr();
				if (distance < _sqr(info.m_flWidth))
				{
					auto caster = GMissileData->GetCaster(missile);
					if(caster != nullptr && caster->IsVisible())
						return BlockSpell(caster->ServerPosition());
					else
						return BlockSpell(GMissileData->GetStartPosition(missile));
				}
			}
		}
	}

	for (const auto& entry : m_vecBuffDatabase)
	{
		auto option = m_pMenu->GetOption(std::get<0>(entry));
		if (option != nullptr && option->Enabled())
		{
			if (m_pPlayer->HasBuff(std::get<1>(entry)))
			{
				auto data = m_pPlayer->GetBuffDataByName(std::get<1>(entry));
				if (data != nullptr)
				{
					auto duration = GBuffData->GetEndTime(data) - GGame->Time();
					if (duration < std::get<2>(entry))
						return BlockSpell(GBuffData->GetCaster(data)->GetPosition());
				}
			}
		}
	}

	auto jaxE = m_pMenu->GetOption("Jax E");
	if (jaxE != nullptr && jaxE->Enabled())
	{
		for (auto hero : GEntityList->GetAllHeros(false, true))
		{
			if (hero->IsDead() || !hero->IsVisible())
				continue;

			if (hero->HasBuff("JaxCounterStrike"))
			{
				auto endTime = GBuffData->GetEndTime(hero->GetBuffDataByName("JaxCounterStrike"));
				if (endTime - GGame->Time() < 0.7f)
				{
					auto distance = (m_pPlayer->ServerPosition() - hero->ServerPosition()).Length2DSqr();
					if (distance < _sqr(325.0f))
					{
						auto predPos = Vec3(0.0f, 0.0f, 0.0f);
						GPrediction->GetFutureUnitPosition(hero, 0.35f, true, predPos);

						return BlockSpell(predPos);
					}
				}
			}
		}
	}
}

auto SpellBlock::OnSpellCast(const CastedSpell& spell) -> void
{
	if (!m_pEnable->Enabled() || m_pPlayer->IsDead() || !m_pSpell->IsReady())
		return;

	auto caster = spell.Caster_;
	auto target = spell.Target_;

	if (caster == nullptr || !caster->IsHero() || !caster->IsEnemy(m_pPlayer))
		return;

	if (target != nullptr && target->GetNetworkId() == m_pPlayer->GetNetworkId())
	{
		if (m_pBlockDatabase->IsBlockableSpell(spell.Name_))
		{
			auto info = m_pBlockDatabase->GetBlockableSpell(spell.Name_);
			if (!info.m_bDummy && info.m_flWidth < 1.0f)
			{
				char text[32]{ '\0' };
				strcpy(text, caster->ChampionName());
				strcat(text, spellSlotNames[info.m_slot]);

				if (m_pMenu->GetOption(text)->Enabled())
					return BlockSpell(caster->ServerPosition());
			}
		}
	}

	if(!spell.AutoAttack_ && m_pBlockDatabase->IsBlockableAoESpell(spell.Name_))
	{
		auto casterName = caster->ChampionName();
		auto info = m_pBlockDatabase->GetBlockableAoESpell(spell.Name_);

		if (!info.m_bDummy)
		{
			char text[32]{ '\0' };
			strcpy(text, caster->ChampionName());
			strcat(text, spellSlotNames[info.m_slot]);

			if (m_pMenu->GetOption(text)->Enabled())
			{
				auto end = spell.EndPosition_;
				if (info.m_bSelfCast)
				{
					end = spell.Position_;
				}
				else
				{
					auto distanceSqr = (spell.EndPosition_ - spell.Position_).Length2DSqr();
					if (distanceSqr > _sqr(info.m_flRange))
					{
						auto dir = (spell.EndPosition_ - spell.Position_).VectorNormalize();
						end = caster->ServerPosition() + dir.Multiply2D(info.m_flRange);
					}
				}

				auto distanceSqr = (end - m_pPlayer->ServerPosition()).Length2DSqr();
				if (distanceSqr < _sqr(info.m_flRadius))
					return BlockSpell(spell.Position_);
			}
		}
	}

	if (!spell.AutoAttack_ && IsSpecialSpell(spell.Name_))
	{
		auto casterName = caster->ChampionName();
		auto slot = GSpellData->GetSlot(spell.Data_);

		char text[32]{ '\0' };
		strcpy(text, caster->ChampionName());
		strcat(text, spellSlotNames[slot]);

		if (m_pMenu->GetOption(text)->Enabled())
		{
			if (!strcmp(spell.Name_, "RivenTriCleave") && caster->GetBuffCount("RivenTriCleave") == 2)
			{
				auto endPos = spell.Position_ + caster->Direction() * 270.0f;
				auto distanceSqr = (m_pPlayer->ServerPosition() - endPos).Length2DSqr();
				if (distanceSqr < _sqr(250.0f))
					return BlockSpell(caster->ServerPosition());
			}
			else if (!strcmp(spell.Name_, "JaxEmpowerTwo"))
			{
				auto distanceSqr = (m_pPlayer->ServerPosition() - caster->ServerPosition()).Length2DSqr();
				if (distanceSqr <= _sqr(250.0f))
					return BlockSpell(caster->ServerPosition());
			}
		}
	}
	else if (spell.AutoAttack_ && caster->HasBuff("JaxEmpowerTwo") && target->GetNetworkId() == m_pPlayer->GetNetworkId())
	{
		auto jaxW = m_pMenu->GetOption("Jax W");
		if (jaxW != nullptr && jaxW->Enabled())
			return BlockSpell(caster->ServerPosition());
	}
	else if (spell.AutoAttack_ && m_pPlayer->HasBuff("AkaliMota") && !strcmp(caster->ChampionName(), "Akali"))
	{
		auto akaliQ = m_pMenu->GetOption("Akali Q");
		if (akaliQ != nullptr && akaliQ->Enabled())
			return BlockSpell(caster->ServerPosition());
	}

	if (!spell.AutoAttack_ && !strcmp(spell.Name_, "YasuoQ3W"))
	{
		auto yasQ = m_pMenu->GetOption("Yasuo Q");
		if (yasQ != nullptr && yasQ->Enabled())
		{
			auto distanceSqr = (spell.Position_ - m_pPlayer->ServerPosition()).Length2DSqr();
			if (distanceSqr < _sqr(220.0f))
				return BlockSpell(caster->ServerPosition());
		}
	}
}

auto SpellBlock::IsSpecialSpell(const char* spellName) -> bool
{
	return	!strcmp(spellName, "RivenTriCleave") || !strcmp(spellName, "JaxCounterStrike") || 
			!strcmp(spellName, "ZedR") || !strcmp(spellName, "NocturneUnspeakableHorror") ||
			!strcmp(spellName, "KledQ") || !strcmp(spellName, "FizzR") || 
			!strcmp(spellName, "JaxEmpowerTwo") || !strcmp(spellName, "AkaliMota");
}

auto SpellBlock::BlockSpell(const Vec3& ripostePos) -> void
{
	if (m_blockType == kSpellBlockFiora)
		m_pSpell->CastOnPosition(ripostePos);
	else
		m_pSpell->CastOnPlayer();
}
