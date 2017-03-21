#pragma once
#include "Spells.hpp"

class SpellManager
{
public:
	explicit SpellManager( IMenu* parentMenu, IUnit* player );
	~SpellManager();

	auto OnRender() -> void;
	auto OnUpdate() -> void;
	auto OnSpellCast( const CastedSpell& spell ) -> void;
	auto OnCreateObject( IUnit* object ) -> void;
	auto OnDestroyObject( IUnit* object ) -> void;

private:
	std::vector<SkillshotIndicator> m_vecSkillshots;
	std::vector<DashIndicator> m_vecDashes;

	IMenu* m_pMenu;
	IMenuOption* m_pDrawSkillshots;
	IMenuOption* m_pDrawDashes;

	IUnit* m_pPlayer;

	int m_iNextEraseTick;
	bool m_bErasing;
};