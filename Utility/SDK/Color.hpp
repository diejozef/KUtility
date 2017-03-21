#pragma once
#include "Vector.hpp"

class Color
{
public:
	Color()
	{
		r = g = b = a = 0;
	}

	Color( const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a ) :
		r( r ), g( g ), b( b ), a( a ) { }

	Color( const uint8_t r, const uint8_t g, const uint8_t b ) :
		r( r ), g( g ), b( b ), a( 255 ) { }

	inline Vec4 Get() const
	{
		return Vec4( r, g, b, a );
	}

	inline Vec4 Get( const uint8_t alpha ) const
	{
		return Vec4( r, g, b, alpha );
	}

	inline DWORD GetHex() const
	{
		return ((DWORD)((((a) & 0xff) << 24) | (((r) & 0xff) << 16) | (((g) & 0xff) << 8) | ((b) & 0xff)));
	}

	inline DWORD GetHex( const uint8_t alpha ) const
	{
		return ((DWORD)((((alpha) & 0xff) << 24) | (((r) & 0xff) << 16) | (((g) & 0xff) << 8) | ((b) & 0xff)));
	}

	inline static Color White() { return Color( 255, 255, 255 ); }
	inline static Color Black() { return Color( 0, 0, 0 ); }
	inline static Color Red() { return Color( 255, 0, 0 ); }
	inline static Color Green() { return Color( 0, 255, 0 ); }
	inline static Color Blue() { return Color( 0, 0, 255 ); }
	inline static Color LightBlue() { return Color( 0, 115, 200 ); }
	inline static Color LightBlue2() { return Color( 65, 170, 255 ); }
	inline static Color LightBlue3() { return Color( 65, 140, 255 ); }
	inline static Color	LightGrey2() { return Color( 150, 150, 150 ); }
	inline static Color Grey() { return Color( 70, 70, 70 ); }
	inline static Color LightGrey() { return Color( 170, 170, 170 ); }
	inline static Color DarkGrey() { return Color( 45, 45, 45 ); }
	inline static Color Yellow() { return Color( 255, 255, 0 ); }
	inline static Color Magenta() { return Color( 255, 0, 255 ); }
	inline static Color DarkMagenta() { return Color( 139, 0, 139 ); }
	inline static Color Purple() { return Color( 128, 0, 128 ); }
	inline static Color DarkGreen() { return Color( 0, 128, 0 ); }
	inline static Color DarkGreen2() { return Color( 0, 100, 0 ); }
	inline static Color Crimson() { return Color( 220, 20, 60 ); }
	inline static Color LightRed2() { return Color( 255, 75, 75 ); }
	inline static Color LightGreen() { return Color( 75, 255, 75 ); }
	inline static Color LightGreen2() { return Color( 50, 255, 50 ); }
	inline static Color LightRed() { return Color( 255, 50, 50 ); }
	inline static Color Orange() { return Color( 255, 69, 0 ); }
	inline static Color Orange2() { return Color( 255, 120, 0 ); }

private:
	uint8_t a, r, g, b;
};