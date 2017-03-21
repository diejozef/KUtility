#include "AutoBuy.hpp"

AutoBuy::AutoBuy( IMenu* parentMenu, IUnit* player ) :
	m_pPlayer( player )
{
	m_pMenu = parentMenu->AddMenu( "AutoBuy" );
	m_pAutoBuyBlueWard = m_pMenu->CheckBox( "Buy Blue Trinket", false );
	m_pAutoBuySweeper = m_pMenu->CheckBox( "Buy Sweeper", false );
	m_pUpgrade = m_pMenu->CheckBox( "Only Upgrade", false );
}

AutoBuy::~AutoBuy()
{
	m_pMenu->Remove();
}

auto AutoBuy::OnUpdate() -> void
{
	auto buyBlueWard = m_pAutoBuyBlueWard->Enabled();
	auto buySweeper = m_pAutoBuySweeper->Enabled();
	auto upgrade = m_pUpgrade->Enabled();

	if ( !buyBlueWard && !buySweeper && !upgrade )
		return;

	auto hasBlueWard = m_pPlayer->HasItemId( kFarsightAlteration );
	auto hasSweeper = m_pPlayer->HasItemId( kOracleAlteration );

	if ( hasBlueWard || hasSweeper )
		return;

	if ( m_pPlayer->GetLevel() >= 9 && InFountain() )
	{
		if ( upgrade )
		{
			if ( m_pPlayer->HasItemId( kWardingTotem ) && !hasBlueWard )
			{
				GGame->BuyItem( kFarsightAlteration );
			}
			else if ( m_pPlayer->HasItemId( kSweepingLens ) && !hasSweeper )
			{
				GGame->BuyItem( kOracleAlteration );
			}

			return;
		}

		if ( buyBlueWard && !hasBlueWard )
		{
			GGame->BuyItem( kFarsightAlteration );
		}
		else if ( buySweeper && !hasSweeper )
		{
			GGame->BuyItem( kOracleAlteration );
		}
	}
}

auto AutoBuy::InFountain() -> bool
{
	auto turrets = GEntityList->GetAllTurrets( true, false );
	auto i = std::find_if( turrets.begin(), turrets.end(), []( IUnit* turret )
	{
		if ( turret == nullptr )
			return false;

		if ( strstr( turret->GetObjectName(), "Shrine" ) )
			return true;

		return false;
	} );

	if ( i != turrets.end() )
	{
		auto fountain = *i;
		auto distance = ( m_pPlayer->ServerPosition() - fountain->GetPosition() ).Length2D();

		if ( distance < 1100.0f )
			return true;
	}

	return false;
}
