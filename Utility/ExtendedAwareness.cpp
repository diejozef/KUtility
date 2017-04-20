#include "ExtendedAwareness.hpp"

ExtendedAwareness::ExtendedAwareness(IMenu* parentMenu, IUnit* player,
									 std::unordered_map<int, FowTracker>* trackers,
									 std::unordered_map<std::string, ITexture*>* textures) :
	m_pPlayer(player),
	m_pFowTrackers(trackers),
	m_pTextures(textures)
{
	m_pMenu = parentMenu->AddMenu("Extended Awareness");
	m_pEnable = m_pMenu->CheckBox("Enable", false);
	m_pUseIcons = m_pMenu->CheckBox("Use Icons", false);
	m_pDrawIndicatorRange = m_pMenu->CheckBox("Draw Indicator Range", false);
	m_pDrawLines = m_pMenu->CheckBox("Draw Lines", false);
	m_pMaxDistance = m_pMenu->AddInteger("Max. Distance", 550, 20000, 6000);
	m_pMinDistance = m_pMenu->AddInteger("Min. Distance", 0, 5000, 2000);
	m_pIndicatorDistance = m_pMenu->AddInteger("Indicator Distance", 100, 2000, 1000);
	m_pWarningDistance = m_pMenu->AddInteger("Warning Distance", 550, 4000, 2000);
	m_pIconMaxSize = m_pMenu->AddInteger("Max. Size of Icons", 25, 100, 30);
	m_pIconMinSize = m_pMenu->AddInteger("Min. Size of Icons", 5, 50, 15);
}

ExtendedAwareness::~ExtendedAwareness()
{
	m_pMenu->Remove();
}

auto ExtendedAwareness::OnRenderEnemy(IUnit* hero) -> void
{
	if (!m_pEnable->Enabled() || hero->IsDead() || hero->IsOnScreen())
		return;

	auto fowTracker = (*m_pFowTrackers)[hero->GetNetworkId()];
	auto distanceSqr = (hero->GetPosition() - m_pPlayer->GetPosition()).Length2DSqr();
	auto timeSinceLastSeen = GGame->Time() - fowTracker.Time();

	if (distanceSqr < _sqr(m_pMaxDistance->GetInteger())
		&& distanceSqr > _sqr(m_pMinDistance->GetInteger())
		&& (hero->IsVisible() || timeSinceLastSeen < 5.0f))
	{
		auto heroPos = hero->GetPosition();
		heroPos.y = 100.0f;

		auto playerPos = m_pPlayer->GetPosition();
		playerPos.y = 100.0f;

		if (m_pDrawLines->Enabled())
		{
			auto playerPosScreen = Vec3(0.f, 0.f, 0.f);
			GGame->Projection(m_pPlayer->GetPosition(), &playerPosScreen);

			auto heroPosScreen = Vec3(0.f, 0.f, 0.f);
			GGame->Projection(hero->GetPosition(), &heroPosScreen);

			if (fowTracker.InFow())
				GGame->Projection(fowTracker.LastPos(), &heroPosScreen);

			if (playerPosScreen.z < 1.0f && heroPosScreen.z < 1.0f)
			{
				GRender->DrawLine(playerPosScreen.To2D(), heroPosScreen.To2D(),
								  fowTracker.InFow() ? Color::LightGrey().Get(120) : Color::White().Get(120));
			}
		}

		auto dir = (hero->GetPosition() - m_pPlayer->GetPosition()).VectorNormalize();
		auto drawPos3d = playerPos + dir.Multiply2D(static_cast<float>(m_pIndicatorDistance->GetInteger()));
		auto drawPos = Vec3(0.f, 0.f, 0.f);
		auto isWarning = distanceSqr < _sqr(static_cast<float>(m_pWarningDistance->GetInteger()));

		if (GGame->Projection(drawPos3d, &drawPos) && drawPos.z < 1.0f)
		{
			if (!m_pUseIcons->Enabled())
			{
				if (!hero->IsVisible())
				{
					KDrawing::DrawString(drawPos.To2D(), Color::Grey(), true, "%s(%.0f) (%.0f%%)",
										 hero->ChampionName(), timeSinceLastSeen, hero->HealthPercent());
				}
				else
				{
					auto color = isWarning ? Color::Orange2() : Color::White();

					KDrawing::DrawString(drawPos.To2D(), color, true, "%s (%.0f%%)",
										 hero->ChampionName(), hero->HealthPercent());
				}
			}
			else
			{
				auto pct = 1.0f - distanceSqr / _sqr(m_pMaxDistance->GetInteger());
				auto texture = (*m_pTextures)[std::string(hero->ChampionName())];
				auto border = (*m_pTextures)["KUtil_Circle"];
				auto r = m_pIconMaxSize->GetInteger() * pct;

				r = std::max(r, static_cast<float>(m_pIconMinSize->GetInteger()));

				if (texture != nullptr && border != nullptr)
				{
					texture->DrawCircle(drawPos.x, drawPos.y, r);

					auto color = fowTracker.InFow() ? Color::LightGrey() :
						hero->HealthPercent() < 30.0f ? Color::LightRed() : Color::LightGreen();

					border->SetColor(color.Get());
					border->SetScaleBySize(Vec2(r * 2 + 1, r * 2 + 1));
					border->Draw(drawPos.x - r, drawPos.y - r);

					color = isWarning ? Color::Orange2() : Color::White();

					if (hero->IsVisible())
					{
						KDrawing::DrawString(Vec2(drawPos.x + r + 5.0f, drawPos.y), color, true, "(%.0f%%)",
											 hero->HealthPercent());
					}
					else
					{
						KDrawing::DrawString(Vec2(drawPos.x + r + 5.0f, drawPos.y), color, true, "(%.0f) (%.0f%%)",
											 timeSinceLastSeen, hero->HealthPercent());
					}
				}
			}
		}
	}
}

auto ExtendedAwareness::OnRender() -> void
{
	if (!m_pEnable->Enabled() || !m_pDrawIndicatorRange->Enabled())
		return;

	GRender->DrawCircle(m_pPlayer->GetPosition(), static_cast<float>(m_pIndicatorDistance->GetInteger()), Color::White().Get(80), 1.0f);
}
