#include "Spells.hpp"

SkillshotIndicator::SkillshotIndicator() :
	m_bValid( false )
{

}

SkillshotIndicator::SkillshotIndicator( const Vec3& pos, const Vec3& end, float duration, const SpellInfo& info ) :
	m_vecStart( pos ),
	m_vecEnd( end ),
	m_info( info ),
	m_iMissileNetworkId( -1 ),
	m_bValid( true )
{
	m_flEndTime = GGame->Time() + duration;
	m_flPlayerPosY = GEntityList->Player()->ServerPosition().y;
}

SkillshotIndicator::SkillshotIndicator( const Vec3& pos, const Vec3& end, float duration, const SpellInfo& info, int missileNetworkId ) :
	m_vecStart( pos ),
	m_vecEnd( end ),
	m_info( info ),
	m_iMissileNetworkId( missileNetworkId ),
	m_bValid( true )
{
	m_flEndTime = GGame->Time() + duration;
	m_flPlayerPosY = GEntityList->Player()->ServerPosition().y;
}

SkillshotIndicator::~SkillshotIndicator()
{

}

auto SkillshotIndicator::Valid() const -> bool
{
	return (m_bValid && m_flEndTime >= GGame->Time());
}

auto SkillshotIndicator::Invalidate() -> void
{
	m_bValid = false;
	m_flEndTime = GGame->Time() - 1.f;
}

auto SkillshotIndicator::SetStartPos( const Vec3& pos ) -> void
{
	m_vecStart = pos;
}

auto SkillshotIndicator::SetEndPos( const Vec3& pos ) -> void
{
	m_vecEnd = pos;
}

auto SkillshotIndicator::SetMissileNetworkId( int id ) -> void
{
	m_iMissileNetworkId = id;
}

auto SkillshotIndicator::Rectangle() const -> std::array<Vec2, 4>
{
	auto start = m_vecStart.To2DEx();
	auto end = m_vecEnd.To2DEx();
	auto dir = (end - start).VectorNormalize();
	auto perpDir = dir.Perpendicular();
	auto radius = m_info.m_flRadius;

	return{
		Vec2( start + perpDir * radius ), // rightStart
		Vec2( start - perpDir * radius ), // leftStart
		Vec2( end + perpDir * radius ), // rightEnd
		Vec2( end - perpDir * radius ) // leftEnd
	};
}

auto SkillshotIndicator::Draw() const -> void
{
	if ( m_info.m_type == kSpellSkillshotLine )
	{
		std::array<Vec3, 4> out;
		std::array<Vec2, 4> rect = Rectangle();

		GGame->Projection( Vec3( rect[ 0 ].x, m_flPlayerPosY, rect[ 0 ].y ), &out[ 0 ] );
		GGame->Projection( Vec3( rect[ 1 ].x, m_flPlayerPosY, rect[ 1 ].y ), &out[ 1 ] );
		GGame->Projection( Vec3( rect[ 2 ].x, m_flPlayerPosY, rect[ 2 ].y ), &out[ 2 ] );
		GGame->Projection( Vec3( rect[ 3 ].x, m_flPlayerPosY, rect[ 3 ].y ), &out[ 3 ] );

		if ( out[ 0 ].z < 1.f && out[ 1 ].z < 1.f && out[ 2 ].z < 1.f && out[ 3 ].z < 1.f )
		{
			auto color = Color::White().Get();

			GRender->DrawLine( out[ 0 ].To2D(), out[ 2 ].To2D(), color );
			GRender->DrawLine( out[ 1 ].To2D(), out[ 3 ].To2D(), color );
			GRender->DrawLine( out[ 0 ].To2D(), out[ 1 ].To2D(), color );
			GRender->DrawLine( out[ 3 ].To2D(), out[ 2 ].To2D(), color );
		}
	}
	else if ( m_info.m_type == kSpellSkillshotCircle )
	{
		GRender->DrawOutlinedCircle( m_vecEnd, Color::White().Get(), m_info.m_flRadius );
	}
}

DashIndicator::DashIndicator()
{

}

DashIndicator::~DashIndicator()
{

}

DashIndicator::DashIndicator( const Vec3& posFrom, const Vec3& posTo, float duration ) :
	m_vecStart( posFrom ),
	m_vecEnd( posTo )
{
	m_flEndTime = GGame->Time() + duration;
}

auto DashIndicator::Draw( void ) const -> void
{
	std::array<Vec3, 2> w2s;

	if ( GGame->Projection( m_vecStart, &w2s[ 0 ] ) && w2s[ 0 ].z < 1.f
		&& GGame->Projection( m_vecEnd, &w2s[ 1 ] ) && w2s[ 1 ].z < 1.f )
	{
		GRender->DrawLine( w2s[ 0 ].To2D(), w2s[ 1 ].To2D(), Color::White().Get() );
	}
}

auto DashIndicator::Valid( void ) const -> bool
{
	return (m_flEndTime >= GGame->Time());
}
