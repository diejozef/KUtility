#include "SpellManager.hpp"

SpellManager::SpellManager( IMenu* parentMenu, IUnit* player ) :
	m_pPlayer( player ),
	m_bErasing( false )
{
	m_pMenu = parentMenu->AddMenu( "Spells" );
	m_pDrawSkillshots = m_pMenu->CheckBox( "Draw Skillshots", false );
	m_pDrawDashes = m_pMenu->CheckBox( "Draw Dashes", false );

	m_iNextEraseTick = GGame->CurrentTick() + 8000;
}

SpellManager::~SpellManager()
{
	m_pMenu->Remove();
}

auto SpellManager::OnRender() -> void
{
	// cleaning in progress, don't render
	if ( m_bErasing )
		return;

	if ( m_pDrawSkillshots->Enabled() )
	{
		std::for_each( m_vecSkillshots.begin(), m_vecSkillshots.end(), []( const SkillshotIndicator& skillshot )
		{
			if ( skillshot.Valid() )
				skillshot.Draw();
		} );
	}

	if ( m_pDrawDashes->Enabled() )
	{
		std::for_each( m_vecDashes.begin(), m_vecDashes.end(), []( const DashIndicator& dash )
		{
			if ( dash.Valid() )
				dash.Draw();
		} );
	}
}

auto SpellManager::OnUpdate() -> void
{
	if ( GGame->CurrentTick() == m_iNextEraseTick )
	{
		m_bErasing = true;

		auto lastSkillshot = std::remove_if( m_vecSkillshots.begin(), m_vecSkillshots.end(), []( const SkillshotIndicator& skillshot )
		{
			return !skillshot.Valid();
		} );
		m_vecSkillshots.resize( lastSkillshot - m_vecSkillshots.begin() );

		auto lastDash = std::remove_if( m_vecDashes.begin(), m_vecDashes.end(), []( const DashIndicator& dash )
		{
			return !dash.Valid();
		} );
		m_vecDashes.resize( lastDash - m_vecDashes.begin() );

		m_iNextEraseTick = GGame->CurrentTick() + 8000;
		m_bErasing = false;
	}
	else
	{
		for ( auto missile : GEntityList->GetAllMissiles( false, true ) )
		{
			if ( missile == nullptr || !GMissileData->GetCaster( missile )->IsHero() || GMissileData->IsAutoAttack( missile ) )
				continue;

			for ( auto& skillshot : m_vecSkillshots )
			{
				if ( !skillshot.Valid() || skillshot.MissileNetworkId() == -1 )
					continue;

				if ( skillshot.MissileNetworkId() == missile->GetNetworkId() )
				{
					auto pos = missile->GetPosition();

					if ( skillshot.Type() == kSpellSkillshotLine )
						pos.y = m_pPlayer->GetPosition().y;

					skillshot.SetStartPos( pos );
				}
			}
		}
	}
}

auto SpellManager::OnSpellCast( const CastedSpell& spell ) -> void
{
	if ( spell.AutoAttack_ || !spell.Caster_->IsHero() || !spell.Caster_->IsEnemy( m_pPlayer ) )
		return;

	if ( !strcmp( spell.Name_, "VarusQ" ) || !strcmp( spell.Name_, "ViktorDeathRay" ) )
		return;

	auto slot = static_cast< eSpellSlot >( GSpellData->GetSlot( spell.Data_ ) );
	if ( slot == kSlotUnknown )
		return;

	DashInfo dashInfo;
	if ( SpellDb::GetDash( spell.Name_, dashInfo ) )
	{
		auto start = spell.Position_;
		auto end = spell.EndPosition_;
		auto distance = ( end - start ).Length2D();

		if ( distance > dashInfo.m_flRange )
		{
			auto dir = ( end - start ).VectorNormalize();
			end = start + dir * dashInfo.m_flRange;
		}

		m_vecDashes.emplace_back( DashIndicator( start, end, 1.5f ) );

		return;
	}

	SpellInfo info;
	if ( SpellDb::GetSpell( spell.Caster_->ChampionName(), spell.Name_, slot, info ) )
	{
		float distance = ( spell.EndPosition_ - spell.Position_ ).Length2D();

		if ( distance > 10.f )
		{
			auto duration = 1.25f;
			auto speed = info.m_flSpeed;
			auto range = m_pPlayer->BoundingRadius() + info.m_flRange;
			auto type = info.m_type;

			auto start = spell.Position_;
			auto end = spell.EndPosition_;

			if ( type == kSpellSkillshotLine )
				start.y = end.y = m_pPlayer->ServerPosition().y;

			auto dir = ( end - start ).VectorNormalize();

			if ( type == kSpellSkillshotCircle )
			{
				auto castedRange = ( end - spell.Caster_->GetPosition() ).Length2D();

				if ( castedRange > range )
					end = start + dir * range;
			}
			else if ( type == kSpellSkillshotLine )
			{
				end = start + dir * range;
			}

			if ( info.m_flSpeed > 0.0f && info.m_type == kSpellSkillshotLine )
				duration = info.m_flDelay + ( range / speed );

			m_vecSkillshots.emplace_back( SkillshotIndicator( start, end, duration, info ) );
		}
	}
}

auto SpellManager::OnCreateObject( IUnit* object ) -> void
{
	if ( !object->IsMissile() || object->UnitFlags() != FL_MISSILE )
		return;

	auto caster = GMissileData->GetCaster( object );
	if ( !caster->IsHero() || !caster->IsEnemy( m_pPlayer ) || GMissileData->IsAutoAttack( object ) )
		return;

	auto missileName = GMissileData->GetName( object );
	if ( missileName == nullptr || missileName == '\0' )
		return;

	SpellInfo info;
	if ( !SpellDb::GetSpellByMissile( missileName, info ) )
		return;

	auto& i = std::find_if( m_vecSkillshots.begin(), m_vecSkillshots.end(), [&]( const SkillshotIndicator& skillshot )
	{
		if ( !skillshot.Valid() )
			return false;

		auto info = skillshot.Info();

		return ( !_stricmp( info.m_strProjectileName, missileName )
				|| !_stricmp( info.m_strSpellName, missileName ) );
	} );

	if ( i == m_vecSkillshots.end() )
	{
		auto duration = 1.5f;
		auto speed = info.m_flSpeed;
		auto range = m_pPlayer->BoundingRadius() + info.m_flRange;

		auto start = GMissileData->GetStartPosition( object );
		auto end = GMissileData->GetEndPosition( object );
		start.y = end.y = m_pPlayer->ServerPosition().y;

		auto dir = ( end - start ).VectorNormalize();

		end = ( info.m_type == kSpellSkillshotLine ) ? start + dir * range : end;

		if ( info.m_flSpeed > 0.0f && info.m_type == kSpellSkillshotLine )
			duration = range / info.m_flSpeed;

		m_vecSkillshots.emplace_back( SkillshotIndicator( start, end, duration, info, object->GetNetworkId() ) );
	}
	else if ( i->MissileNetworkId() == -1 )
	{
		i->SetMissileNetworkId( object->GetNetworkId() );
	}
}

auto SpellManager::OnDestroyObject( IUnit* object ) -> void
{
	if ( !object->IsMissile() || object->UnitFlags() != FL_MISSILE
		|| !GMissileData->GetCaster( object )->IsHero()
		|| !GMissileData->GetCaster( object )->IsEnemy( m_pPlayer )
		|| GMissileData->IsAutoAttack( object ) )
		return;

	for ( auto& skillshot : m_vecSkillshots )
	{
		if ( !skillshot.Valid() )
			continue;

		if ( GGame->Time() > skillshot.EndTime() || skillshot.MissileNetworkId() == object->GetNetworkId() )
		{
			skillshot.Invalidate();
			break;
		}

		auto distance = ( skillshot.StartPos() - object->GetPosition() ).Length2D();

		if ( distance < 15.0f )
			skillshot.Invalidate();
	}
}
