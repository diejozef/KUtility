#pragma once
#include "Spells.hpp"

class SpellManager
{
public:
	explicit SpellManager(IMenu* parentMenu, IUnit* player);
	~SpellManager();

	auto OnRender() -> void;
	auto OnUpdate() -> void;
	auto OnSpellCast(const CastedSpell& spell) -> void;
	auto OnCreateObject(IUnit* object) -> void;
	auto OnDestroyObject(IUnit* object) -> void;

private:
	std::vector<SkillshotIndicator> m_vecSkillshots;
	std::vector<DashIndicator> m_vecDashes;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pDrawSkillshots;
	IMenuOption* m_pDrawDashes;

private:
	IUnit* m_pPlayer;

private:
	auto IsEnabled(void) const -> bool;
};