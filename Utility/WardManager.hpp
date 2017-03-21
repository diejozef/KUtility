#pragma once
#include "Wards.hpp"

class WardManager
{
public:
	explicit WardManager( IMenu* parentMenu, IUnit* player );
	~WardManager();

	auto OnRender() -> void;
	auto OnUpdate() -> void;
	auto OnSpellCast( const CastedSpell& spell ) -> void;
	auto OnCreateObject( IUnit* object ) -> void;
	auto OnUnitDeath( IUnit* object ) -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pDrawWards;
	IMenuOption* m_pDrawRange;

	IUnit* m_pPlayer;

	std::vector<Ward> m_vecWards;
	std::vector<WardDefinition> m_vecWardDefinitions;

	int m_iNextEraseTick;
	bool m_bErasing;

	auto HandleWardCorpse( IUnit* corpse ) -> void;
};

