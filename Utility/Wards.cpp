#include "Wards.hpp"

Ward::Ward() :
	m_bValid(false)
{

}

Ward::~Ward()
{

}

Ward::Ward(const Vec3& pos, const WardDefinition& info, float duration) :
	m_vecPos(pos),
	m_info(info),
	m_bValid(true),
	m_iNetworkId(-1)
{
	m_flEndTime = GGame->Time() + duration;
	m_bDrawTimer = (info.m_type == kSightWard || info.m_type == kTrap || info.m_type == kCaitlynTrap);

	switch (info.m_type)
	{
	case kSightWard:
		m_color = Color::Yellow();
		break;
	case kTrap:
		m_color = Color::DarkGreen();
		break;
	case kPinkWard:
		m_color = Color::Magenta();
		break;
	case kBlueWard:
		m_color = Color::LightBlue2();
		break;
	case kCaitlynTrap:
		m_color = Color::White();
		break;
	}
}

Ward::Ward(const Vec3& pos, const WardDefinition& info, float duration, int networkId) :
	m_vecPos(pos),
	m_info(info),
	m_bValid(true),
	m_iNetworkId(networkId)
{
	m_flEndTime = GGame->Time() + duration;
	m_bDrawTimer = (info.m_type == kSightWard || info.m_type == kTrap || info.m_type == kCaitlynTrap);

	switch (info.m_type)
	{
	case kSightWard:
		m_color = Color::Yellow();
		break;
	case kTrap:
		m_color = Color::LightGreen();
		break;
	case kPinkWard:
		m_color = Color::Magenta();
		break;
	case kBlueWard:
		m_color = Color::LightBlue2();
		break;
	case kCaitlynTrap:
		m_color = Color::White();
		break;
	}
}

auto Ward::Valid() const -> bool
{
	return (m_bValid && m_flEndTime >= GGame->Time());
}

auto Ward::Invalidate() -> void
{
	m_bValid = false;
	m_flEndTime = GGame->Time() - 1.0f;
}

auto Ward::Draw(bool drawRange) const -> void
{
	auto radius = drawRange ? m_info.m_flRange : 50.0f;
	auto alpha = static_cast<uint8_t>(drawRange ? 100 : 255);

	GRender->DrawCircle(m_vecPos, radius, m_color.Get(alpha), 5.0f);

	if (!m_bDrawTimer)
		return;

	auto w2s = Vec3(0.f, 0.f, 0.f);
	if (GGame->Projection(m_vecPos, &w2s) && w2s.z < 1.f)
	{
		auto remainingTime = m_flEndTime - GGame->Time();
		KDrawing::DrawString(Vec2(w2s.x - 3.0f, w2s.y), m_color, true, "%.0f s", remainingTime);
	}
}

auto Ward::SetNetworkId(int networkId) -> void
{
	m_iNetworkId = networkId;
}

auto Ward::SetEndTime(float time) -> void
{
	m_flEndTime = time;
}
