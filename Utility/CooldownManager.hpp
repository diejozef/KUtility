#pragma once
#include <PluginSDK.h>

class CooldownManager
{
public:
	explicit CooldownManager(IUnit* player);
	~CooldownManager();

public:
	auto OnSpellCast(const CastedSpell& spell) -> void;
	auto OnTeleport(OnTeleportArgs* data) -> void;
	auto OnCreateObject(IUnit* object) -> void;
	auto OnDestroyObject(IUnit* object) -> void;

public:
	auto GetRemainingCooldown(int networkId, int slot) -> float;

private:
	IUnit* m_pPlayer;

private:
	std::unordered_map<int, std::array<float, 6>> m_mapCooldownEndTimes;
};