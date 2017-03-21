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
	explicit Ward( const Vec3& pos, const WardDefinition& info, float duration );
	~Ward();

	auto Valid( void ) const -> bool;
	auto Invalidate( void ) -> void;
	auto Draw( bool drawRange ) -> void;

	inline auto Pos( void ) const -> Vec3 {
		return m_vecPos;
	}

	inline auto Info( void ) const -> WardDefinition {
		return m_info;
	}

	inline auto EndTime( void ) const -> float {
		return m_flEndTime;
	}

private:
	Vec3 m_vecPos;
	WardDefinition m_info;
	float m_flEndTime;
	bool m_bValid;
};