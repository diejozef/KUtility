#include "LastPositions.hpp"

LastPositions::LastPositions( IMenu* parentMenu, std::vector<FowTracker>* trackers ) :
	m_pFowTrackers( trackers )
{
	m_pMenu = parentMenu->AddMenu( "Last Positions" );
	m_pEnable = m_pMenu->CheckBox( "Enable", false );
	m_pDrawTimer = m_pMenu->CheckBox( "Draw Timer", false );
	//m_pDrawIcons = m_pMenu->CheckBox( "[wip] Draw Icons", false );
}

LastPositions::~LastPositions()
{
	m_pMenu->Remove();
}

auto LastPositions::OnRenderEnemy( IUnit* hero ) -> void
{
	if ( !m_pEnable->Enabled() || hero->IsDead() || hero->IsVisible() )
		return;

	auto fowTracker = std::find_if( m_pFowTrackers->begin(), m_pFowTrackers->end(), [&]( const FowTracker& t )
	{
		return t.Unit() == hero;
	} );

	auto w2m = fowTracker->LastMinimapPos();
	auto champName = hero->ChampionName();

	char displayName[ 4 ]{ '\0' };
	strncpy_s( displayName, champName, 3 );

	if ( m_pDrawTimer->Enabled() )
		KDrawing::DrawString( w2m, Color::Yellow(), true, "%s(%.0f)", displayName, ( GGame->Time() - fowTracker->Time() ) );
	else
		KDrawing::DrawString( w2m, Color::Yellow(), true, "%s", displayName );
}
