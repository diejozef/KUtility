#pragma once
#include <PluginSDK.h>

namespace KDrawing
{
	template<typename ...Args>
	static auto DrawString(const Vec2& pos, const Color& color, bool shadow, const char* fmt, Args... args) -> void
	{
		if (shadow)
			GRender->DrawString(Vec2(pos.x + 1, pos.y + 1), Color::Black().Get(), fmt, args...);

		GRender->DrawString(pos, color.Get(), fmt, args...);
	}

	extern std::unordered_map<size_t, std::vector<std::pair<float, float>>> cacheSinCos;

	static auto DrawCircle(const Vec2& pos, const Color& color, float radius, size_t resolution) -> void
	{
		// credits to y3t1
		auto& sinCos = cacheSinCos[resolution];
		if (sinCos.size() == 0)
		{
			for (auto seg = 0U; seg <= resolution; ++seg)
			{
				auto mul = M_PI * (seg / (resolution * 0.5f));
				sinCos.emplace_back(std::make_pair(std::sinf(mul), std::cosf(mul)));
			}
		}

		for (auto it = sinCos.begin(); std::next(it) != sinCos.end(); ++it)
		{
			auto start = Vec2(pos.x + radius * std::get<0>(*it),
							  pos.y + radius * std::get<1>(*it));

			auto end = Vec2(pos.x + radius * std::get<0>(*std::next(it)),
							pos.y + radius * std::get<1>(*std::next(it)));

			GRender->DrawLine(start, end, color.Get());
		}
	}
}