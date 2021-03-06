#include "LastPositions.hpp"

LastPositions::LastPositions(IMenu* parentMenu,
							 std::vector<FowTracker>* trackers,
							 std::unordered_map<std::string, ITexture*>* textures) :
	m_pFowTrackers(trackers),
	m_pTextures(textures)
{
	m_pMenu = parentMenu->AddMenu("ku_last_positions");
	m_pEnable = m_pMenu->CheckBox("Enable", false);
	m_pDraw3D = m_pMenu->CheckBox("3D Drawings", false);
	m_pDrawIcons = m_pMenu->CheckBox("Draw Icons", false);
}

LastPositions::~LastPositions()
{
	m_pMenu->Remove();
}

auto LastPositions::OnRenderEnemy(IUnit* hero) -> void
{
	if (!m_pEnable->Enabled() || hero->IsDead() || hero->IsVisible())
		return;

	auto fowTracker = *std::find_if(m_pFowTrackers->begin(), m_pFowTrackers->end(), [&](const FowTracker& t)
	{
		return t.Unit()->GetNetworkId() == hero->GetNetworkId();
	});

	auto w2m = fowTracker.LastMinimapPos();
	auto champName = hero->ChampionName();
	auto timeSinceLastSeen = GGame->Time() - fowTracker.Time();

	if (!m_pDrawIcons->Enabled())
	{
		char displayName[4]{ '\0' };
		strncpy_s(displayName, champName, 3);

		KDrawing::DrawString(w2m, Color::Yellow(), true, "%s(%.0f)", displayName, timeSinceLastSeen);
	}
	else
	{
		if (m_pTextures->find(std::string(champName)) != m_pTextures->end() &&
			m_pTextures->find("KUtil_LPTMinimapCircle") != m_pTextures->end())
		{

			(*m_pTextures)[std::string(champName)]->DrawCircle(w2m.x, w2m.y, 10.0f);

			auto border = (*m_pTextures)["KUtil_LPTMinimapCircle"];
			border->SetColor(Color::LightGrey().Get());
			border->Draw(w2m.x - 10.0f, w2m.y - 10.0f);
			KDrawing::DrawString(Vec2(w2m.x + 7.0f, w2m.y), Color::Yellow(), true, "(%.0f)", timeSinceLastSeen);
		}
	}

	if (m_pDraw3D->Enabled())
	{
		auto w2s = Vec3(0.f, 0.f, 0.f);

		if (GGame->Projection(fowTracker.LastPos(), &w2s) && w2s.z < 1.0f)
		{
			if (!m_pDrawIcons->Enabled())
			{
				char displayName[4]{ '\0' };
				strncpy_s(displayName, champName, 3);

				KDrawing::DrawString(w2s.To2D(), Color::White(), true, "%s(%.0f)", displayName, timeSinceLastSeen);
			}
			else
			{
				if (m_pTextures->find(std::string(champName)) != m_pTextures->end() && 
					m_pTextures->find("KUtil_LPTWorldCircle") != m_pTextures->end())
				{
					(*m_pTextures)[std::string(champName)]->DrawCircle(w2s.x, w2s.y, 20.0f);

					auto border = (*m_pTextures)["KUtil_LPTWorldCircle"];
					border->SetColor(Color::LightGrey().Get());
					border->Draw(w2s.x - 20.0f, w2s.y - 20.0f);
					KDrawing::DrawString(Vec2(w2s.x + 24.0f, w2s.y), Color::Yellow(), true, "(%.0f)", timeSinceLastSeen);
				}
			}
		}
	}
}
