#include "AARange.hpp"

AARange::AARange( IMenu* parentMenu, IUnit* player ) :
	m_pPlayer( player )
{
	m_pMenu = parentMenu->AddMenu( "AutoAttack Range" );
	m_pEnable = m_pMenu->CheckBox( "Enable", false );
}

AARange::~AARange()
{
	m_pMenu->Remove();
}

auto AARange::OnRender() -> void
{
	if ( !m_pEnable->Enabled() || m_pPlayer->IsDead() )
		return;

	auto radius = m_pPlayer->BoundingRadius() + m_pPlayer->AttackRange();

	GRender->DrawOutlinedCircle( m_pPlayer->GetPosition(), Color::Yellow().Get( 100 ), radius );
}
