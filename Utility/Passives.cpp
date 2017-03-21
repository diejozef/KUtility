#include "Passives.hpp"

Passives::Passives( IMenu* parentMenu )
{
	m_pMenu = parentMenu->AddMenu( "Passives" );
	m_pEnable = m_pMenu->CheckBox( "Enable", false );
	m_pDrawPassives = m_pMenu->CheckBox( "Draw Passives", false );
	m_pDrawRebirthPassives = m_pMenu->CheckBox( "Draw Rebirth Passives", false );
}

Passives::~Passives()
{
	m_pMenu->Remove();
}

auto Passives::OnRenderEnemy( IUnit* hero ) -> void
{
	if ( !m_pEnable->Enabled() || hero->IsDead() || !hero->IsVisible() )
		return;

	static std::vector<std::pair<const char*, const char*>> passives{
		std::make_pair( "chronoshift", "[Cs: %.1f]" ),
		std::make_pair( "zhonyasringshield", "[Zh: %.1f]" ),
		std::make_pair( "undyingrage", "[R: %.1f]" )
	};

	static std::vector<std::tuple<const char*, const char*, const char*>> rebirthPassives{
		std::make_tuple( "Anivia", "rebirthready", "rebirthcooldown" ),
		std::make_tuple( "Aatrox", "aatroxpassiveready", "aatroxpassiveactivate" ),
		std::make_tuple( "Zac", "zacrebirthready", "zacrebirthcooldown" )
	};

	int line = 0;

	if ( m_pDrawPassives->Enabled() )
	{
		for ( auto passive : passives )
		{
			if ( hero->HasBuff( std::get<0>( passive ) ) )
			{
				void* buff = hero->GetBuffDataByName( std::get<0>( passive ) );
				auto remainingTime = GBuffData->GetEndTime( buff ) - GGame->Time();

				if ( remainingTime > 0.1f )
				{
					Vec3 w2s;
					if ( GGame->Projection( hero->GetPosition(), &w2s ) && w2s.z < 1.f )
					{
						KDrawing::DrawString( Vec2( w2s.x, w2s.y ), Color::LightRed(), true, std::get<1>( passive ), remainingTime );
						line++;
					}
				}
			}
		}
	}

	if ( m_pDrawRebirthPassives->Enabled() )
	{

		for ( auto passive : rebirthPassives )
		{
			if ( !strcmp( hero->ChampionName(), std::get<0>( passive ) ) )
			{
				Vec3 w2s;
				if ( GGame->Projection( hero->GetPosition(), &w2s ) && w2s.z < 1.f )
				{
					auto passiveUp = hero->HasBuff( std::get<1>( passive ) );

					if ( passiveUp )
					{
						KDrawing::DrawString( Vec2( w2s.x, w2s.y + line * 12 ), Color::LightGreen(), true, "[P]" );
					}
					else
					{
						auto remainingTime = GBuffData->GetEndTime( hero->GetBuffDataByName( std::get<2>( passive ) ) ) - GGame->Time();
						KDrawing::DrawString( Vec2( w2s.x, w2s.y + line * 12 ), Color::LightRed(), true, "[P: %.0f]", remainingTime );
					}
				}
			}
		}
	}
}
