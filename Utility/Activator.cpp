#include "Activator.hpp"

Activator::Activator(IMenu* parentMenu, IUnit* player, InputManager* inputManager) :
	m_pPlayer(player),
	m_pInputManager(inputManager),
	m_pSmite(nullptr),
	m_pIgnite(nullptr),
	m_pLantern(nullptr)
{
	m_pMenu = parentMenu->AddMenu("Activator");

	auto smiteSlot = GetSmite();
	m_bHasSmite = (smiteSlot != kSlotUnknown);
	if (m_bHasSmite)
		m_pSmite = new Smite(m_pMenu, smiteSlot, m_pPlayer, m_pInputManager);

	auto igniteSlot = GetIgnite();
	m_bHasIgnite = (igniteSlot != kSlotUnknown);
	if (m_bHasIgnite)
		m_pIgnite = new Ignite(m_pMenu, igniteSlot, m_pPlayer);

	m_pCleanse = new Cleanse(m_pMenu, m_pPlayer);
	m_pAutoBuy = new AutoBuy(m_pMenu, m_pPlayer);
	m_pAutoLevelUp = new AutoLevelUp(m_pMenu, m_pPlayer);

	auto allies = GEntityList->GetAllHeros(true, false);
	m_bThresh = std::count_if(allies.begin(), allies.end(), [](IUnit* ally)
	{
		return !strcmp(ally->ChampionName(), "Thresh");
	}) > 0;

	if (m_bThresh)
		m_pLantern = new Lantern(m_pMenu, m_pPlayer, m_pInputManager);
}

Activator::~Activator()
{
	if (m_pSmite != nullptr)
		_delete(m_pSmite);

	if (m_pIgnite != nullptr)
		_delete(m_pIgnite);

	if (m_pLantern != nullptr)
		_delete(m_pLantern);

	_delete(m_pCleanse);
	_delete(m_pAutoBuy);
	_delete(m_pAutoLevelUp);

	m_pMenu->Remove();
}

auto Activator::OnRender() -> void
{
	if (m_bHasSmite)
		m_pSmite->OnRender();

	if (m_bThresh)
		m_pLantern->OnRender();
}

auto Activator::OnUpdate() -> void
{
	if (m_bHasSmite)
		m_pSmite->OnUpdate();

	if (m_bHasIgnite)
		m_pIgnite->OnUpdate();

	if (m_bThresh)
		m_pLantern->OnUpdate();

	m_pCleanse->OnUpdate();
	m_pAutoBuy->OnUpdate();
}

auto Activator::OnBuffAdd(IUnit* source, void* data) -> void
{
	m_pCleanse->OnBuffAdd(source, data);
}

auto Activator::OnLevelUp(IUnit* source, int level) -> void
{
	m_pAutoLevelUp->OnLevelUp(source, level);
}

auto Activator::OnCreateObject(IUnit* object) -> void
{
	if (m_bThresh)
		m_pLantern->OnCreateObject(object);
}

auto Activator::OnDestroyObject(IUnit* object) -> void
{
	if (m_bThresh)
		m_pLantern->OnDestroyObject(object);
}

auto Activator::GetSmite() -> eSpellSlot
{
	if (strstr(m_pPlayer->GetSpellName(kSlotSummoner1), "SummonerSmite"))
		return kSlotSummoner1;
	else if (strstr(m_pPlayer->GetSpellName(kSlotSummoner2), "SummonerSmite"))
		return kSlotSummoner2;

	return kSlotUnknown;
}

auto Activator::GetIgnite() -> eSpellSlot
{
	return m_pPlayer->GetSpellSlot("SummonerDot");
}
