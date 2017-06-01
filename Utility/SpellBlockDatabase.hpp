#pragma once
#include <PluginSDK.h>

struct SpellBlockInfo
{
public:
	explicit SpellBlockInfo() :
		m_strChampionName(""),
		m_slot(0),
		m_flWidth(0.0f),
		m_bDummy(true)
	{ }

	explicit SpellBlockInfo(const char* champName, int slot, float width = 0.0f) :
		m_strChampionName(champName),
		m_slot(slot),
		m_flWidth(width),
		m_bDummy(false)
	{ }

	const char* m_strChampionName;
	int m_slot;
	float m_flWidth;
	bool m_bDummy;
};

struct AoESpellBlockInfo
{
public:
	explicit AoESpellBlockInfo() :
		m_slot(0),
		m_flRadius(0.0f),
		m_flRange(0.0f),
		m_bSelfCast(false),
		m_bDummy(true)
	{ }

	explicit AoESpellBlockInfo(int slot, float radius, float range, bool selfcast = false) :
		m_slot(slot),
		m_flRadius(radius),
		m_flRange(range),
		m_bSelfCast(selfcast),
		m_bDummy(false)
	{ }

	int m_slot;
	float m_flRange;
	float m_flRadius;
	bool m_bSelfCast;
	bool m_bDummy;
};

class SpellBlockDatabase
{
public:
	explicit SpellBlockDatabase();
	~SpellBlockDatabase();

public:
	auto IsBlockableSpell(const char* spellName) -> bool;
	auto GetBlockableSpell(const char* spellName) -> SpellBlockInfo;
public:
	auto IsBlockableAoESpell(const char* spellName) -> bool;
	auto GetBlockableAoESpell(const char* spellName) -> AoESpellBlockInfo;

private:
	std::unordered_map<std::string, SpellBlockInfo> m_mapSpellDatabase;
	std::unordered_map<std::string, AoESpellBlockInfo> m_mapAoESpells;
};