#include "SpellManager.hpp"

SpellManager::SpellManager(IMenu* parentMenu, IUnit* player) :
	m_pPlayer(player)
{
	m_pMenu = parentMenu->AddMenu("ku_spell_drawings");
	m_pDrawSkillshots = m_pMenu->CheckBox("Draw Skillshots", false);
	m_pDrawDashes = m_pMenu->CheckBox("Draw Dashes", false);
}

SpellManager::~SpellManager()
{
	m_pMenu->Remove();
}

auto SpellManager::OnRender() -> void
{
	if (!IsEnabled())
		return;

	if (m_pDrawSkillshots->Enabled())
	{
		std::for_each(m_vecSkillshots.begin(), m_vecSkillshots.end(), [](const SkillshotIndicator& skillshot)
		{
			if (skillshot.Valid())
				skillshot.Draw();
		});
	}

	if (m_pDrawDashes->Enabled())
	{
		std::for_each(m_vecDashes.begin(), m_vecDashes.end(), [](const DashIndicator& dash)
		{
			if (dash.Valid())
				dash.Draw();
		});
	}
}

auto SpellManager::OnUpdate() -> void
{
	if (!IsEnabled())
		return;

	for (auto missile : GEntityList->GetAllMissiles(false, true))
	{
		if (missile == nullptr || !GMissileData->GetCaster(missile)->IsHero() || GMissileData->IsAutoAttack(missile))
			continue;

		for (auto& skillshot : m_vecSkillshots)
		{
			if (!skillshot.Valid() || skillshot.MissileNetworkId() == -1)
				continue;

			if (skillshot.MissileNetworkId() == missile->GetNetworkId())
				skillshot.SetStartPos(missile->GetPosition().To2DEx());
		}
	}
}

auto SpellManager::OnSpellCast(const CastedSpell& spell) -> void
{
	if (!IsEnabled())
		return;

	if (spell.AutoAttack_ || !spell.Caster_->IsHero() || !spell.Caster_->IsEnemy(m_pPlayer))
		return;

	auto slot = static_cast<eSpellSlot>(GSpellData->GetSlot(spell.Data_));
	if (slot == kSlotUnknown || slot < kSlotQ || slot > kSlotSummoner2)
		return;

	if (!strcmp(spell.Name_, "VarusQ") || !strcmp(spell.Name_, "ViktorDeathRay"))
		return;

	DashInfo dashInfo;
	if (SpellDb::GetDash(spell.Name_, dashInfo))
	{
		auto start = spell.Position_;
		auto end = spell.EndPosition_;
		auto distanceSqr = (end - start).Length2DSqr();

		if (distanceSqr > _sqr(dashInfo.m_flRange))
		{
			auto dir = (end - start).VectorNormalize();
			end = start + dir.Multiply2D(dashInfo.m_flRange);
		}

		m_vecDashes.emplace_back(DashIndicator(start, end, 1.5f));
		return;
	}

	SpellInfo info;
	if (SpellDb::GetSpell(spell.Caster_->ChampionName(), spell.Name_, slot, info))
	{
		auto distanceSqr = (spell.EndPosition_ - spell.Position_).Length2DSqr();
		if (distanceSqr > _sqr(10.f))
		{
			auto duration = 1.5f;
			auto speed = info.m_flSpeed;
			auto range = m_pPlayer->BoundingRadius() + info.m_flRange;
			auto type = info.m_type;

			auto start = spell.Position_.To2DEx();
			auto end = spell.EndPosition_.To2DEx();
			auto dir = (end - start).VectorNormalize();

			if (type == kSpellSkillshotCircle)
			{
				auto castedRange = (end - start).LengthSqr();
				if (castedRange > _sqr(range))
					end = start + dir * range;
			}
			else if (type == kSpellSkillshotLine)
			{
				end = start + dir * range;
			}

			if (info.m_flSpeed > 0.0f && info.m_type == kSpellSkillshotLine)
				duration = info.m_flDelay + (range / speed);

			m_vecSkillshots.emplace_back(SkillshotIndicator(start, end, duration, info));
		}
	}
}

auto SpellManager::OnCreateObject(IUnit* object) -> void
{
	if (!IsEnabled())
		return;

	if (!object->IsMissile() || object->UnitFlags() != FL_MISSILE || GMissileData->IsAutoAttack(object))
		return;

	auto caster = GMissileData->GetCaster(object);
	if (caster == nullptr || !caster->IsHero() || !caster->IsEnemy(m_pPlayer))
		return;

	auto missileName = GMissileData->GetName(object);
	if (missileName == nullptr || missileName[0] == '\0')
		return;

	SpellInfo info;
	if (!SpellDb::GetSpellByMissile(missileName, info))
		return;

	auto& i = std::find_if(m_vecSkillshots.begin(), m_vecSkillshots.end(), [&](const SkillshotIndicator& skillshot)
	{
		if (!skillshot.Valid())
			return false;

		auto info = skillshot.Info();

		return (!_stricmp(info.m_strProjectileName, missileName)
				|| !_stricmp(info.m_strSpellName, missileName));
	});

	if (i == m_vecSkillshots.end())
	{
		auto duration = 1.5f;
		auto speed = info.m_flSpeed;
		auto range = m_pPlayer->BoundingRadius() + info.m_flRange;

		auto start = GMissileData->GetStartPosition(object).To2DEx();
		auto end = GMissileData->GetEndPosition(object).To2DEx();
		auto dir = (end - start).VectorNormalize();

		end = (info.m_type == kSpellSkillshotLine) ? start + dir * range : end;

		if (info.m_flSpeed > 0.0f && info.m_type == kSpellSkillshotLine)
			duration = range / info.m_flSpeed;

		m_vecSkillshots.emplace_back(SkillshotIndicator(start, end, duration, info, object->GetNetworkId()));
	}
	else if (i->MissileNetworkId() == -1)
	{
		i->SetMissileNetworkId(object->GetNetworkId());
	}
}

auto SpellManager::OnDestroyObject(IUnit* object) -> void
{
	if (!IsEnabled())
		return;

	if (!object->IsMissile() || object->UnitFlags() != FL_MISSILE
		|| !GMissileData->GetCaster(object)->IsHero()
		|| !GMissileData->GetCaster(object)->IsEnemy(m_pPlayer)
		|| GMissileData->IsAutoAttack(object))
		return;

	for (auto& skillshot : m_vecSkillshots)
	{
		if (!skillshot.Valid())
			continue;

		if (skillshot.MissileNetworkId() == object->GetNetworkId())
		{
			skillshot.Invalidate();
			break;
		}

		auto distanceSqr = (skillshot.StartPos() - object->GetPosition().To2DEx()).LengthSqr();
		if (distanceSqr < _sqr(15.0f) && !strcmp(skillshot.Info().m_strProjectileName, GMissileData->GetName(object)))
			skillshot.Invalidate();
	}
}

auto SpellManager::IsEnabled(void) const -> bool
{
	return m_pDrawSkillshots->Enabled() || m_pDrawDashes->Enabled();
}
