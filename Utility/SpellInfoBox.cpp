#include "SpellInfoBox.hpp"

SpellInfoBox::SpellInfoBox(IMenu* parentMenu,
						   std::unordered_map<int, FowTracker>* trackers,
						   std::unordered_map<std::string, ITexture*>* textures,
						   InputManager* inputManager,
						   CooldownManager* cooldownManager) :
	m_iWidth(325),
	m_iLineHeight(18),
	m_pFowTrackers(trackers),
	m_pTextures(textures),
	m_pInputManager(inputManager),
	m_pCooldownManager(cooldownManager)
{
	m_pMenu = parentMenu->AddMenu("Info Box");
	m_pDraw = m_pMenu->CheckBox("Enable", false);

	m_vecScreenSize = GRender->ScreenSize();

	m_pPosX = m_pMenu->AddInteger("X", 0, m_vecScreenSize.x, 0);
	m_pPosY = m_pMenu->AddInteger("Y", 0, m_vecScreenSize.y, 0);

	auto enemies = GEntityList->GetAllHeros(false, true);
	m_iHeight = m_iLineHeight * enemies.size();

	std::for_each(enemies.begin(), enemies.end(), [&](IUnit* hero)
	{
		std::array<const char*, 2> summoners{ hero->GetSpellName(kSlotSummoner1), hero->GetSpellName(kSlotSummoner2) };
		std::sort(summoners.begin(), summoners.end(), [](const char* name1, const char* name2)
		{
			return !strcmp(name1, "SummonerFlash") > !strcmp(name2, "SummonerFlash");
		});

		m_mapSummoners[hero->GetNetworkId()] = summoners;
	});
}

SpellInfoBox::~SpellInfoBox()
{
	m_pMenu->Remove();
}

auto SpellInfoBox::OnUpdate() -> void
{
	if (!m_pDraw->Enabled())
		return;

	HandleDrag();
}

auto SpellInfoBox::OnRender() -> void
{
	if (!m_pDraw->Enabled())
		return;

	auto xPos = m_pPosX->GetInteger();
	auto yPos = m_pPosY->GetInteger();
	static auto lineTextOffset = 1;

	auto line = 0U;

	for (auto hero : GEntityList->GetAllHeros(false, true))
	{
		if (hero == nullptr)
			continue;

		auto fowTracker = (*m_pFowTrackers)[hero->GetNetworkId()];

		// Background
		auto clr = Color(0, 0, 0);
		clr = hero->HealthPercent() > 50.0f ? Color::LightBlue2() : hero->HealthPercent() > 20.0f ? Color::Orange2() : Color::LightRed();

		GRender->DrawFilledBox(Vec2(xPos, (yPos + line * m_iLineHeight) - 2),
							   Vec2(m_iWidth, m_iLineHeight),
							   Color::Black().Get(130));

		GRender->DrawFilledBox(Vec2(xPos + 1, (yPos + 1 + line * m_iLineHeight) - 2),
							   Vec2(((m_iWidth - 2) * (hero->HealthPercent() * 0.01f)), m_iLineHeight - 2),
							   clr.Get(50));

		// 1st column
		clr = hero->IsDead() ? Color::DarkGrey() : !hero->IsVisible() ? Color::Yellow() : Color::White();

		char text[64]{ '\0' };

		if (!fowTracker.InFow() || hero->IsVisible())
			KDrawing::DrawString(Vec2(xPos + 5, (yPos + line * m_iLineHeight - lineTextOffset)),
								 clr, true, "%s", hero->ChampionName());
		else
			KDrawing::DrawString(Vec2(xPos + 5, (yPos + line * m_iLineHeight - lineTextOffset)),
								 clr, true, "%s(%.0f)", hero->ChampionName(), (GGame->Time() - fowTracker.Time()));

		// 2nd column
		strcpy_s(text, "[R RDY]");

		auto ready = hero->GetSpellRemainingCooldown(kSlotR) < 1.f;

		if (!ready)
			sprintf_s(text, "[R %.0f]", hero->GetSpellRemainingCooldown(kSlotR));

		clr = ready ? Color::LightGreen() : Color::Yellow();

		if (hero->GetSpellLevel(kSlotR) < 1)
		{
			strcpy_s(text, "[R]");
			clr = Color::LightRed();
		}

		KDrawing::DrawString(Vec2(xPos + 110, yPos + line * m_iLineHeight - lineTextOffset), clr, true, text);

		// 3rd and 4th column
		strcpy_s(text, "[RDY]");
		auto column = 0U;
		auto summoners = m_mapSummoners[hero->GetNetworkId()];

		for (auto summoner : summoners)
		{
			auto slot = hero->GetSpellSlot(summoner);

			auto texture = (*m_pTextures)[std::string(summoner)];
			if (texture != nullptr)
				texture->Draw(xPos + 180 + (column * 80), yPos + line * m_iLineHeight);

			auto rcd = hero->GetSpellRemainingCooldown(slot);
			auto rcd2 = m_pCooldownManager->GetRemainingCooldown(hero->GetNetworkId(), slot);
			if (rcd2 > 0.0f)
				rcd = rcd2;

			ready = rcd < 1.f;
			if (!ready)
				sprintf_s(text, "[%.0f]", rcd);
			else
				strcpy_s(text, "[RDY]");

			clr = ready ? Color::LightGreen() : Color::Yellow();

			KDrawing::DrawString(Vec2(xPos + 197 + (column * 80), yPos + line * m_iLineHeight - lineTextOffset), clr, true, text);
			column++;
		}

		line++;
	}
}

auto SpellInfoBox::HandleDrag() const -> void
{
	if (!m_pDraw->Enabled())
		return;

	static auto isDragging = false;

	POINT cursor;
	GetCursorPos(&cursor);

	auto x = m_pPosX->GetInteger();
	auto y = m_pPosY->GetInteger();

	if (isDragging
		|| cursor.x > x && cursor.x < x + m_iWidth
		&& cursor.y > y && cursor.y < y + m_iHeight)
	{
		static std::array<int, 2> delta = { -1, -1 };

		if (delta[0] == -1 && delta[1] == -1)
		{
			delta[0] = cursor.x - x;
			delta[1] = cursor.y - y;
		}

		if (m_pInputManager->IsMouseButtonPressed(kLButton))
		{
			isDragging = true;

			auto newX = cursor.x - delta[0];
			auto newY = cursor.y - delta[1];

			m_pPosX->UpdateInteger(newX);
			m_pPosY->UpdateInteger(newY);

			x = newX;
			y = newY;
		}
		else
		{
			isDragging = false;

			delta[0] = -1;
			delta[1] = -1;
		}
	}

	if (x < 0)
		m_pPosX->UpdateInteger(0);

	if (y < 0)
		m_pPosY->UpdateInteger(0);

	if (x > m_vecScreenSize.x - m_iWidth)
		m_pPosX->UpdateInteger(m_vecScreenSize.x - m_iWidth);

	if (y > m_vecScreenSize.y - m_iHeight)
		m_pPosY->UpdateInteger(m_vecScreenSize.y - m_iHeight);
}
