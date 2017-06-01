#include "SkinChanger.hpp"

SkinChanger::SkinChanger(IMenu* parentMenu, IUnit* player) :
	m_pPlayer(player)
{
	char text[64]{ '\0' };
	sprintf_s(text, "ku_skin_changer - %s", m_pPlayer->ChampionName());
	m_pMenu = parentMenu->AddMenu(text);

	m_pEnable = m_pMenu->CheckBox("Enable", false);
	m_pSkinId = m_pMenu->AddInteger("Skin ID", 0, 20, m_pPlayer->GetSkinId());
}

SkinChanger::~SkinChanger()
{
	m_pMenu->Remove();
}

auto SkinChanger::OnUpdate() -> void
{
	if (!m_pEnable->Enabled())
		return;

	auto selectedSkinId = m_pSkinId->GetInteger();

	if (selectedSkinId != m_pPlayer->GetSkinId())
		m_pPlayer->SetSkinId(selectedSkinId);
}
