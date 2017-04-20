#include "DrawManager.hpp"

DrawManager::DrawManager(IMenu* parentMenu, IUnit* player, InputManager* inputManager) :
	m_pPlayer(player),
	m_pInputManager(inputManager)
{
	m_pMenu = parentMenu->AddMenu("Drawings");

	auto enemies = GEntityList->GetAllHeros(false, true);
	std::for_each(enemies.begin(), enemies.end(), [&](IUnit* hero)
	{
		m_mapFowTrackers.emplace(std::make_pair(hero->GetNetworkId(), FowTracker(hero)));

		char fileName[64]{ '\0' };
		sprintf_s(fileName, "%s.png", hero->ChampionName());
		std::string championName(hero->ChampionName());

		if (m_mapTextures.find(championName) == m_mapTextures.end())
		{
			auto texture = GRender->CreateTextureFromFile(fileName);
			auto size = texture->GetSize();

			if (size.y < 1.0f || size.x < 1.0f)
			{
				char text[64];
				sprintf_s(text, "[KUtility/DM] Missing texture: %s", fileName);
				GGame->PrintChat(text);
			}
			else
				m_mapTextures[championName] = texture;
		}

		auto summonerSpellName = std::string("");

		for (auto i = static_cast<int>(kSlotSummoner1); i <= static_cast<int>(kSlotSummoner2); i++)
		{
			summonerSpellName = std::string(hero->GetSpellName(i));
			sprintf_s(fileName, "%s.png", summonerSpellName.c_str());

			if (m_mapTextures.find(summonerSpellName) == m_mapTextures.end())
			{
				auto texture = GRender->CreateTextureFromFile(fileName);
				auto size = texture->GetSize();

				if (size.y < 1.0f || size.x < 1.0f)
				{
					char text[64];
					sprintf_s(text, "[KUtility/DM] Missing texture: %s", fileName);
					GGame->PrintChat(text);
				}
				else
				{
					texture->Resize(14, 15);
					m_mapTextures[summonerSpellName] = texture;
				}
			}
		}
	});

	std::array<std::pair<std::string, std::string>, 6> extraTextures{
		std::make_pair("S5_SummonerSmiteDuel", "S5_SummonerSmiteDuel.png"),
		std::make_pair("S5_SummonerSmitePlayerGanker", "S5_SummonerSmitePlayerGanker.png"),
		std::make_pair("KUtil_LPTMinimapCircle", "KUtil_LPTMinimapCircle.png"),
		std::make_pair("KUtil_LPTWorldCircle", "KUtil_LPTWorldCircle.png"),
		std::make_pair("KUtil_Circle", "KUtil_Circle.png"),
		std::make_pair("KUtil_DarkFilter", "KUtil_DarkFilter.png")
	};

	for (const auto& textureName : extraTextures)
	{
		if (m_mapTextures.find(std::get<0>(textureName)) != m_mapTextures.end())
			continue;

		auto texture = GRender->CreateTextureFromFile(std::get<1>(textureName).c_str());
		auto size = texture->GetSize();

		if (size.y < 1.0f || size.x < 1.0f)
		{
			char text[64];
			sprintf_s(text, "[KUtility/DM] Missing texture: %s", std::get<1>(textureName).c_str());
			GGame->PrintChat(text);
		}
		else
		{
			auto name = std::get<0>(textureName);
			if (name.find("SummonerSmite") != std::string::npos)
				texture->Resize(14, 15);

			m_mapTextures[name] = texture;
		}
	}

	m_pCooldownManager = new CooldownManager(m_pPlayer);
	m_pSpellInfoBox = new SpellInfoBox(m_pMenu, &m_mapFowTrackers, &m_mapTextures, m_pInputManager, m_pCooldownManager);
	m_pLastPositions = new LastPositions(m_pMenu, &m_mapFowTrackers, &m_mapTextures);
	m_pSpellTracker = new SpellTracker(m_pMenu, m_pPlayer, m_pCooldownManager);
	m_pPassives = new Passives(m_pMenu);
	m_pBoundingBox = new BoundingBox(m_pMenu);
	m_pTurretRanges = new TurretRanges(m_pMenu, m_pPlayer);
	m_pClicks = new Clicks(m_pMenu);
	m_pClones = new Clones(m_pMenu);
	m_pAARange = new AARange(m_pMenu, m_pPlayer);
	m_pExtendedAwareness = new ExtendedAwareness(m_pMenu, m_pPlayer, &m_mapFowTrackers, &m_mapTextures);
	m_pRecallManager = new RecallManager(parentMenu, m_pPlayer, m_pInputManager, &m_mapFowTrackers);
}

DrawManager::~DrawManager()
{
	_delete(m_pSpellInfoBox);
	_delete(m_pLastPositions);
	_delete(m_pSpellTracker);
	_delete(m_pPassives);
	_delete(m_pBoundingBox);
	_delete(m_pTurretRanges);
	_delete(m_pClicks);
	_delete(m_pClones);
	_delete(m_pAARange);
	_delete(m_pExtendedAwareness);
	_delete(m_pRecallManager);
	_delete(m_pCooldownManager);

	m_pMenu->Remove();
}

auto DrawManager::OnRender() -> void
{
	for (auto hero : GEntityList->GetAllHeros(true, true))
	{
		if (hero == nullptr)
			continue;

		if (hero->IsEnemy(m_pPlayer))
		{
			m_pPassives->OnRenderEnemy(hero);
			m_pLastPositions->OnRenderEnemy(hero);
			m_pBoundingBox->OnRenderEnemy(hero);
			m_pClicks->OnRenderEnemy(hero);
			m_pClones->OnRenderEnemy(hero);
			m_pExtendedAwareness->OnRenderEnemy(hero);
		}

		m_pSpellTracker->OnRenderHero(hero);
	}

	m_pTurretRanges->OnRender();
	m_pSpellInfoBox->OnRender();
	m_pAARange->OnRender();
	m_pRecallManager->OnRender();
	m_pExtendedAwareness->OnRender();
}

auto DrawManager::OnUpdate() -> void
{
	m_pSpellInfoBox->OnUpdate();
	m_pRecallManager->OnUpdate();
}

auto DrawManager::OnEnterFow(IUnit* unit) -> void
{
	if (!unit->IsHero() || !unit->IsEnemy(m_pPlayer))
		return;

	m_mapFowTrackers[unit->GetNetworkId()].OnEnterFow(GGame->Time());
}

auto DrawManager::OnExitFow(IUnit* unit) -> void
{
	if (!unit->IsHero() || !unit->IsEnemy(m_pPlayer))
		return;

	m_mapFowTrackers[unit->GetNetworkId()].OnExitFow();
}

auto DrawManager::OnUnitDeath(IUnit* unit) -> void
{
	if (!unit->IsHero() || !unit->IsEnemy(m_pPlayer))
		return;

	m_mapFowTrackers[unit->GetNetworkId()].OnResetPos();
}

auto DrawManager::OnCreateObject(IUnit* object) -> void
{
	
}

auto DrawManager::OnDestroyObject(IUnit* object) -> void
{

}

auto DrawManager::OnTeleport(OnTeleportArgs* data) -> void
{
	m_pRecallManager->OnTeleport(data);
	m_pCooldownManager->OnTeleport(data);
}