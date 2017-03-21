#include "FowTracker.hpp"

FowTracker::FowTracker()
{

}

FowTracker::FowTracker( IUnit* unit ) :
	m_pUnit( unit )
{
	m_flTime = GGame->Time();
	m_vecLastPos = m_pUnit->GetPosition();
	GGame->WorldToMinimap( m_vecLastPos, m_vecLastMinimapPos );
}

FowTracker::~FowTracker()
{

}

auto FowTracker::OnEnterFow( float time ) -> void
{
	m_flTime = time;
	m_bInFow = true;

	if ( m_pUnit->IsDead() )
	{
		OnResetPos();
	}
	else
	{
		m_vecLastPos = m_pUnit->GetPosition();
		GGame->WorldToMinimap( m_vecLastPos, m_vecLastMinimapPos );
	}
}

auto FowTracker::OnResetPos( void ) -> void
{
	if ( m_pUnit->GetTeam() == kTeamChaos )
		m_vecLastPos = std::move( Vec3( 14300.0f, 170.0f, 14300.0f ) );
	else
		m_vecLastPos = std::move( Vec3( 420.0f, 160.0f, 420.0f ) ); // 420xy riot blz it

	GGame->WorldToMinimap( m_vecLastPos, m_vecLastMinimapPos );
}

auto FowTracker::OnExitFow() -> void
{
	m_bInFow = false;
}

auto FowTracker::SetLastPos( const Vec3& pos ) -> void
{
	m_vecLastPos = pos;
	GGame->WorldToMinimap( m_vecLastPos, m_vecLastMinimapPos );
}
