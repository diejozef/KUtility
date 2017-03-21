#include "Wards.hpp"

Ward::Ward() :
	m_bValid( false )
{

}

Ward::~Ward()
{

}

Ward::Ward( const Vec3& pos, const WardDefinition& info, float duration ) :
	m_vecPos( pos ),
	m_info( info ),
	m_bValid( true )
{
	m_flEndTime = GGame->Time() + duration;
}

auto Ward::Valid() const -> bool
{
	return ( m_bValid && m_flEndTime >= GGame->Time() );
}

auto Ward::Invalidate() -> void
{
	m_bValid = false;
	m_flEndTime = GGame->Time() - 1.0f;
}

auto Ward::Draw( bool drawRange ) -> void
{
	auto clr = Color( 0, 0, 0 );

	auto type = m_info.m_type;

	switch ( type )
	{
	case kSightWard: 
		clr = Color::Yellow(); 
		break;
	case kTrap: 
		clr = Color::DarkGreen(); 
		break;
	case kPinkWard: 
		clr = Color::Magenta(); 
		break;
	case kBlueWard: 
		clr = Color::LightBlue2(); 
		break;
	case kCaitlynTrap: 
		clr = Color::White(); 
		break;
	}

	auto radius = drawRange ? m_info.m_flRange : 50.0f;
	auto alpha = static_cast< uint8_t >( drawRange ? 130.0f : 255.0f );

	GRender->DrawOutlinedCircle( m_vecPos, clr.Get( alpha ), radius );
	
	auto w2s = Vec3( 0.f, 0.f, 0.f );

	if ( type != kPinkWard && type != kBlueWard && GGame->Projection( m_vecPos, &w2s ) && w2s.z < 1.f )
	{
		auto remainingTime = m_flEndTime - GGame->Time();
		KDrawing::DrawString( Vec2( w2s.x - 3.0f, w2s.y ), clr, true, "%.0f s", remainingTime );
	}
}
