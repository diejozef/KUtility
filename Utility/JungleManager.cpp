#include "JungleManager.hpp"

JungleManager::JungleManager(IMenu* parentMenu)
{
	m_pMenu = parentMenu->AddMenu("Jungle Tracker");
	m_pFowTracker = m_pMenu->CheckBox("FoW Tracker", false);
	m_pUsePings = m_pMenu->CheckBox("Use Pings", false);

	m_flNextResetCheckTime = GGame->Time() + 5.0f;

	m_vecJungleTrackers = { //rip
		JungleTracker(Vec3(7878.0f, 52.0f, 9500.0f)), // redside raptors
		JungleTracker(Vec3(5960.0f, 52.0f, 11160.0f)), // redside satchel 1
		JungleTracker(Vec3(5990.0f, 52.0f, 10150.0f)), // redside vision plant 1
		JungleTracker(Vec3(8645.0f, 52.0f, 11575.0f)), // redside satchel 2
		JungleTracker(Vec3(10365.0f, 52.0f, 7666.0f)), // redside satchel 3
		JungleTracker(Vec3(11360.0f, 52.0f, 6030.0f)), // redside vision plant 2
		JungleTracker(Vec3(11860.0f, 52.0f, 5195.0f)), // redside satchel 4

		JungleTracker(Vec3(6978.0f, 52.0f, 5416.0f)), // blueside raptors
		JungleTracker(Vec3(6140.0f, 52.0f, 3430.0f)), // blueside satchel 1
		JungleTracker(Vec3(8815.0f, 52.0f, 3670.0f)), // blueside satchel 2
		JungleTracker(Vec3(8850.0f, 52.0f, 4730.0f)), // blueside vision plant 1
		JungleTracker(Vec3(4385.0f, 52.0f, 7275.0f)), // blueside satchel 3
		JungleTracker(Vec3(3400.0f, 52.0f, 8920.0f)), // blueside vision plant 2
		JungleTracker(Vec3(2860.0f, 52.0f, 9900.0f)), // blueside satchel 4

		JungleTracker(Vec3(4155.0f, 52.0f, 9451.0f)), // top river center
		JungleTracker(Vec3(10711.0f, 52.0f, 5400.0f)) // bot river center
	};
}

JungleManager::~JungleManager()
{
	m_pMenu->Remove();
}

auto JungleManager::OnRender() -> void
{
	if (!m_pFowTracker->Enabled() || GGame->Time() < 90.0f)
		return;

	for (const auto& tracker : m_vecJungleTrackers)
		tracker.OnRender();
}

auto JungleManager::OnUpdate() -> void
{
	if (!m_pFowTracker->Enabled())
		return;

	if (GGame->Time() < m_flNextResetCheckTime)
		return;

	for (auto& tracker : m_vecJungleTrackers)
		tracker.OnUpdate();

	m_flNextResetCheckTime = GGame->Time() + 5.0f;
}

auto JungleManager::OnJungleNotify(JungleNotifyData* data) -> void
{
	if (!m_pFowTracker->Enabled() || GGame->Time() < 90.0f)
		return;

	auto pos = data->Position;

	if (!GGame->WithinFogOfWar(pos))
		return;

	auto& i = std::find_if(m_vecJungleTrackers.begin(), m_vecJungleTrackers.end(), [&](const JungleTracker& tracker)
	{
		auto distance = (tracker.Pos() - pos).Length2DSqr();

		return distance < _sqr(650.0f);
	});

	if (i != m_vecJungleTrackers.end())
	{
		auto time = GGame->Time();
		auto timeSinceLastAggro = time - i->LastAggroTime();

		if (timeSinceLastAggro < 5.0f)
		{
			i->IncreaseAggroCount();

			if (time > i->NextPingTime() && i->AggroCount() >= 1)
			{
				GGame->ShowPing(kPingNormal, i->Pos(), true);

				i->SetNextPingTime(time + 20.0f);
				i->ResetAggroCount();
			}
		}

		for (auto& tracker : m_vecJungleTrackers)
			tracker.SetLastAggro(false);

		i->OnJungleNotify(GGame->Time());
	}
}
