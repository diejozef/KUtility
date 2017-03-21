#include "Clicks.hpp"

Clicks::Clicks( IMenu* parentMenu )
{
	m_pMenu = parentMenu->AddMenu( "Clicks" );
	m_pEnable = m_pMenu->CheckBox( "Enable", false );
}

Clicks::~Clicks()
{
	m_pMenu->Remove();
}

auto Clicks::OnRenderEnemy( IUnit* hero ) -> void
{
	if ( !m_pEnable->Enabled() || hero->IsDead() || !hero->IsVisible() )
		return;

	auto waypointList = hero->GetWaypointList();

	if ( waypointList.size() > 1 )
	{
		auto start = hero->GetPosition();
		auto end = waypointList.back();
		std::array<Vec3, 2> w2s;

		if ( GGame->Projection( start, &w2s[ 0 ] ) && w2s[ 0 ].z < 1.f &&
			GGame->Projection( end, &w2s[ 1 ] ) && w2s[ 1 ].z < 1.f )
		{
			GRender->DrawLine( w2s[ 0 ].To2D(), w2s[ 1 ].To2D(), Color::White().Get( 150U ) );
		}
	}
}
