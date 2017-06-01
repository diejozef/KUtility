#pragma once
#include <PluginSDK.h>

enum eSupportItemId
{
	kLocketOfTheIronSolari = 3190,
	kMikaelsCrucible = 3222,
	kFaceOfTheMountain = 3401
};

class Support
{
public:
	explicit Support(IMenu* parentMenu, IUnit* player);
	~Support();

public:
	auto OnBuffAdd(IUnit* source, void* data) -> void;
	auto OnUpdate() -> void;

private:
	IMenu* m_pMenu;
	IMenu* m_pTargetChampions;
	IMenu* m_pSourceChampions;
	IMenuOption* m_pUseMikaels;
	IMenuOption* m_pUseFotm;
	IMenuOption* m_pUseLocket;
	IMenuOption* m_pShieldHp;

private:
	IUnit* m_pPlayer;
	IInventoryItem* m_pMikaels;
	IInventoryItem* m_pFotm;
	IInventoryItem* m_pLocket;
};