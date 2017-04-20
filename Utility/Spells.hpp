#pragma once
#include "PluginSDK.h"
#include "SpellDatabase.hpp"

class SkillshotIndicator
{
public:
	explicit SkillshotIndicator();
	explicit SkillshotIndicator(const Vec2& pos, const Vec2& end, float duration, const SpellInfo& info);
	explicit SkillshotIndicator(const Vec2& pos, const Vec2& end, float duration, const SpellInfo& info, int missileNetworkId);
	~SkillshotIndicator();

	auto Rectangle(void) const->std::array<Vec2, 4>;
	auto Draw(void) const -> void;
	auto Valid(void) const -> bool;
	auto Invalidate(void) -> void;
	auto SetStartPos(const Vec2& pos) -> void;
	auto SetEndPos(const Vec2& pos) -> void;
	auto SetMissileNetworkId(int id) -> void;

	inline auto StartPos(void) const -> Vec2 {
		return m_vecStart;
	}

	inline auto EndPos(void) const -> Vec2 {
		return m_vecEnd;
	}

	inline auto EndTime(void) const -> float {
		return m_flEndTime;
	}

	inline auto Type(void) const -> eSpellInfoType {
		return m_info.m_type;
	}

	inline auto Info(void) const -> SpellInfo {
		return m_info;
	}

	inline auto MissileNetworkId(void) const -> int {
		return m_iMissileNetworkId;
	}

private:
	Vec2 m_vecStart;
	Vec2 m_vecEnd;
	float m_flEndTime;
	float m_flHeight;
	SpellInfo m_info;
	int m_iMissileNetworkId;
	bool m_bValid;
	std::array<Vec2, 4> m_rectangle;
};

class DashIndicator
{
public:
	DashIndicator();
	DashIndicator(const Vec3& posFrom, const Vec3& posTo, float duration);
	~DashIndicator();

	auto Draw(void) const -> void;
	auto Valid(void) const -> bool;

	inline auto StartPos(void) const -> Vec3 {
		return m_vecStart;
	}

	inline auto EndPos(void) const -> Vec3 {
		return m_vecEnd;
	}

	inline auto EndTime(void) const -> float {
		return m_flEndTime;
	}

private:
	Vec3 m_vecStart;
	Vec3 m_vecEnd;
	float m_flEndTime;
};