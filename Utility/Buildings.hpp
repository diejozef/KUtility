#pragma once
#include <PluginSDK.h>
#include <KDrawing.hpp>

struct inhibitor_t
{
public:
	explicit inhibitor_t(IUnit* object, float hp) :
		m_pSelf(object),
		m_bDestroyed(false),
		m_flRespawnTime(0.0f),
		m_flLastHp(hp)
	{ }

	~inhibitor_t() { }

	IUnit* m_pSelf;
	bool m_bDestroyed;
	float m_flRespawnTime;
	float m_flLastHp;
};

class Buildings
{
public:
	explicit Buildings(IMenu* parentMenu);
	~Buildings();

public:
	auto OnUpdate() -> void;
	auto OnRender() -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pEnable;
	IMenuOption* m_pDrawTurrets;
	IMenuOption* m_pDrawInhibitors;
	IMenuOption* m_pOnlyEnemyTurrets;
	float m_flNextCheckTime;

private:
	std::vector<inhibitor_t> m_vecInhibitors;
};