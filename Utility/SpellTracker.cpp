#include "SpellTracker.hpp"

SpellTracker::SpellTracker( IMenu* parentMenu, IUnit* player ) :
	m_pPlayer( player )
{
	m_pMenu = parentMenu->AddMenu( "Spell Tracker" );
	m_pEnable = m_pMenu->CheckBox( "Enable", false );
	m_pDrawTimer = m_pMenu->CheckBox( "Draw Timer", false );
	m_pDrawAllies = m_pMenu->CheckBox( "Draw Allies", false );
}

SpellTracker::~SpellTracker()
{
	m_pMenu->Remove();
}

auto SpellTracker::OnRenderHero( IUnit* hero ) -> void
{
	if ( !m_pEnable->Enabled() || hero->IsDead() || !hero->IsVisible() || hero == m_pPlayer )
		return;

	if ( !m_pDrawAllies->Enabled() && !hero->IsEnemy( m_pPlayer ) )
		return;

	auto hpBarPos = Vec2( 0.f, 0.f );

	if ( hero->GetHPBarPosition( hpBarPos ) )
	{
		hpBarPos.x += 10;
		hpBarPos.y += 31;

		auto champName = hero->ChampionName();

		if ( !strcmp( champName, "Jhin" ) || !strcmp( champName, "Annie" ) || !strcmp( champName, "Corki" ) )
			hpBarPos.y += 3;

		auto column = 0;

		for ( auto i = 0; i < 4; i++ )
		{
			auto cd = hero->GetSpellTotalCooldown( i );
			auto rcd = hero->GetSpellRemainingCooldown( i );
			auto percentage = cd > 0.0f && rcd > 0.0f ? 1.0f - rcd / cd : 1.0f;

			auto drawPos = hpBarPos;
			drawPos.x += column * 25;

			GRender->DrawFilledBox( Vec2( drawPos.x - 1, drawPos.y - 1 ), Vec2( 23, 6 ), Color::Black().Get() );

			if ( hero->GetSpellLevel( i ) > 0 && rcd > 1.f )
			{
				GRender->DrawFilledBox( drawPos, Vec2( 21 * percentage, 4 ), Color::LightBlue().Get() );

				if ( m_pDrawTimer->Enabled() )
				{
					auto offset = rcd < 10.0f ? 3 : rcd < 100.0f ? 9 : 11;
					KDrawing::DrawString( Vec2( drawPos.x + ( 11.5f - offset ), drawPos.y + 6 ), Color::White(), true, "%.0f", rcd );
				}
			}
			else if ( hero->GetSpellLevel( i ) < 1 )
			{
				GRender->DrawFilledBox( drawPos, Vec2( 21, 4 ), Color::LightRed().Get() );
			}
			else if ( rcd < 1.f )
			{
				GRender->DrawFilledBox( drawPos, Vec2( 21, 4 ), Color::LightGreen().Get() );
			}

			column++;
		}
	}
}
