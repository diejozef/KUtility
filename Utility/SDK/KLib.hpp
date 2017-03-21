#pragma once
#include <PluginSDK.h>

namespace KLib
{
	auto EnemiesInRangeOfHero( IUnit* Hero, float Range ) -> int;
	auto UnderTurret( IUnit* Unit, bool IncludeFriendlyTurrets ) -> bool;
	auto HasSpellShield( IUnit* Unit ) -> bool;
	auto RealRange( IUnit* Unit ) -> float;
	auto Immobile( IUnit* Unit ) -> bool;
	auto HasSheenBuff( IUnit* Unit ) -> bool;
	auto SetupSkillshot( ISpell2* Spell, float Delay, float Radius, float Speed, float Range = 0.f ) -> void;
	auto CanBeKilled( IUnit* Unit ) -> bool;
	auto CanBeKilledWithAA( IUnit* Local, IUnit* Hero ) -> bool;
	auto InRange( IUnit* Local, IUnit* Unit, float Range, bool IncludeBounds ) -> bool;
	auto InvalidStun( void* BuffData ) -> bool;
	auto InvalidSnare( void* BuffData ) -> bool;
	auto ImmobileUntil( IUnit* Unit, float Time ) -> bool;
	auto HasInvalidCC( IUnit* Unit ) -> bool;
	auto InCone( Vec3 Pos, Vec3 EndPos, Vec3 StartPos, int Angle, float Range ) -> bool;
}