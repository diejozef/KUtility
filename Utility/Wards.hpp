#pragma once
#include "PluginSDK.h"
#include <KDrawing.hpp>

enum eWardType
{
	kPinkWard,
	kBlueWard,
	kSightWard,
	kTrap,
	kCaitlynTrap
};

struct WardDefinition
{
public:
	explicit WardDefinition() { }
	explicit WardDefinition(const char* objectName, const char* spellName, float duration, eWardType type, float range) :
		m_strObjectName(objectName),
		m_strSpellName(spellName),
		m_flDuration(duration),
		m_type(type),
		m_flRange(range) { }

	const char* m_strObjectName;
	const char* m_strSpellName;
	float m_flDuration;
	eWardType m_type;
	float m_flRange;
};

class Ward
{
public:
	explicit Ward();
	explicit Ward(const Vec3& pos, const WardDefinition& info, float duration);
	explicit Ward(const Vec3& pos, const WardDefinition& info, float duration, int networkId);
	~Ward();

public:
	auto Valid(void) const -> bool;
	auto Invalidate(void) -> void;
	auto Draw(bool drawRange) const -> void;
	auto SetNetworkId(int networkId) -> void;
	auto SetEndTime(float time) -> void;

public:
	inline auto Pos(void) const -> Vec3 {
		return m_vecPos;
	}

	inline auto Info(void) const -> WardDefinition {
		return m_info;
	}

	inline auto EndTime(void) const -> float {
		return m_flEndTime;
	}

	inline auto NetworkId(void) const -> int {
		return m_iNetworkId;
	}

private:
	Vec3 m_vecPos;
	WardDefinition m_info;
	Color m_color;
	float m_flEndTime;
	bool m_bValid;
	bool m_bDrawTimer;
	int m_iNetworkId;
};