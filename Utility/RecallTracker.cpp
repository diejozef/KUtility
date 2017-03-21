#include "RecallTracker.hpp"

RecallTrackerBar::RecallTrackerBar()
{
}

RecallTrackerBar::RecallTrackerBar( const Vec2& pos, int width, int height ) :
	m_vecPos( pos ),
	m_iWidth( width ),
	m_iHeight( height )
{

}

RecallTrackerBar::~RecallTrackerBar()
{

}

auto RecallTrackerBar::Draw( void ) -> void
{
	GRender->DrawFilledBox( m_vecPos, Vec2( m_iWidth, m_iHeight ), Color::Black().Get( 170 ) );
}

auto RecallTrackerBar::SetPos( const Vec2& pos ) -> void
{
	m_vecPos = pos;
}

auto RecallTrackerBar::SetWidth( int width ) -> void
{
	m_iWidth = width;
}

auto RecallTrackerBar::SetHeight( int height ) -> void
{
	m_iHeight = height;
}

RecallTracker::RecallTracker( IUnit* hero ) :
	m_pUnit( hero ),
	m_bRecalling( false ),
	m_bAborted( false ),
	m_bFinished( false ),
	m_flEndTime( 0.0f ),
	m_flDuration( 0.0f )
{

}

RecallTracker::~RecallTracker()
{

}

auto RecallTracker::Draw( float time, bool drawHp, int index, const RecallTrackerBar& bar ) const -> void
{
	auto remainingTime = m_flEndTime - time;
	auto progressPct = remainingTime < 0.1f ? 0.01f : remainingTime / m_flDuration;

	auto markerPos = Vec2( bar.Pos().x + 2 + ( bar.Width() - 4 ) * progressPct, bar.Pos().y + 1 );
	auto markerEndPos = Vec2( markerPos.x, markerPos.y - ( 18 + index * 15 ) );

	GRender->DrawFilledBox( Vec2( bar.Pos().x + 2, bar.Pos().y + 2 ),
							Vec2( ( bar.Width() - 4 ) * progressPct, bar.Height() - 4 ),
							Color::LightBlue().Get() );

	GRender->DrawLine( markerPos, markerEndPos, Color::White().Get() );

	if ( drawHp )
		KDrawing::DrawString( markerEndPos, Color::White(), true, " %s(%.0f%%)", m_pUnit->ChampionName(), m_pUnit->HealthPercent() );
	else
		KDrawing::DrawString( markerEndPos, Color::White(), true, " %s", m_pUnit->ChampionName() );
}

auto RecallTracker::OnTeleport( float time, bool printInChat, OnTeleportArgs* data ) -> void
{
	switch ( data->Status )
	{
	case Teleport_Start:
		m_bRecalling = true;
		m_flDuration = data->Duration / 1000.0f;
		m_flEndTime = time + m_flDuration;
		break;

	case Teleport_Abort:
		m_bRecalling = false;

		GRender->NotificationEx( Color::LightRed().Get(), 4, false, true, "RECALL ABORTED: %s", m_pUnit->ChampionName() );

		break;

	case Teleport_Finish:
		m_bRecalling = false;

		GRender->NotificationEx( Color::LightGreen().Get(), 2, false, true, "RECALL FINISHED: %s", m_pUnit->ChampionName() );

		if ( printInChat )
		{
			auto time = GGame->Time();
			auto minutes = static_cast< int >( std::floorf( time / 60 ) );
			auto seconds = static_cast< int >( time - minutes * 60 );

			std::stringstream ss;
			ss	<< "<font color=\"#dddddd\">["
				<< std::setfill( '0' ) << std::setw( 2 ) << minutes << ":"
				<< std::setfill( '0' ) << std::setw( 2 ) << seconds << "] KUtility:</font> "
				<< "<font color=\"#ffffff\">Recall</font> <b><font color=\"#4bff4b\">FINISHED</font></b><font color=\"#ffffff\">:</font> " 
				<< "<font color=\"#ffff33\">" << m_pUnit->ChampionName() << "</font>";

			GGame->PrintChat( ss.str().c_str() );
		}

		break;
		
	default:
		break;
	}
}
