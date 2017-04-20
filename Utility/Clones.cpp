#include "Clones.hpp"

Clones::Clones(IMenu* parentMenu)
{
	m_pMenu = parentMenu->AddMenu("Clones");
	m_pEnable = m_pMenu->CheckBox("Enable", false);
}

Clones::~Clones()
{
	m_pMenu->Remove();
}

auto Clones::OnRenderEnemy(IUnit* hero) -> void
{
	if (!m_pEnable->Enabled() || hero->IsDead() || !hero->IsVisible())
		return;

	auto clone = hero->GetClone();

	if (clone == nullptr)
		return;

	GRender->DrawOutlinedCircle(hero->GetPosition(), Color::Crimson().Get(), hero->BoundingRadius());
}
