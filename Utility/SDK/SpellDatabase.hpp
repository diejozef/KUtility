#pragma once
#include "PluginSDK.h"

enum eSpellInfoType
{
	kSpellInfoInvalid = -1,
	kSpellSkillshotLine,
	kSpellSkillshotCircle,
	kSpellTarget
};

enum eSpellInfoDanger
{
	kSpellDangerLow,
	kSpellDangerMedium,
	kSpellDangerHigh,
	kSpellDangerVeryHigh
};

struct SmiteInfo
{
	explicit SmiteInfo() { }
	explicit SmiteInfo(const char* championName, eSpellSlot slot, eSpellType type, float range, float delay) :
		m_strChampionName(championName),
		m_slot(slot),
		m_type(type),
		m_flRange(range),
		m_flDelay(delay)
	{ }

	const char* m_strChampionName;
	eSpellSlot m_slot;
	eSpellType m_type;
	float m_flRange;
	float m_flDelay;
};

struct DashInfo
{
	explicit DashInfo() { }
	explicit DashInfo(const char* spellName, float range) :
		m_strSpellName(spellName),
		m_flRange(range)
	{ }

	const char* m_strSpellName;
	float m_flRange;
};

struct SpellInfo
{
	explicit SpellInfo() { }
	explicit SpellInfo(
		const char* champName,
		const char* spellName,
		const char* projectileName,
		eSpellSlot slot,
		eSpellInfoType type,
		float delay,
		float range,
		float radius,
		float speed,
		const std::vector<const char*>& extraSpellNames,
		const std::vector<const char*>& extraProjectileNames
	) :
		m_strChampionName(champName),
		m_strSpellName(spellName),
		m_strProjectileName(projectileName),
		m_slot(slot),
		m_type(type),
		m_flDelay(delay),
		m_flRange(range),
		m_flRadius(radius),
		m_flSpeed(speed),
		m_vecExtraSpellNames(extraSpellNames),
		m_vecExtraProjectileNames(extraProjectileNames)
	{ }

	const char* m_strChampionName;
	const char* m_strSpellName;
	const char* m_strProjectileName;
	eSpellSlot m_slot;
	eSpellInfoType m_type;
	float m_flDelay;
	float m_flRange;
	float m_flRadius;
	float m_flSpeed;
	std::vector<const char*> m_vecExtraSpellNames;
	std::vector<const char*> m_vecExtraProjectileNames;
};

namespace SpellDb
{
	auto GetSpell(const char* champName, const char* spellName, eSpellSlot slot, SpellInfo& out) -> bool;
	auto GetSpellByMissile(const char* projectileName, SpellInfo& out) -> bool;
	auto GetDash(const char* spellName, DashInfo& out) -> bool;
	auto GetSmiteSpell(const char* championName, SmiteInfo& out) -> bool;
	auto LoadSpells() -> void;
};