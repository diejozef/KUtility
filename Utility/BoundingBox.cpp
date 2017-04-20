#include "BoundingBox.hpp"

BoundingBox::BoundingBox(IMenu* parentMenu)
{
	m_pMenu = parentMenu->AddMenu("Bounding Box");
	m_pEnable = m_pMenu->CheckBox("Enable", false);
}

BoundingBox::~BoundingBox()
{
	m_pMenu->Remove();
}

auto BoundingBox::OnRenderEnemy(IUnit* hero) -> void
{
	if (!m_pEnable->Enabled() || hero->IsDead() || !hero->IsVisible())
		return;

	auto min = Vec3(0.f, 0.f, 0.f);
	auto max = Vec3(0.f, 0.f, 0.f);
	auto center = Vec3(0.f, 0.f, 0.f);

	hero->GetBounds(&min, &max);
	hero->GetHitboxCenter(&center);

	// scaling
	auto diameter = hero->BoundingRadius() * 2.0f;
	auto scale = diameter / (Vec3(min.x, min.y, min.z) - Vec3(max.x, min.y, min.z)).Length();

	auto minDistance = (min - center).Length();
	auto minDir = (min - center).VectorNormalize();
	min = center + minDir * (minDistance * scale);

	auto maxDistance = (max - center).Length();
	auto maxDir = (max - center).VectorNormalize();
	max = center + maxDir * (maxDistance * scale);

	min.y = hero->ServerPosition().y;

	std::array<Vec3, 10> points{
		// base
		Vec3(min.x, min.y, min.z), // left-bottom-front 0
		Vec3(min.x, min.y, max.z), // left-bottom-back 1
		Vec3(max.x, min.y, max.z), // right-bottom-back 2
		Vec3(max.x, min.y, min.z), // right-bottom-front 3

		// edge
		Vec3(min.x, min.y, min.z), // left-bottom-front 4
		Vec3(min.x, max.y, min.z), // left-upper-front 5

		// top
		Vec3(min.x, max.y, max.z), // left-upper-back 6
		Vec3(max.x, max.y, max.z), // right-upper-back 7
		Vec3(max.x, max.y, min.z), // right-upper-front 8
		Vec3(min.x, max.y, min.z), // left-upper-front 9
	};

	for (auto it = points.begin(); it != points.end(); it++)
	{
		if (std::next(it) != points.end())
		{
			std::array<Vec3, 2> w2s;
			GGame->Projection(*it, &w2s[0]);
			GGame->Projection(*std::next(it), &w2s[1]);

			if (w2s[0].z < 1.f && w2s[1].z < 1.f)
				GRender->DrawLine(w2s[0].To2D(), w2s[1].To2D(), Color::Yellow().Get());
		}
	}

	std::array<std::pair<Vec3, Vec3>, 3> edges{
		std::make_pair(points[1], points[6]), // left-bottom-back -> left-upper-back
		std::make_pair(points[2], points[7]), // right-bottom-back -> right-upper-back
		std::make_pair(points[3], points[8])	// right-bottom-front -> right-upper-front
	};

	for (auto edge : edges)
	{
		std::array<Vec3, 2> w2s;
		GGame->Projection(std::get<0>(edge), &w2s[0]);
		GGame->Projection(std::get<1>(edge), &w2s[1]);

		if (w2s[0].z < 1.f && w2s[1].z < 1.f)
			GRender->DrawLine(w2s[0].To2D(), w2s[1].To2D(), Color::Yellow().Get());
	}
}
