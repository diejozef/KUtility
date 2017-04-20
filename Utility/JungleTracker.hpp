#pragma once
#include <PluginSDK.h>
#include <KDrawing.hpp>

class JungleTracker
{
public:
	explicit JungleTracker();
	explicit JungleTracker(const Vec3& pos);
	~JungleTracker();

	auto OnRender() const -> void;
	auto OnUpdate() -> void;
	auto OnJungleNotify(float time) -> void;

	auto SetLastAggro(bool last) -> void;
	auto SetNextPingTime(float time) -> void;
	auto ResetAggroCount() -> void;
	auto IncreaseAggroCount() -> void;

	inline auto Pos() const -> Vec3 {
		return m_vecPos;
	}

	inline auto LastAggroTime() const -> float {
		return m_flLastAggroTime;
	}

	inline auto NextPingTime() const -> float {
		return m_flNextPingTime;
	}

	inline auto AggroCount() const -> int {
		return m_iAggroCount;
	}

private:
	bool m_bLastAggro;
	float m_flLastAggroTime;
	float m_flNextPingTime;
	int m_iAggroCount;
	Vec3 m_vecPos;
	Vec2 m_vecMinimapPos;
};