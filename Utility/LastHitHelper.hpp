#pragma once
#include <PluginSDK.h>

enum eOnHitItemId
{
	kRecurveBow = 1043,
	kDoransShield = 1054,
	kKircheisShard = 2015,
	kSheen = 3057,
	kTrinityForce = 3078,
	kStatikkShiv = 3087,
	kWitsEnd = 3091,
	kRapidFirecannon = 3094,
	kNashorsTooth = 3115,
	kGuinsoosRageblade = 3124,
	kBladeOfTheRuinedKing = 3153,

};

class LastHitHelper
{
public:
	explicit LastHitHelper(IMenu* parentMenu, IUnit* player);
	~LastHitHelper();

public:
	auto OnGetAutoAttackDamage(OnGetAutoAttackDamageArgs* data) -> float;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pEnable;

private:
	IUnit* m_pPlayer;
};