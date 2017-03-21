#include "SpellInfoBox.hpp"

SpellInfoBox::SpellInfoBox( IMenu* parentMenu, std::vector<FowTracker>* trackers, InputManager* inputManager ) :
	m_pTrackers( trackers ),
	m_pInputManager( inputManager ),
	m_iWidth( 325 ),
	m_iLineHeight( 18 )
{
	m_pMenu = parentMenu->AddMenu( "Info Box" );
	m_pDraw = m_pMenu->CheckBox( "Enable", false );

	m_vecScreenSize = GRender->ScreenSize();

	m_pPosX = m_pMenu->AddInteger( "X", 0, m_vecScreenSize.x, 0 );
	m_pPosY = m_pMenu->AddInteger( "Y", 0, m_vecScreenSize.y, 0 );

	auto enemies = GEntityList->GetAllHeros( false, true );
	m_iHeight = m_iLineHeight * enemies.size();

	for ( auto hero : enemies )
	{
		if ( hero == nullptr )
			continue;

		char fileName[ 64 ]{ '\0' };
		auto summonerSpellName = std::string( "" );

		for ( auto i = static_cast< int >( kSummonerSlot1 ); i <= static_cast< int >( kSummonerSlot2 ); i++ )
		{
			summonerSpellName = std::string( hero->GetSpellName( i ) );
			sprintf_s( fileName, "%s.png", summonerSpellName.c_str() );

			if ( m_mapTextures.find( summonerSpellName ) == m_mapTextures.end() )
			{
				auto texture = GRender->CreateTextureFromFile( fileName );

				if ( texture->GetSize().y == 0.0f )
				{
					char text[ 64 ];
					sprintf_s( text, "[KUtility] Missing texture: %s", fileName );
					GGame->PrintChat( text );
				}
				else
				{
					texture->Resize( 14, 15 );
					m_mapTextures[ summonerSpellName ] = texture;
				}
			}
		}
	}

	// non-default smite textures
	std::array<std::pair<std::string, std::string>, 2> smiteArray{
		std::make_pair( "S5_SummonerSmiteDuel", "S5_SummonerSmiteDuel.png" ),
		std::make_pair( "S5_SummonerSmitePlayerGanker", "S5_SummonerSmitePlayerGanker.png" )
	};

	for ( auto smite : smiteArray )
	{
		if ( m_mapTextures.find( std::get<0>( smite ) ) != m_mapTextures.end() )
			continue;

		auto texture = GRender->CreateTextureFromFile( std::get<1>( smite ).c_str() );

		if ( texture->GetSize().y == 0.0f )
		{
			char text[ 64 ];
			sprintf_s( text, "[KUtility] Missing texture: %s", std::get<1>( smite ).c_str() );
			GGame->PrintChat( text );
		}
		else
		{
			texture->Resize( 14, 15 );
			m_mapTextures[ std::get<0>( smite ) ] = texture;
		}
	}
}

SpellInfoBox::~SpellInfoBox()
{
	m_pMenu->Remove();
}

auto SpellInfoBox::OnUpdate() -> void
{
	if ( !m_pDraw->Enabled() )
		return;

	HandleDrag();
}

auto SpellInfoBox::OnRender() -> void
{
	if ( !m_pDraw->Enabled() )
		return;

	int xPos = m_pPosX->GetInteger();
	int yPos = m_pPosY->GetInteger();
	static int lineTextOffset = 1;

	int line = 0;

	for ( auto hero : GEntityList->GetAllHeros( false, true ) )
	{
		if ( hero == nullptr )
			continue;

		auto fowTracker = std::find_if( m_pTrackers->begin(), m_pTrackers->end(), [&]( const FowTracker& champ ) -> bool
		{
			if ( champ.Unit() == hero )
				return true;

			return false;
		} );

		// Background
		auto clr = Color( 0, 0, 0 );

		clr = hero->HealthPercent() > 50.0f ? Color::LightGreen() : hero->HealthPercent() > 20.0f ? Color::Orange() : Color::LightRed();

		GRender->DrawFilledBox( Vec2( xPos, ( yPos + line * m_iLineHeight ) - 2 ), 
								Vec2( m_iWidth, m_iLineHeight ), 
								Color::Black().Get( 130 ) );

		GRender->DrawFilledBox( Vec2( xPos + 1, ( yPos + 1 + line * m_iLineHeight ) - 2 ), 
								Vec2( ( ( m_iWidth - 2 ) * ( hero->HealthPercent() * 0.01f ) ), m_iLineHeight - 2 ), 
								clr.Get( 50 ) );

		// 1st column
		clr = hero->IsDead() ? Color::DarkGrey() : !hero->IsVisible() ? Color::Yellow() : Color::White();

		char text[ 64 ]{ '\0' };

		if ( !fowTracker->InFow() || hero->IsVisible() )
			KDrawing::DrawString(	Vec2( xPos + 5, ( yPos + line * m_iLineHeight - lineTextOffset ) ), 
									clr, true, "%s", hero->ChampionName() );
		else
			KDrawing::DrawString(	Vec2( xPos + 5, ( yPos + line * m_iLineHeight - lineTextOffset ) ), 
									clr, true, "%s(%.0f)", hero->ChampionName(), ( GGame->Time() - fowTracker->Time() ) );

		// 2nd column
		strcpy_s( text, "[R RDY]" );

		auto ready = hero->GetSpellRemainingCooldown( kSlotR ) < 1.f;

		if ( !ready )
			sprintf_s( text, "[R %.0f]", hero->GetSpellRemainingCooldown( kSlotR ) );

		clr = ready ? Color::LightGreen() : Color::Yellow();

		if ( hero->GetSpellLevel( kSlotR ) < 1 )
		{
			strcpy_s( text, "[R]" );
			clr = Color::LightRed();
		}

		KDrawing::DrawString( Vec2( xPos + 110, yPos + line * m_iLineHeight - lineTextOffset ), clr, true, text );

		// 3rd column
		strcpy_s( text, "[RDY]" );

		auto flashSlot = hero->GetSpellSlot( "SummonerFlash" );

		if ( flashSlot != kSlotUnknown )
		{
			ready = hero->GetSpellRemainingCooldown( flashSlot ) < 1.f;

			auto texture = m_mapTextures[ "SummonerFlash" ];

			if ( texture != nullptr )
				texture->Draw( xPos + 180, yPos + line * m_iLineHeight );

			if ( !ready )
				sprintf_s( text, "[%.0f]", hero->GetSpellRemainingCooldown( flashSlot ) );
			else
				strcpy_s( text, "[RDY]" );

			clr = ready ? Color::LightGreen() : Color::Yellow();

			KDrawing::DrawString( Vec2( xPos + 197, yPos + line * m_iLineHeight - lineTextOffset ), clr, true, text );
		}
		else
		{
			std::string name( hero->GetSpellName( kSummonerSlot1 ) );
			auto texture = m_mapTextures[ name ];

			if ( texture != nullptr )
				texture->Draw( xPos + 180, yPos + line * m_iLineHeight );

			ready = hero->GetSpellRemainingCooldown( kSummonerSlot1 ) < 1.f;

			if ( !ready )
				sprintf_s( text, "[%.0f]", hero->GetSpellRemainingCooldown( kSummonerSlot1 ) );
			else
				strcpy_s( text, "[RDY]" );

			clr = ready ? Color::LightGreen() : Color::Yellow();

			KDrawing::DrawString( Vec2( xPos + 197, yPos + line * m_iLineHeight - lineTextOffset ), clr, true, text );
		}

		// 4th column
		strcpy_s( text, "[S2]" );

		auto secondSummonerSlot = flashSlot == kSummonerSlot1 ? kSummonerSlot2 : kSummonerSlot1;

		if ( flashSlot == kSlotUnknown )
			secondSummonerSlot = kSummonerSlot2;

		std::string name( hero->GetSpellName( secondSummonerSlot ) );
		auto texture = m_mapTextures[ name ];

		if ( texture != nullptr )
			texture->Draw( xPos + 260, yPos + line * m_iLineHeight );

		ready = hero->GetSpellRemainingCooldown( secondSummonerSlot ) < 1.f;

		if ( !ready )
			sprintf_s( text, "[%.0f]", hero->GetSpellRemainingCooldown( secondSummonerSlot ) );
		else
			strcpy_s( text, "[RDY]" );

		clr = ready ? Color::LightGreen() : Color::Yellow();

		KDrawing::DrawString( Vec2( xPos + 277, yPos + line * m_iLineHeight - lineTextOffset ), clr, true, text );

		line++;
	}
}

auto SpellInfoBox::HandleDrag() -> void
{
	if ( !m_pDraw->Enabled() )
		return;

	static bool isDragging = false;

	POINT cursor;
	GetCursorPos( &cursor );

	auto x = m_pPosX->GetInteger();
	auto y = m_pPosY->GetInteger();

	if ( isDragging 
		|| cursor.x > x && cursor.x < x + m_iWidth
		&& cursor.y > y && cursor.y < y + m_iHeight )
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

	if ( x > m_vecScreenSize.x - m_iWidth )
		m_pPosX->UpdateInteger( m_vecScreenSize.x - m_iWidth );

	if ( y > m_vecScreenSize.y - m_iHeight )
		m_pPosY->UpdateInteger( m_vecScreenSize.y - m_iHeight );
}
