#pragma once
#include "Wards.hpp"

class WardManager
{
public:
	explicit WardManager(IMenu* parentMenu, IUnit* player);
	~WardManager();

	auto OnRender() -> void;
	auto OnSpellCast(const CastedSpell& spell) -> void;
	auto OnCreateObject(IUnit* object) -> void;
	auto OnDestroyObject(IUnit* object) -> void;
	auto OnUnitDeath(IUnit* object) -> void;
	auto OnEnterFow(IUnit* object) -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pDrawWards;
	IMenuOption* m_pDrawRange;
	IUnit* m_pPlayer;

private:
	std::vector<Ward> m_vecWards;
	std::unordered_map<std::string, WardDefinition> m_mapWardObjectDefinitions;
	std::unordered_map<std::string, WardDefinition> m_mapWardSpellDefinitions;

private:
	auto HandleWardCorpse(IUnit* corpse) -> void;
	auto OnDestroy(IUnit* object) -> void;
};

