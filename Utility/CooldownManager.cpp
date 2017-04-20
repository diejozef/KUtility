#include "CooldownManager.hpp"

CooldownManager::CooldownManager(IUnit* player) :
	m_pPlayer(player)
{
	auto enemies = GEntityList->GetAllHeros(false, true);
	std::for_each(enemies.begin(), enemies.end(), [&](IUnit* hero)
	{
		for (auto i = static_cast<int>(kSlotQ); i <= static_cast<int>(kSlotSummoner2); i++)
			m_mapCooldownEndTimes[hero->GetNetworkId()][i] = 0.0f;
	});
}

CooldownManager::~CooldownManager()
{
}

auto CooldownManager::OnSpellCast(const CastedSpell& spell) -> void
{
	if (spell.AutoAttack_)
		return;

	auto caster = spell.Caster_;
	if (caster == nullptr || !caster->IsHero() || !caster->IsEnemy(m_pPlayer))
		return;
}

auto CooldownManager::OnTeleport(OnTeleportArgs* data) -> void
{
	auto caster = data->Source;
	auto type = data->Type;
	auto status = data->Status;

	if (caster == nullptr || !caster->IsEnemy(m_pPlayer) || 
		m_mapCooldownEndTimes.find(caster->GetNetworkId()) == m_mapCooldownEndTimes.end())
		return;

	if (type == Teleport_Teleport && status != Teleport_Start)
	{
		// Todo: include summoner cdr mastery here
		auto scdr = 0.0f;
		if (caster->HasItemId(3158))
			scdr = 0.1f;

		auto tpSlot = caster->GetSpellSlot("SummonerTeleport");
		auto tpCd = 300.0f * (1.0f - scdr);
		auto baseCd = status == Teleport_Finish ? tpCd : tpCd * 0.666f;

		m_mapCooldownEndTimes[caster->GetNetworkId()][tpSlot] = GGame->Time() + baseCd;
	}
}

auto CooldownManager::OnCreateObject(IUnit* object) -> void
{
}

auto CooldownManager::OnDestroyObject(IUnit* object) -> void
{
}

auto CooldownManager::GetRemainingCooldown(int networkId, int slot) -> float
{
	if (slot < kSlotQ || slot > kSlotSummoner2 || m_mapCooldownEndTimes.find(networkId) == m_mapCooldownEndTimes.end())
		return 0.0f;

	auto rcd = m_mapCooldownEndTimes[networkId][slot] - GGame->Time();
	if (rcd > 0.0f)
		return rcd;

	return 0.0f;
}
