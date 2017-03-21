#pragma once
#include <PluginSDK.h>

class FowTracker
{
public:
	explicit FowTracker();
	explicit FowTracker( IUnit* unit );
	~FowTracker();

	auto OnEnterFow( float time ) -> void;
	auto OnResetPos( void ) -> void;
	auto OnExitFow( void ) -> void;

public:
	auto SetLastPos( const Vec3& pos ) -> void;

public:
	inline auto InFow( void ) const -> bool {
		return m_bInFow;
	}

	inline auto Time( void ) const -> float {
		return m_flTime;
	}

	inline auto Unit( void ) const -> IUnit* {
		return m_pUnit;
	}

	inline auto LastPos( void ) const -> Vec3 {
		return m_vecLastPos;
	}

	inline auto LastMinimapPos( void ) const -> Vec2 {
		return m_vecLastMinimapPos;
	}

private:
	IUnit* m_pUnit;
	Vec3 m_vecLastPos;
	Vec2 m_vecLastMinimapPos;
	bool m_bInFow;
	float m_flTime;
};