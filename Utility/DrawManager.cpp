#include "DrawManager.hpp"

DrawManager::DrawManager(IMenu* parentMenu, IUnit* player, InputManager* inputManager) :
	m_pPlayer(player),
	m_pInputManager(inputManager)
{
	m_pMenu = parentMenu->AddMenu("ku_drawings");

	m_vecFowTrackers.reserve(5);

	auto enemies = GEntityList->GetAllHeros(false, true);
	std::for_each(enemies.begin(), enemies.end(), [&](IUnit* hero)
	{
		m_vecFowTrackers.emplace_back(FowTracker(hero));

		char fileName[64]{ '\0' };
		sprintf_s(fileName, "KUtility\\%s.png", hero->ChampionName());
		std::string championName(hero->ChampionName());

		if (m_mapTextures.find(championName) == m_mapTextures.end())
		{
			if (GUtility->DoesFileExist(std::string("Textures\\" + std::string(fileName)).c_str()))
			{
				m_mapTextures[championName] = GRender->CreateTextureFromFile(fileName);
			}
			else
			{
				char text[64]{ '\0' };
				sprintf_s(text, "[KUtility/DM/1] Missing texture: %s", fileName);
				GGame->PrintChat(text);
			}
		}

		auto summonerSpellName = std::string("");

		for (auto i = static_cast<int>(kSlotSummoner1); i <= static_cast<int>(kSlotSummoner2); i++)
		{
			summonerSpellName = std::string(hero->GetSpellName(i));
			sprintf_s(fileName, "KUtility\\%s.png", summonerSpellName.c_str());

			if (m_mapTextures.find(summonerSpellName) == m_mapTextures.end())
			{
				if (GUtility->DoesFileExist(std::string("Textures\\" + std::string(fileName)).c_str()))
				{
					auto texture = GRender->CreateTextureFromFile(fileName);
					texture->Resize(14, 15);

					m_mapTextures[summonerSpellName] = texture;
				}
				else
				{
					char text[64]{ '\0' };
					sprintf_s(text, "[KUtility/DM/2] Missing texture: %s", fileName);
					GGame->PrintChat(text);
				}
			}
		}
	});

	std::array<std::pair<std::string, std::string>, 6> extraTextures{
		std::make_pair("S5_SummonerSmiteDuel", "KUtility\\S5_SummonerSmiteDuel.png"),
		std::make_pair("S5_SummonerSmitePlayerGanker", "KUtility\\S5_SummonerSmitePlayerGanker.png"),
		std::make_pair("KUtil_LPTMinimapCircle", "KUtility\\KUtil_LPTMinimapCircle.png"),
		std::make_pair("KUtil_LPTWorldCircle", "KUtility\\KUtil_LPTWorldCircle.png"),
		std::make_pair("KUtil_Circle", "KUtility\\KUtil_Circle.png"),
		std::make_pair("KUtil_DarkFilter", "KUtility\\KUtil_DarkFilter.png")
	};

	for (const auto& textureName : extraTextures)
	{
		if (m_mapTextures.find(std::get<0>(textureName)) != m_mapTextures.end())
			continue;

		auto fileName = std::get<1>(textureName).c_str();

		if (GUtility->DoesFileExist(std::string("Textures\\" + std::string(fileName)).c_str()))
		{
			auto texture = GRender->CreateTextureFromFile(fileName);
			auto name = std::get<0>(textureName);
			if (name.find("SummonerSmite") != std::string::npos)
				texture->Resize(14, 15);

			m_mapTextures[name] = texture;
		}
		else
		{
			char text[64]{ '\0' };
			sprintf_s(text, "[KUtility/DM/3] Missing texture: %s", fileName);
			GGame->PrintChat(text);
		}
	}

	m_pCooldownManager = new CooldownManager(m_pPlayer);
	m_pSpellInfoBox = new SpellInfoBox(m_pMenu, &m_vecFowTrackers, &m_mapTextures, m_pInputManager, m_pCooldownManager);
	m_pLastPositions = new LastPositions(m_pMenu, &m_vecFowTrackers, &m_mapTextures);
	m_pSpellTracker = new SpellTracker(m_pMenu, m_pPlayer, m_pCooldownManager);
	m_pPassives = new Passives(m_pMenu);
	m_pBoundingBox = new BoundingBox(m_pMenu);
	m_pTurretRanges = new TurretRanges(m_pMenu, m_pPlayer);
	m_pClicks = new Clicks(m_pMenu);
	m_pClones = new Clones(m_pMenu);
	m_pAARange = new AARange(m_pMenu, m_pPlayer);
	m_pExtendedAwareness = new ExtendedAwareness(m_pMenu, m_pPlayer, &m_vecFowTrackers, &m_mapTextures);
	m_pBuildings = new Buildings(m_pMenu);
	m_pRecallManager = new RecallManager(parentMenu, m_pPlayer, m_pInputManager, &m_vecFowTrackers);
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
	_delete(m_pBuildings);
	_delete(m_pRecallManager);
	_delete(m_pCooldownManager);

	m_pMenu->Remove();
}

auto DrawManager::OnRender() -> void
{
	auto texture = m_mapTextures["Zilean"];
	if (texture != nullptr)
	{
		m_mapTextures["Zilean"]->Draw(6, 6);
	}

	for (auto hero : GEntityList->GetAllHeros(true, true))
	{
		if (hero->IsEnemy(m_pPlayer))
		{
			m_pPassives->OnRenderEnemy(hero);
			m_pBoundingBox->OnRenderEnemy(hero);
			m_pLastPositions->OnRenderEnemy(hero);
			m_pClicks->OnRenderEnemy(hero);
			m_pClones->OnRenderEnemy(hero);
			m_pExtendedAwareness->OnRenderEnemy(hero);
		}

		m_pSpellTracker->OnRenderHero(hero);
	}

	m_pSpellInfoBox->OnRender();
	m_pTurretRanges->OnRender();
	m_pAARange->OnRender();
	m_pRecallManager->OnRender();
	m_pExtendedAwareness->OnRender();
	m_pBuildings->OnRender();
}

auto DrawManager::OnUpdate() -> void
{
	m_pSpellInfoBox->OnUpdate();
	m_pRecallManager->OnUpdate();
	m_pBuildings->OnUpdate();
}

auto DrawManager::OnEnterFow(IUnit* unit) -> void
{
	if (!unit->IsHero() || !unit->IsEnemy(m_pPlayer))
		return;

	std::find_if(m_vecFowTrackers.begin(), m_vecFowTrackers.end(), [&](const FowTracker& t)
	{
		return t.Unit()->GetNetworkId() == unit->GetNetworkId();
	})->OnEnterFow(GGame->Time());
}

auto DrawManager::OnExitFow(IUnit* unit) -> void
{
	if (!unit->IsHero() || !unit->IsEnemy(m_pPlayer))
		return;

	std::find_if(m_vecFowTrackers.begin(), m_vecFowTrackers.end(), [&](const FowTracker& t)
	{
		return t.Unit()->GetNetworkId() == unit->GetNetworkId();
	})->OnExitFow();
}

auto DrawManager::OnUnitDeath(IUnit* unit) -> void
{
	if (!unit->IsHero() || !unit->IsEnemy(m_pPlayer))
		return;

	std::find_if(m_vecFowTrackers.begin(), m_vecFowTrackers.end(), [&](const FowTracker& t)
	{
		return t.Unit()->GetNetworkId() == unit->GetNetworkId();
	})->OnResetPos();
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
