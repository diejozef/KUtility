#include "RecallManager.hpp"

RecallManager::RecallManager( IMenu* parentMenu, IUnit* player, InputManager* inputManager, std::vector<FowTracker>* trackers ) :
	m_pPlayer( player ),
	m_pInputManager( inputManager ),
	m_pFowTrackers( trackers )
{
	m_pMenu = parentMenu->AddMenu( "Recall Tracker" );
	m_pEnable = m_pMenu->CheckBox( "Enable", false );
	m_pUseChat = m_pMenu->CheckBox( "Use Chat", false );
	m_pDrawHp = m_pMenu->CheckBox( "Draw Health %", false );

	m_vecScreenSize = GRender->ScreenSize();
	m_pPosX = m_pMenu->AddInteger( "X", 0, m_vecScreenSize.x, m_vecScreenSize.x / 2 );
	m_pPosY = m_pMenu->AddInteger( "Y", 0, m_vecScreenSize.y, m_vecScreenSize.y / 2 );

	m_bar = RecallTrackerBar( std::move( Vec2( m_pPosX->GetInteger(), m_pPosY->GetInteger() ) ), 400, 13 );

	auto enemies = GEntityList->GetAllHeros( false, true );
	std::for_each( enemies.begin(), enemies.end(), [&]( IUnit* hero )
	{
		if ( hero != nullptr )
			m_vecTrackers.emplace_back( RecallTracker( hero ) );
	} );
}

RecallManager::~RecallManager()
{
	m_pMenu->Remove();
}

auto RecallManager::OnRender() -> void
{
	if ( !m_pEnable->Enabled() )
		return;

	auto recallCount = std::count_if( m_vecTrackers.begin(), m_vecTrackers.end(), []( const RecallTracker& t )
	{
		return t.Recalling();
	} );

	if( recallCount > 0 )
	{
		m_bar.Draw();

		auto time = GGame->Time();

		if ( recallCount > 1 )
		{
			std::sort( m_vecTrackers.begin(), m_vecTrackers.end(), [&]( const RecallTracker& a, const RecallTracker& b )
			{
				return a.RemainingTime( time ) < b.RemainingTime( time );
			} );
		}

		auto i = 0;

		std::for_each( m_vecTrackers.begin(), m_vecTrackers.end(), [&]( const RecallTracker& t )
		{
			if ( t.Recalling() ) 
			{
				t.Draw( time, m_pDrawHp->Enabled(), i, m_bar );
				i++;
			}
		} );
	}
}

auto RecallManager::OnUpdate() -> void
{
	if ( !m_pEnable->Enabled() )
		return;

	HandleDrag();
}

auto RecallManager::OnTeleport( OnTeleportArgs* data ) -> void
{
	if ( data->Type != Teleport_Recall )
		return;

	auto caster = data->Source;
	if ( !caster->IsEnemy( m_pPlayer ) )
		return;

	if ( data->Status == Teleport_Start && caster->IsVisible() )
		return;

	auto& tracker = std::find_if( m_vecTrackers.begin(), m_vecTrackers.end(), [&]( const RecallTracker& t )
	{
		return t.Unit() == caster;
	} );

	if ( tracker != m_vecTrackers.end() )
		tracker->OnTeleport( GGame->Time(), m_pUseChat->Enabled(), data );

	if ( data->Status == Teleport_Finish )
	{
		auto& fowTracker = std::find_if( m_pFowTrackers->begin(), m_pFowTrackers->end(), [&]( const FowTracker& t )
		{
			return t.Unit() == caster;
		} );

		if ( fowTracker != m_pFowTrackers->end() )
			fowTracker->OnResetPos();
	}
}

auto RecallManager::HandleDrag() -> void
{
	if ( !m_pEnable->Enabled() )
		return;

	static bool isDragging = false;

	POINT cursor;
	GetCursorPos( &cursor );

	auto x = m_pPosX->GetInteger();
	auto y = m_pPosY->GetInteger();

	if ( isDragging
		|| cursor.x > x && cursor.x < x + m_bar.Width()
		&& cursor.y > y && cursor.y < y + m_bar.Height() )
	{
		static std::array<int, 2> delta = { -1, -1 };

		if ( delta[ 0 ] == -1 && delta[ 1 ] == -1 )
		{
			delta[ 0 ] = cursor.x - x;
			delta[ 1 ] = cursor.y - y;
		}

		if ( m_pInputManager->IsMouseButtonPressed( kLButton ) )
		{
			isDragging = true;

			auto newX = cursor.x - delta[ 0 ];
			auto newY = cursor.y - delta[ 1 ];

			m_pPosX->UpdateInteger( newX );
			m_pPosY->UpdateInteger( newY );

			x = newX;
			y = newY;
		}
		else
		{
			isDragging = false;

			delta[ 0 ] = -1;
			delta[ 1 ] = -1;
		}
	}

	if ( x < 0 )
		m_pPosX->UpdateInteger( 0 );

	if ( y < 0 )
		m_pPosY->UpdateInteger( 0 );

	if ( x > m_vecScreenSize.x - m_bar.Width() )
		m_pPosX->UpdateInteger( m_vecScreenSize.x - m_bar.Width() );

	if ( y > m_vecScreenSize.y - m_bar.Height() )
		m_pPosY->UpdateInteger( m_vecScreenSize.y - m_bar.Height() );

	m_bar.SetPos( std::move( Vec2( m_pPosX->GetInteger(), m_pPosY->GetInteger() ) ) );
}
