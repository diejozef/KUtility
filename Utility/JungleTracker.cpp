#include "JungleTracker.hpp"

JungleTracker::JungleTracker() :
	m_flLastAggroTime( 0.0f ),
	m_flNextPingTime( 0.0f ),
	m_iAggroCount( 0 ),
	m_bLastAggro( false )
{

}

JungleTracker::JungleTracker( const Vec3& pos ) :
	m_vecPos( pos ),
	m_flLastAggroTime( 0.0f ),
	m_flNextPingTime( 0.0f ),
	m_iAggroCount( 0 ),
	m_bLastAggro( false )
{
	GGame->WorldToMinimap( m_vecPos, m_vecMinimapPos );
}

JungleTracker::~JungleTracker()
{

}

auto JungleTracker::SetLastAggro( bool last ) -> void
{
	m_bLastAggro = last;
}

auto JungleTracker::SetNextPingTime( float time ) -> void
{
	m_flNextPingTime = time;
}

auto JungleTracker::ResetAggroCount() -> void
{
	m_iAggroCount = 0;
}

auto JungleTracker::IncreaseAggroCount() -> void
{
	m_iAggroCount += 1;
}

auto JungleTracker::OnRender() -> void
{
	auto timeSinceLastAggro = GGame->Time() - m_flLastAggroTime;

	if ( timeSinceLastAggro <= 60.0f )
	{
		auto w2m = m_vecMinimapPos;

		w2m.x -= timeSinceLastAggro < 10.0f ? 5.5f : 7.5f;
		w2m.y -= 7.5f;

		KDrawing::DrawString( w2m, m_bLastAggro ? Color::Orange() : Color::White(), true, "%.0f", timeSinceLastAggro );
	}
}

auto JungleTracker::OnUpdate() -> void
{
	auto timeSinceLastAggro = GGame->Time() - m_flLastAggroTime;

	if ( timeSinceLastAggro > 15.0f )
		m_iAggroCount = 0;
}

auto JungleTracker::OnJungleNotify( float time ) -> void
{
	m_flLastAggroTime = time;
	m_bLastAggro = true;
}