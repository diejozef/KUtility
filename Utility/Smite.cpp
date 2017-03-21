#include "Smite.hpp"
#include <KLib.hpp>

Smite::Smite( IMenu* parentMenu, eSpellSlot smiteSlot, IUnit* player ) :
	m_pPlayer( player )
{
	m_pMenu = parentMenu->AddMenu( "Smite" );
	m_pAutoSmite = m_pMenu->CheckBox( "Auto Use", false );
	m_pDrawToggle = m_pMenu->CheckBox( "Draw Toggle Status", false );
	m_pUseSmiteCombo = m_pMenu->CheckBox( "[beta] Use Combo", false );

	m_pSmite = GPluginSDK->CreateSpell2( smiteSlot, kTargetCast, false, false, kCollidesWithNothing );
	m_pSmite->SetOverrideRange( m_pPlayer->BoundingRadius() + 500.0f ); // temporary fix

	m_bHasSmiteCombo = SpellDb::GetSmiteSpell( m_pPlayer->ChampionName(), m_smiteComboInfo );
	if ( m_bHasSmiteCombo )
	{
		m_pSmiteCombo = GPluginSDK->CreateSpell2( m_smiteComboInfo.m_slot, m_smiteComboInfo.m_type, false, false, kCollidesWithNothing );
		m_pSmiteCombo->SetOverrideRange( m_smiteComboInfo.m_flRange );
	}
}

Smite::~Smite()
{
	m_pMenu->Remove();
}

auto Smite::OnUpdate() -> void
{
	if ( m_pAutoSmite->Enabled() && m_pSmite->IsReady() )
		AutoSmite();
}

auto Smite::OnRender() -> void
{
	if ( m_pDrawToggle->Enabled() && m_pAutoSmite->Enabled() )
	{
		auto w2s = Vec3( 0.f, 0.f, 0.f );

		if ( GGame->Projection( m_pPlayer->GetPosition(), &w2s ) && w2s.z < 1.f )
		{
			w2s.y += 13;

			auto combo = m_pUseSmiteCombo->Enabled() && m_bHasSmiteCombo;
			GRender->DrawString( w2s.To2D(), Color::LightGreen().Get(), "AutoSmite [Combo: %s]", combo ? "ON" : "OFF" );
		}
	}
}

auto Smite::AutoSmite() -> void
{
	for ( auto minion : GEntityList->GetAllMinions( false, false, true ) )
	{
		if ( minion == nullptr || minion->IsDead() || !KLib::InRange( m_pPlayer, minion, m_pSmite->Range(), true ) )
			continue;

		auto name = minion->GetObjectName();

		if ( name == nullptr || name[ 0 ] == '\0' )
			continue;

		if ( StrStrIA( name, "dragon" ) || StrStrIA( name, "baron" ) || StrStrIA( name, "herald" ) )
		{
			auto dmg = static_cast<float>( GDamage->GetSummonerSpellDamage( m_pPlayer, minion, kSummonerSpellSmite ) );
			auto health = GHealthPrediction->GetPredictedHealth( minion, kLastHitPrediction, 150, 150 );
			auto extraDmg = GetExtraSmiteDamage( minion );

			if ( extraDmg < 1.0f && dmg > health )
			{
				m_pSmite->CastOnUnit( minion );
				break;
			}
			else if ( dmg + extraDmg > health )
			{
				m_smiteComboInfo.m_type == kTargetCast ? m_pSmiteCombo->CastOnUnit( minion ) : m_pSmiteCombo->CastOnPosition( minion->GetPosition() );
				break;
			}
		}
	}
}

auto Smite::GetExtraSmiteDamage( IUnit* target ) -> float
{
	if ( m_pUseSmiteCombo->Enabled()
		&& m_bHasSmiteCombo
		&& m_pSmiteCombo->IsReady()
		&& target->IsValidTarget() && KLib::InRange( m_pPlayer, target, m_pSmiteCombo->Range(), true ) )
		return static_cast<float>( GDamage->GetSpellDamage( m_pPlayer, target, m_smiteComboInfo.m_slot ) );
	else
		return 0.0f;
}
