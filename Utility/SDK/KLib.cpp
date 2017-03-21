#include "KLib.hpp"

auto KLib::EnemiesInRangeOfHero( IUnit* Hero, float Range ) -> int
{
	int ret = 0;

	for ( auto hero : GEntityList->GetAllHeros( false, true ) )
	{
		if ( hero == nullptr || hero->IsDead() || hero == Hero )
			continue;

		auto distance = (hero->ServerPosition() - Hero->ServerPosition()).Length2D();

		if ( distance <= Range )
			ret++;
	}

	return ret;
}

/* Credits to Sebby */
auto KLib::InCone( Vec3 Pos, Vec3 EndPos, Vec3 StartPos, int Angle, float Range ) -> bool
{
	auto angle = Angle * static_cast<float>(M_PI / 180);
	auto end2 = EndPos.To2DEx() - StartPos.To2DEx();
	auto edge1 = end2.Rotated( -angle / 2 );
	auto edge2 = edge1.Rotated( angle );

	auto point = Pos.To2DEx() - StartPos.To2DEx();
	auto distanceSquared = point.LengthSqr();

	if ( distanceSquared < Range * Range && edge1.CrossProduct( point ) > 0 && point.CrossProduct( edge2 ) > 0 )
		return true;

	return false;
}

auto KLib::UnderTurret( IUnit* Unit, bool IncludeFriendlyTurrets ) -> bool
{
	for ( auto turret : GEntityList->GetAllTurrets( IncludeFriendlyTurrets, true ) )
	{
		if ( turret == nullptr || turret->IsDead() )
			continue;

		auto distance = (turret->ServerPosition() - Unit->GetPosition()).Length2D();

		if ( distance < turret->BoundingRadius() + 775.f )
			return true;
	}

	return false;
}

auto KLib::HasSpellShield( IUnit* Unit ) -> bool
{
	return (Unit->HasBuffOfType( BUFF_SpellShield ) || Unit->HasBuffOfType( BUFF_SpellImmunity ));
}

auto KLib::RealRange( IUnit* Unit ) -> float
{
	return (Unit->BoundingRadius() + Unit->AttackRange());
}

auto KLib::Immobile( IUnit* Unit ) -> bool
{
	return (Unit->HasBuffOfType( BUFF_Charm ) ||
		Unit->HasBuffOfType( BUFF_Fear ) ||
		Unit->HasBuffOfType( BUFF_Knockup ) ||
		Unit->HasBuffOfType( BUFF_Snare ) ||
		Unit->HasBuffOfType( BUFF_Stun ) ||
		Unit->HasBuffOfType( BUFF_Taunt ) ||
		Unit->HasBuffOfType( BUFF_Suppression ) ||
		Unit->HasBuff( "rebirth" ) ||
		Unit->HasBuff( "bardstasis" ) ||
		Unit->HasBuff( "chronorevive" ) ||
		Unit->HasBuff( "lissandrarself" ) ||
		Unit->HasBuff( "teleport_target" ) ||
		Unit->HasBuff( "woogletswitchcap" ) ||
		Unit->HasBuff( "zhonyasringshield" ) ||
		Unit->HasBuff( "aatroxpassivedeath" ) ||
		Unit->HasBuff( "lissandrarenemy2" ));
}

auto KLib::HasSheenBuff( IUnit* Unit ) -> bool
{
	return (Unit->HasBuff( "sheen" ) ||
		Unit->HasBuff( "LichBane" ) ||
		Unit->HasBuff( "dianaarcready" ) ||
		Unit->HasBuff( "ItemFrozenFist" ) ||
		Unit->HasBuff( "sonapassiveattack" ));
}

auto KLib::SetupSkillshot( ISpell2* Spell, float Delay, float Radius, float Speed, float Range ) -> void
{
	Spell->SetOverrideDelay( Delay );
	Spell->SetOverrideRadius( Radius );
	Spell->SetOverrideSpeed( Speed );

	if ( Range > 0.f )
		Spell->SetOverrideRange( Range );
}

auto KLib::CanBeKilled( IUnit * Unit ) -> bool
{
	return (!Unit->HasBuff( "undyingrage" ) &&
		!Unit->HasBuff( "chronoshift" ) &&
		!Unit->IsInvulnerable() &&
		Unit->IsVisible() &&
		Unit->IsTargetable() &&
		!Unit->HasBuffOfType( BUFF_Invulnerability ));
}

auto KLib::CanBeKilledWithAA( IUnit* Local, IUnit* Hero ) -> bool
{
	auto dmg = static_cast<float>(GDamage->GetAutoAttackDamage( Local, Hero, true ));
	return (dmg > Hero->GetHealth());
}

auto KLib::InRange( IUnit* Local, IUnit* Unit, float Range, bool IncludeBounds ) -> bool
{
	auto distance = (Unit->ServerPosition() - Local->ServerPosition()).Length2D();
	auto range = IncludeBounds ? Unit->BoundingRadius() + Range : Range;

	return (distance <= range);
}

auto KLib::InvalidStun( void* BuffData ) -> bool
{
	/* placeholder */
}

auto KLib::InvalidSnare( void* BuffData ) -> bool
{
	/* placeholder */
}

auto KLib::ImmobileUntil( IUnit* Unit, float Time ) -> bool
{
	/* placeholder */
}

auto KLib::HasInvalidCC( IUnit * Unit ) -> bool
{
	/* placeholder */
}
