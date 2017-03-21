#include "JungleManager.hpp"

JungleManager::JungleManager( IMenu* parentMenu )
{
	m_pMenu = parentMenu->AddMenu( "Jungle Tracker" );
	m_pFowTracker = m_pMenu->CheckBox( "FoW Tracker", false );
	m_pUsePings = m_pMenu->CheckBox( "Use Pings", false );

	m_flNextResetCheckTime = GGame->Time() + 5.0f;

	m_vecJungleTrackers = {
		JungleTracker( Vec3( 7878.0f, 52.0f, 9500.0f ) ), // redside raptors
		JungleTracker( Vec3( 7120.0f, 52.0f, 10918.0f ) ), // redside redbuff
		JungleTracker( Vec3( 6436.0f, 52.0f, 12224.0f ) ), // redside krugs
		JungleTracker( Vec3( 11017.0f, 52.0f, 7011.0f ) ), // redside bluebuff
		JungleTracker( Vec3( 12612.0f, 52.0f, 6400.0f ) ), // redside gromp
		JungleTracker( Vec3( 11007.0f, 52.0f, 8400.0f ) ), // redside wolfs

		JungleTracker( Vec3( 6978.0f, 52.0f, 5416.0f ) ), // blueside raptors
		JungleTracker( Vec3( 7767.0f, 52.0f, 4005.0f ) ), // blueside redbuff
		JungleTracker( Vec3( 8390.0f, 52.0f, 2692.0f ) ), // blueside krugs
		JungleTracker( Vec3( 3810.0f, 52.0f, 7905.0f ) ), // blueside bluebuff
		JungleTracker( Vec3( 3829.0f, 52.0f, 6503.0f ) ), // blueside wolfs
		JungleTracker( Vec3( 2171.0f, 52.0f, 8455.0f ) ), // blueside gromp

		JungleTracker( Vec3( 4155.0f, 52.0f, 9451.0f ) ), // top river center
		JungleTracker( Vec3( 10711.0f, 52.0f, 5400.0f ) ), // bot river center

		JungleTracker( Vec3( 9818.0f, 52.0f, 4434.0f ) ), // dragon
		JungleTracker( Vec3( 4950.0f, 52.0f, 10410.0f ) ) // baron
	};
}

JungleManager::~JungleManager()
{
	m_pMenu->Remove();
}

auto JungleManager::OnRender() -> void
{
	if ( !m_pFowTracker->Enabled() || GGame->Time() < 90.0f )
		return;

	for ( auto tracker : m_vecJungleTrackers )
		tracker.OnRender();
}

auto JungleManager::OnUpdate() -> void
{
	if ( GGame->Time() < m_flNextResetCheckTime )
		return;

	for ( auto& tracker : m_vecJungleTrackers )
		tracker.OnUpdate();

	m_flNextResetCheckTime = GGame->Time() + 5.0f;
}

auto JungleManager::OnJungleNotify( JungleNotifyData* data ) -> void
{
	if ( GGame->Time() < 90.0f )
		return;

	auto pos = data->Position;

	if ( !GGame->WithinFogOfWar( pos ) )
		return;

	auto& i = std::find_if( m_vecJungleTrackers.begin(), m_vecJungleTrackers.end(), [&]( const JungleTracker& tracker )
	{
		auto distance = ( tracker.Pos() - pos ).Length2D();

		if ( distance < 700.0f )
			return true;

		return false;
	} );

	if ( i != m_vecJungleTrackers.end() )
	{
		auto time = GGame->Time();
		auto timeSinceLastAggro = time - i->LastAggroTime();

		if ( timeSinceLastAggro < 5.0f )
		{
			i->IncreaseAggroCount();

			if ( time > i->NextPingTime() && i->AggroCount() >= 1 )
			{
				GGame->ShowPing( kPingNormal, i->Pos(), true );

				i->SetNextPingTime( time + 20.0f );
				i->ResetAggroCount();
			}
		}

		for ( auto& tracker : m_vecJungleTrackers )
			tracker.SetLastAggro( false );

		i->OnJungleNotify( GGame->Time() );
	}
}
