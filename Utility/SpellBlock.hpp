#pragma once
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <PluginSDK.h>
#include "SpellBlockDatabase.hpp"

enum eSpellBlockType
{
	kSpellBlockFiora,
	kSpellBlockSivir
};

class SpellBlock
{
public:
	explicit SpellBlock(IMenu* parentMenu, IUnit* player, eSpellBlockType blockType);
	~SpellBlock();

public:
	auto OnUpdate() -> void;
	auto OnSpellCast(const CastedSpell& spell) -> void;

private:
	IMenu* m_pMenu;
	IMenu* m_pBlockList;
	IMenuOption* m_pEnable;

private:
	IUnit* m_pPlayer;
	ISpell2* m_pSpell;
	SpellBlockDatabase* m_pBlockDatabase;

private:
	eSpellBlockType m_blockType;
	bool m_bRivenQ;
	bool m_bJaxE;

private:
	std::vector<std::tuple<const char*, const char*, float>> m_vecBuffDatabase;

private:
	auto IsSpecialSpell(const char* spellName) -> bool;
	auto BlockSpell(const Vec3& ripostePos) -> void;
};