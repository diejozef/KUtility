#include "DrawManager.hpp"

DrawManager::DrawManager( IMenu* parentMenu, IUnit* player, InputManager* inputManager ) :
	m_pPlayer( player ),
	m_pInputManager( inputManager )
{
	m_pMenu = parentMenu->AddMenu( "Drawings" );

	auto enemies = GEntityList->GetAllHeros( false, true );
	std::for_each( enemies.begin(), enemies.end(), [&]( IUnit* hero )
	{
		if ( hero != nullptr )
			m_vecFowTrackers.emplace_back( FowTracker( hero ) );
	} );

	m_pSpellInfoBox = new SpellInfoBox( m_pMenu, &m_vecFowTrackers, m_pInputManager );
	m_pLastPositions = new LastPositions( m_pMenu, &m_vecFowTrackers );
	m_pSpellTracker = new SpellTracker( m_pMenu, m_pPlayer );
	m_pPassives = new Passives( m_pMenu );
	m_pBoundingBox = new BoundingBox( m_pMenu );
	m_pTurretRanges = new TurretRanges( m_pMenu, m_pPlayer );
	m_pClicks = new Clicks( m_pMenu );
	m_pClones = new Clones( m_pMenu );
	m_pAARange = new AARange( m_pMenu, m_pPlayer );
	m_pRecallManager = new RecallManager( parentMenu, m_pPlayer, m_pInputManager, &m_vecFowTrackers );
}

DrawManager::~DrawManager()
{
	_delete( m_pSpellInfoBox );
	_delete( m_pLastPositions );
	_delete( m_pSpellTracker );
	_delete( m_pPassives );
	_delete( m_pBoundingBox );
	_delete( m_pTurretRanges );
	_delete( m_pClicks );
	_delete( m_pClones );
	_delete( m_pAARange );
	_delete( m_pRecallManager );

	m_pMenu->Remove();
}

auto DrawManager::OnRender() -> void
{
	for ( auto hero : GEntityList->GetAllHeros( true, true ) )
	{
		if ( hero == nullptr )
			continue;

		if ( hero->IsEnemy( m_pPlayer ) )
		{
			m_pPassives->OnRenderEnemy( hero );
			m_pLastPositions->OnRenderEnemy( hero );
			m_pBoundingBox->OnRenderEnemy( hero );
			m_pClicks->OnRenderEnemy( hero );
			m_pClones->OnRenderEnemy( hero );
		}

		m_pSpellTracker->OnRenderHero( hero );
	}

	m_pTurretRanges->OnRender();
	m_pSpellInfoBox->OnRender();
	m_pAARange->OnRender();
	m_pRecallManager->OnRender();
}

auto DrawManager::OnUpdate() -> void
{
	m_pSpellInfoBox->OnUpdate();
	m_pRecallManager->OnUpdate();
}

auto DrawManager::OnEnterFow( IUnit* unit ) -> void
{
	if ( !unit->IsHero() )
		return;

	auto& i = std::find_if( m_vecFowTrackers.begin(), m_vecFowTrackers.end(), [&]( const FowTracker& tracker )
	{
		return tracker.Unit() == unit;
	} );

	if ( i != m_vecFowTrackers.end() )
		i->OnEnterFow( GGame->Time() );
}

auto DrawManager::OnExitFow( IUnit* unit ) -> void
{
	if ( !unit->IsHero() )
		return;

	auto& i = std::find_if( m_vecFowTrackers.begin(), m_vecFowTrackers.end(), [&]( const FowTracker& tracker )
	{
		return tracker.Unit() == unit;
	} );

	if ( i != m_vecFowTrackers.end() )
		i->OnExitFow();
}

auto DrawManager::OnUnitDeath( IUnit* unit ) -> void
{
	if ( !unit->IsHero() )
		return;

	auto& i = std::find_if( m_vecFowTrackers.begin(), m_vecFowTrackers.end(), [&]( const FowTracker& tracker )
	{
		return tracker.Unit() == unit;
	} );

	if ( i != m_vecFowTrackers.end() )
		i->OnResetPos();
}

auto DrawManager::OnTeleport( OnTeleportArgs* data ) -> void
{
	m_pRecallManager->OnTeleport( data );
}