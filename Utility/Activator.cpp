#include "Activator.hpp"

Activator::Activator( IMenu* parentMenu, IUnit* player ) :
	m_pPlayer( player ),
	m_pSmite( nullptr ),
	m_pIgnite( nullptr )
{
	m_pMenu = parentMenu->AddMenu( "Activator" );

	auto smiteSlot = GetSmite();
	m_bHasSmite = ( smiteSlot != kSlotUnknown );
	if ( m_bHasSmite )
		m_pSmite = new Smite( m_pMenu, smiteSlot, m_pPlayer );

	auto igniteSlot = GetIgnite();
	m_bHasIgnite = ( igniteSlot != kSlotUnknown );
	if ( m_bHasIgnite )
		m_pIgnite = new Ignite( m_pMenu, igniteSlot, m_pPlayer );

	m_pCleanse = new Cleanse( m_pMenu, m_pPlayer );
	m_pAutoBuy = new AutoBuy( m_pMenu, m_pPlayer );
	m_pAutoLevelUp = new AutoLevelUp( m_pMenu, m_pPlayer );
}

Activator::~Activator()
{
	if ( m_pSmite != nullptr )
		_delete( m_pSmite );

	if ( m_pIgnite != nullptr )
		_delete( m_pIgnite );

	_delete( m_pCleanse );
	_delete( m_pAutoBuy );
	_delete( m_pAutoLevelUp );

	m_pMenu->Remove();
}

auto Activator::OnRender() -> void
{
	if ( m_bHasSmite )
		m_pSmite->OnRender();
}

auto Activator::OnUpdate() -> void
{
	if ( m_bHasSmite )
		m_pSmite->OnUpdate();

	if ( m_bHasIgnite )
		m_pIgnite->OnUpdate();

	m_pCleanse->OnUpdate();
	m_pAutoBuy->OnUpdate();
}

auto Activator::OnBuffAdd( IUnit* source, void* data ) -> void
{
	m_pCleanse->OnBuffAdd( source, data );
}

auto Activator::OnLevelUp( IUnit * source, int level ) -> void
{
	m_pAutoLevelUp->OnLevelUp( source, level );
}

auto Activator::GetSmite() -> eSpellSlot
{
	if ( strstr( m_pPlayer->GetSpellName( kSummonerSlot1 ), "SummonerSmite" ) )
		return kSummonerSlot1;
	else if ( strstr( m_pPlayer->GetSpellName( kSummonerSlot2 ), "SummonerSmite" ) )
		return kSummonerSlot2;

	return kSlotUnknown;
}

auto Activator::GetIgnite() -> eSpellSlot
{
	return m_pPlayer->GetSpellSlot( "SummonerDot" );
}
