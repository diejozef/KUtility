#include "WardManager.hpp"

WardManager::WardManager( IMenu* parentMenu, IUnit* player ) :
	m_pPlayer( player ),
	m_bErasing( false )
{
	m_pMenu = parentMenu->AddMenu( "Wards" );
	m_pDrawWards = m_pMenu->CheckBox( "Draw Wards", false );
	m_pDrawRange = m_pMenu->CheckBox( "Draw Vision Range", false );

	m_vecWardDefinitions = {
		{ "YellowTrinket" ,			"TrinketTotemLvl1" ,	60.0f,		kSightWard,		1100.0f },	// basic trinket
		{ "YellowTrinketUpgrade",	"TrinketTotemLvl2",		120.0f,		kSightWard,		1100.0f },	// this is probably obsolete
		{ "SightWard" ,				"ItemGhostWard" ,		150.0f,		kSightWard,		1100.0f },	// sightstone
		{ "SightWard" ,				"SightWard" ,			150.0f,		kSightWard,		1100.0f },	// sightstone
		{ "MissileWard" ,			"MissileWard" ,			180.0f,		kSightWard,		1100.0f },	// missile marker?
		{ "BlueTrinket" ,			"TrinketOrbLvl3" ,		9999.0f,	kBlueWard,		1100.0f },	// blue trinket (lvl9)
		{ "JammerDevice",			"JammerDevice",			9999.0f,	kPinkWard,		900.0f },	// control ward
		{ "TeemoMushroom" ,			"BantamTrap" ,			600.0f,		kTrap,			200.0f },	// teemo trap
		{ "ShacoBox" ,				"JackInTheBox" ,		60.0f,		kTrap,			200.0f },	// shaco trap
		{ "Noxious_Trap" ,			"BantamTrap" ,			600.0f,		kTrap,			200.0f },	// teemo trap
		{ "JhinTrap" ,				"JhinE" ,				120.0f,		kTrap,			130.0f },	// jhin trap
		{ "CaitlynTrap",			"CaitlynYordleTrap",	90.0f,		kCaitlynTrap,	50.0f }		// cait trap
	};

	m_iNextEraseTick = GGame->CurrentTick() + 8000;
}

WardManager::~WardManager()
{
	m_pMenu->Remove();
}

auto WardManager::OnRender() -> void
{
	// cleaning time, don't render
	if ( !m_pDrawWards->Enabled() || m_bErasing )
		return;

	for ( auto ward : m_vecWards )
	{
		if ( !ward.Valid() )
			continue;

		ward.Draw( m_pDrawRange->Enabled() );
	}
}

auto WardManager::OnUpdate() -> void
{
	if ( GGame->CurrentTick() == m_iNextEraseTick )
	{
		m_bErasing = true;

		auto lastWard = std::remove_if( m_vecWards.begin(), m_vecWards.end(), []( const Ward& ward )
		{
			return !ward.Valid();
		} );
		m_vecWards.resize( lastWard - m_vecWards.begin() );

		m_iNextEraseTick = GGame->CurrentTick() + 8000;
		m_bErasing = false;
	}
}

auto WardManager::OnSpellCast( const CastedSpell& spell ) -> void
{
	auto caster = spell.Caster_;

	if ( caster == nullptr || !caster->IsHero() || !caster->IsEnemy( m_pPlayer ) )
		return;

	for ( auto wardtype : m_vecWardDefinitions )
	{
		if ( !strcmp( wardtype.m_strSpellName, spell.Name_ ) )
		{
			auto duration = wardtype.m_flDuration;

			if ( caster->GetLevel() > 1 && strstr( spell.Name_, "TrinketTotem" ) )
				duration += ( 3.5f * caster->GetLevel() );

			m_vecWards.emplace_back( Ward( spell.EndPosition_, wardtype, duration ) );

			break;
		}
	}
}

auto WardManager::OnCreateObject( IUnit* object ) -> void
{
	if ( object->UnitFlags() != FL_CREEP )
		return;

	auto objectName = object->GetObjectName();
	if ( objectName == nullptr || objectName[ 0 ] == '\0' )
		return;

	if ( !strcmp( objectName, "WardCorpse" ) )
	{
		HandleWardCorpse( object );
		return;
	}

	if ( !object->IsEnemy( m_pPlayer ) )
		return;

	auto name = object->ChampionName();
	if ( name == nullptr || name[ 0 ] == '\0' )
		return;

	for ( auto wardtype : m_vecWardDefinitions )
	{
		if ( !strcmp( wardtype.m_strObjectName, name ) )
		{
			if ( std::find_if( m_vecWards.begin(), m_vecWards.end(), [&]( const Ward& ward ) -> bool
			{
				if ( !ward.Valid() )
					return false;

				auto distance = ( ward.Pos() - object->GetPosition() ).Length2D();
				auto maxDistance = wardtype.m_type == kCaitlynTrap ? 50.0f : 150.0f;

				return ( distance < maxDistance );

			} ) == m_vecWards.end() )
			{
				auto duration = wardtype.m_flDuration;

				if ( wardtype.m_type == kSightWard || wardtype.m_type == kTrap )
					duration = object->GetMana();

				m_vecWards.emplace_back( Ward( object->GetPosition(), wardtype, duration ) );

				break;
			}
		}
	}
}

auto WardManager::OnUnitDeath( IUnit* object ) -> void
{
	if ( object->UnitFlags() != FL_CREEP || object->GetHealth() > 100.0f || !object->IsEnemy( m_pPlayer ) )
		return;

	auto name = object->ChampionName();

	for ( auto& ward : m_vecWards )
	{
		if ( !ward.Valid() )
			continue;

		if ( !strcmp( ward.Info().m_strObjectName, name ) )
		{
			auto pos = object->GetPosition();

			if ( pos == ward.Pos() )
			{
				ward.Invalidate();
				break;
			}
			else
			{
				auto distance = ( pos - ward.Pos() ).Length2D();
				auto maxDistance = ward.Info().m_type == kCaitlynTrap ? 50.0f : 125.0f;

				if ( distance < maxDistance )
				{
					ward.Invalidate();
					break;
				}
			}
		}
	}
}

auto WardManager::HandleWardCorpse( IUnit* corpse ) -> void
{
	auto pos = corpse->GetPosition();

	for ( auto& ward : m_vecWards )
	{
		if ( !ward.Valid() )
			continue;

		if ( pos == ward.Pos() )
		{
			ward.Invalidate();
			break;
		}
		else
		{
			auto distance = ( pos - ward.Pos() ).Length2D();

			if ( distance < 20.0f )
			{
				ward.Invalidate();
				break;
			}
		}
	}
}