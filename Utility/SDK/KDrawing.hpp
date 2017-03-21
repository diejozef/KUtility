#pragma once
#include <PluginSDK.h>

namespace KDrawing
{
	template<typename ...Args>
	static auto DrawString( const Vec2& pos, const Color& color, bool shadow, const char* fmt, Args... args ) -> void
	{
		if ( shadow )
			GRender->DrawString( Vec2( pos.x + 1, pos.y + 1 ), Color::Black().Get(), fmt, args... );

		GRender->DrawString( pos, color.Get(), fmt, args... );
	}
}