#include "WardManager.hpp"

WardManager::WardManager(IMenu* parentMenu, IUnit* player) :
	m_pPlayer(player)
{
	m_pMenu = parentMenu->AddMenu("ku_ward_tracker");
	m_pDrawWards = m_pMenu->CheckBox("Draw Wards", false);
	m_pDrawRange = m_pMenu->CheckBox("Draw Vision Range", false);

	std::array<WardDefinition, 12> wardDefinitions = {
		WardDefinition("YellowTrinket" ,			"TrinketTotemLvl1" ,	60.0f,		kSightWard,		1100.0f),	// basic trinket
		WardDefinition("YellowTrinketUpgrade",		"TrinketTotemLvl2",		120.0f,		kSightWard,		1100.0f),	// this is probably obsolete
		WardDefinition("SightWard" ,				"ItemGhostWard" ,		150.0f,		kSightWard,		1100.0f),	// sightstone
		WardDefinition("SightWard" ,				"SightWard" ,			150.0f,		kSightWard,		1100.0f),	// sightstone
		WardDefinition("MissileWard" ,				"MissileWard" ,			180.0f,		kSightWard,		1100.0f),	// missile marker?
		WardDefinition("BlueTrinket" ,				"TrinketOrbLvl3" ,		9999.0f,	kBlueWard,		1100.0f),	// blue trinket (lvl9)
		WardDefinition("JammerDevice",				"JammerDevice",			9999.0f,	kPinkWard,		900.0f),	// control ward
		WardDefinition("TeemoMushroom" ,			"BantamTrap" ,			600.0f,		kTrap,			200.0f),	// teemo trap
		WardDefinition("Noxious_Trap" ,				"BantamTrap" ,			600.0f,		kTrap,			200.0f),	// teemo trap
		WardDefinition("ShacoBox" ,					"JackInTheBox" ,		60.0f,		kTrap,			200.0f),	// shaco trap
		WardDefinition("JhinTrap" ,					"JhinE" ,				120.0f,		kTrap,			130.0f),	// jhin trap
		WardDefinition("CaitlynTrap",				"CaitlynYordleTrap",	90.0f,		kCaitlynTrap,	50.0f)		// cait trap
	};

	std::for_each(wardDefinitions.begin(), wardDefinitions.end(), [&](const WardDefinition& definition)
	{
		m_mapWardObjectDefinitions.emplace(std::make_pair(definition.m_strObjectName, definition));
		m_mapWardSpellDefinitions.emplace(std::make_pair(definition.m_strSpellName, definition));
	});
}

WardManager::~WardManager()
{
	m_pMenu->Remove();
}

auto WardManager::OnRender() -> void
{
	if (!m_pDrawWards->Enabled())
		return;

	std::for_each(m_vecWards.begin(), m_vecWards.end(), [&](const Ward& ward)
	{
		if (ward.Valid())
			ward.Draw(m_pDrawRange->Enabled());
	});
}

auto WardManager::OnSpellCast(const CastedSpell& spell) -> void
{
	if (spell.AutoAttack_ || GSpellData->GetSlot(spell.Data_) < kSlotItem1)
		return;

	auto caster = spell.Caster_;
	if (caster == nullptr || !caster->IsHero() || !caster->IsEnemy(m_pPlayer))
		return;

	if (m_mapWardSpellDefinitions.find(std::string(spell.Name_)) != m_mapWardSpellDefinitions.end())
	{
		auto entry = m_mapWardSpellDefinitions[std::string(spell.Name_)];
		auto duration = entry.m_flDuration;

		if (caster->GetLevel() > 1 && strstr(spell.Name_, "TrinketTotem"))
			duration += 3.5f * caster->GetLevel();

		m_vecWards.emplace_back(Ward(spell.EndPosition_, entry, duration));
	}
}

auto WardManager::OnCreateObject(IUnit* object) -> void
{
	if (object->UnitFlags() != FL_CREEP || object->GetMaxHealth() > 200.0f)
		return;

	auto objectName = object->GetObjectName();
	if (objectName == nullptr || objectName[0] == '\0')
		return;

	if (!strcmp(objectName, "WardCorpse"))
	{
		HandleWardCorpse(object);
		return;
	}

	if (!object->IsEnemy(m_pPlayer))
		return;

	// only handle cait trap and pink ward(won't go invisible) here
	auto name = object->ChampionName();
	if (name == nullptr || name[0] == '\0')
		return;

	if (!strcmp(name, "CaitlynTrap") || !strcmp(name, "JammerDevice"))
	{
		auto entry = m_mapWardObjectDefinitions[std::string(name)];

		if (std::find_if(m_vecWards.begin(), m_vecWards.end(), [&](const Ward& ward) -> bool
		{
			if (!ward.Valid())
				return false;

			auto distanceSqr = (ward.Pos() - object->GetPosition()).Length2DSqr();
			auto maxDistance = ward.Info().m_type == kCaitlynTrap ? 75.0f : 125.0f;
			return distanceSqr < _sqr(maxDistance);

		}) == m_vecWards.end())
			m_vecWards.emplace_back(Ward(object->GetPosition(), entry, entry.m_flDuration, object->GetNetworkId()));
	}
}

auto WardManager::OnDestroyObject(IUnit* object) -> void
{
	if (object->UnitFlags() != FL_CREEP || !object->IsEnemy(m_pPlayer) || object->GetMaxHealth() > 200.0f)
		return;

	OnDestroy(object);
}

auto WardManager::OnUnitDeath(IUnit* object) -> void
{
	if (object->UnitFlags() != FL_CREEP || !object->IsEnemy(m_pPlayer) || object->GetMaxHealth() > 200.0f)
		return;

	OnDestroy(object);
}

auto WardManager::OnEnterFow(IUnit* object) -> void
{
	if (object->UnitFlags() != FL_CREEP || !object->IsEnemy(m_pPlayer) || object->GetMaxHealth() > 200.0f)
		return;

	auto name = object->ChampionName();
	if (name == nullptr || name[0] == '\0')
		return;

	if (m_mapWardObjectDefinitions.find(std::string(name)) != m_mapWardObjectDefinitions.end())
	{
		auto entry = m_mapWardObjectDefinitions[std::string(name)];

		auto& i = std::find_if(m_vecWards.begin(), m_vecWards.end(), [&](const Ward& ward) -> bool
		{
			if (!ward.Valid())
				return false;

			auto distanceSqr = (ward.Pos() - object->GetPosition()).Length2DSqr();
			return distanceSqr < _sqr(75.0f);

		});

		if(i == m_vecWards.end())
		{
			auto duration = (entry.m_type == kSightWard || entry.m_type == kTrap) ? object->GetMana() : entry.m_flDuration;
			m_vecWards.emplace_back(Ward(object->GetPosition(), entry, duration, object->GetNetworkId()));
		}
		else
		{
			if (entry.m_type == kSightWard || entry.m_type == kTrap)
				i->SetEndTime(GGame->Time() + object->GetMana());

			if (i->NetworkId() == -1)
				i->SetNetworkId(object->GetNetworkId());
		}
	}
}

auto WardManager::HandleWardCorpse(IUnit* corpse) -> void
{
	auto& i = std::find_if(m_vecWards.begin(), m_vecWards.end(), [&](const Ward& ward)
	{
		return ward.Valid() && ward.Pos() == corpse->GetPosition();
	});

	if (i != m_vecWards.end())
		i->Invalidate();
}

auto WardManager::OnDestroy(IUnit* object) -> void
{
	auto name = object->ChampionName();
	if (name == nullptr || name[0] == '\0')
		return;

	auto networkId = object->GetNetworkId();

	for (auto& ward : m_vecWards)
	{
		if (!ward.Valid())
			continue;

		if (networkId == ward.NetworkId())
		{
			ward.Invalidate();
			break;
		}

		if (!strcmp(ward.Info().m_strObjectName, name))
		{
			auto pos = object->GetPosition();
			if (pos == ward.Pos())
			{
				ward.Invalidate();
				break;
			}

			auto distanceSqr = (pos - ward.Pos()).Length2DSqr();
			auto maxDistance = ward.Info().m_type == kCaitlynTrap ? 75.0f : 125.0f;
			if (distanceSqr < _sqr(maxDistance))
			{
				ward.Invalidate();
				break;
			}
		}
	}
}
