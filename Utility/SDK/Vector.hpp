#pragma once
#include <Windows.h>
#include <cmath>

#pragma warning (disable : 4244)

class Vec2
{
public:
	float x;
	float y;

	Vec2()
	{
	}

	explicit Vec2( const float x, const float y )
	{
		this->x = x;
		this->y = y;
	}

	void Set( const float x, const float y )
	{
		this->x = x;
		this->y = y;
	}

	void Zero()
	{
		this->x = 0;
		this->y = 0;
	}

	float operator[]( int index ) const
	{
		return (&x)[ index ];
	}

	float& operator[]( int index )
	{
		return (&x)[ index ];
	}

	Vec2 operator-() const
	{
		return Vec2( -x, -y );
	}

	float operator*( Vec2& a ) const
	{
		return ((x * a.x) + (y * a.y));
	}

	Vec2 operator*( const float a ) const
	{
		return Vec2( x * a, y * a );
	}

	Vec2 operator/( const float a ) const
	{
		float inva = 1.f / a;
		return Vec2( x * inva, y * inva );
	}

	Vec2 operator+( const Vec2& a ) const
	{
		return Vec2( x + a.x, y + a.y );
	}

	Vec2 operator-( const Vec2& a ) const
	{
		return Vec2( x - a.x, y - a.y );
	}

	Vec2& operator+=( const Vec2& a )
	{
		x += a.x;
		y += a.y;
		return *this;
	}

	Vec2& operator-=( const Vec2& a )
	{
		x -= a.x;
		y -= a.y;
		return *this;
	}

	Vec2& operator/=( const Vec2& a )
	{
		x /= a.x;
		y /= a.y;
		return *this;
	}

	Vec2& operator/=( const float a )
	{
		x /= a;
		y /= a;
		return *this;
	}

	Vec2& operator*=( const float a )
	{
		x *= a;
		y *= a;
		return *this;
	}

	friend Vec2 operator*( const float a, const Vec2 b )
	{
		return Vec2( b.x * a, b.y * a );
	}

	bool operator==( const Vec2& a ) const
	{
		return Compare( a ) == true;
	}

	bool operator!=( const Vec2& a ) const
	{
		return Compare( a ) == false;
	}

	bool Compare( const Vec2& a ) const
	{
		return ((x == a.x) && (y == a.y));
	}

	Vec2& VectorNormalize()
	{
		float len = Length();
		*this /= len;
		return *this;
	}

	Vec2 Extended( const Vec2& to, float distance ) const
	{
		return *this + distance * (to - *this).VectorNormalize();
	}

	Vec2 Perpendicular() const
	{
		return Vec2( -y, x );
	}

	Vec2 Rotated( float angle ) const
	{
		auto c = std::cos( angle );
		auto s = std::sin( angle );

		return Vec2( (x * c - y * s), (y * c + x * s) );
	}

	float Length() const
	{
		return sqrtf( x * x + y * y );
	}

	float LengthSqr() const
	{
		return (x * x + y * y);
	}

	float DotProduct( const Vec2& vecOther ) const
	{
		return ((x * vecOther.x) + (y * vecOther.y));
	}

	float CrossProduct( const Vec2& vecOther ) const
	{
		return ((vecOther.y * x) - (vecOther.x * y));
	}
};

class Vec3
{
public:
	float x;
	float y;
	float z;

	Vec3()
	{
	}

	explicit Vec3( const float x, const float y, const float z )
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void Set( const float x, const float y, const float z )
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void Zero()
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	float operator[]( int index ) const
	{
		return (&x)[ index ];
	}

	float& operator[]( int index )
	{
		return (&x)[ index ];
	}

	Vec3 operator-() const
	{
		return Vec3( -x, -y, -z );
	}

	float operator*( Vec3& a ) const
	{
		return ((x * a.x) + (y * a.y) + (z * a.z));
	}

	Vec3 operator*( const float a ) const
	{
		return Vec3( x * a, y * a, z * a );
	}

	Vec3 operator/( const float a ) const
	{
		float inva = 1.f / a;
		return Vec3( x * inva, y * inva, z * inva );
	}

	Vec3 operator+( const Vec3& a ) const
	{
		return Vec3( x + a.x, y + a.y, z + a.z );
	}

	Vec3 operator-( const Vec3& a ) const
	{
		return Vec3( x - a.x, y - a.y, z - a.z );
	}

	Vec3& operator+=( const Vec3& a )
	{
		x += a.x;
		y += a.y;
		z += a.z;
		return *this;
	}

	Vec3& operator-=( const Vec3& a )
	{
		x -= a.x;
		y -= a.y;
		z -= a.z;
		return *this;
	}

	Vec3& operator/=( const Vec3& a )
	{
		x /= a.x;
		y /= a.y;
		z /= a.z;
		return *this;
	}

	Vec3& operator/=( const float a )
	{
		x /= a;
		y /= a;
		z /= a;
		return *this;
	}

	Vec3& operator+( const float a )
	{
		x += a;
		y += a;
		z += a;
		return *this;
	}

	Vec3& operator*=( const float a )
	{
		x *= a;
		y *= a;
		z *= a;
		return *this;
	}

	friend Vec3 operator*( const float a, const Vec3& b )
	{
		return Vec3( b.x * a, b.y * a, b.z * a );
	}

	bool operator==( const Vec3& a ) const
	{
		return Compare( a ) == true;
	}

	bool operator!=( const Vec3& a ) const
	{
		return Compare( a ) == false;
	}

	bool Compare( const Vec3& a ) const
	{
		return ((x == a.x) && (y == a.y) && (z == a.z));
	}

	Vec3& VectorNormalize()
	{
		float len = Length();

		*this /= len;

		return *this;
	}

	Vec3 Extended( const Vec3& to, float distance ) const
	{
		return *this + distance * (to - *this).VectorNormalize();
	}

	Vec2 To2D() const
	{
		return Vec2( x, y );
	}

	// To2D function made for Riot's 3D coords
	Vec2 To2DEx() const
	{
		return Vec2( x, z );
	}

	float Length() const
	{
		return sqrtf( x * x + y * y + z * z );
	}

	float Length2D() const
	{
		return sqrtf( x * x + z * z );
	}

	float DotProduct( const Vec3& vecOther ) const
	{
		return ((x * vecOther.x) + (y * vecOther.y) + (z * vecOther.z));
	}
};

class Vec4
{
public:
	float x;
	float y;
	float z;
	float w;

	Vec4()
	{
	}

	explicit Vec4( const float x, const float y, const float z, const float w )
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	void Set( const float x, const float y, const float z, const float w )
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	void Zero()
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->w = 0;
	}

	float operator[]( int index ) const
	{
		return (&x)[ index ];
	}

	float& operator[]( int index )
	{
		return (&x)[ index ];
	}

	Vec4 operator-() const
	{
		return Vec4( -x, -y, -z, -w );
	}

	float operator*( Vec4& a ) const
	{
		return ((x * a.x) + (y * a.y) + (z * a.z) + (w * a.w));
	}

	Vec4 operator*( const float a ) const
	{
		return Vec4( x * a, y * a, z * a, w * a );
	}

	Vec4 operator/( const float a ) const
	{
		float inva = 1.f / a;
		return Vec4( x * inva, y * inva, z * inva, w * inva );
	}

	Vec4 operator+( const Vec4& a ) const
	{
		return Vec4( x + a.x, y + a.y, z + a.z, w + a.w );
	}

	Vec4 operator-( const Vec4& a ) const
	{
		return Vec4( x - a.x, y - a.y, z - a.z, w - a.w );
	}

	Vec4& operator+=( const Vec4& a )
	{
		x += a.x;
		y += a.y;
		z += a.z;
		w += a.w;
		return *this;
	}

	Vec4& operator-=( const Vec4& a )
	{
		x -= a.x;
		y -= a.y;
		z -= a.z;
		w -= a.w;
		return *this;
	}

	Vec4& operator/=( const Vec4& a )
	{
		x /= a.x;
		y /= a.y;
		z /= a.z;
		w /= a.w;
		return *this;
	}

	Vec4& operator/=( const float a )
	{
		x /= a;
		y /= a;
		z /= a;
		w /= a;
		return *this;
	}

	Vec4& operator*=( const float a )
	{
		x *= a;
		y *= a;
		z *= a;
		w *= a;
		return *this;
	}

	friend Vec4 operator*( const float a, const Vec4 b )
	{
		return Vec4( b.x * a, b.y * a, b.z * a, b.w * a );
	}

	bool operator==( const Vec4& a ) const
	{
		return Compare( a ) == true;
	}

	bool operator!=( const Vec4& a ) const
	{
		return Compare( a ) == false;
	}

	bool Compare( const Vec4& a ) const
	{
		return ((x == a.x) && (y == a.y) && (z == a.z) && (w == a.w));
	}

	void Negate()
	{
		this->x = -this->x;
		this->y = -this->y;
		this->z = -this->z;
	}

	Vec4& VectorNormalize()
	{
		float len = Length();

		*this /= len;

		return *this;
	}

	float Length()
	{
		return sqrtf( x * x + y * y + z * z + w * w );
	}

	float DotProduct( const Vec4& vecOther )
	{
		return ((x * vecOther.x) + (y * vecOther.y) + (z * vecOther.z) + (w * vecOther.w));
	}

	float DotProduct( const Vec3& vecOther )
	{
		return ((x * vecOther.x) + (y * vecOther.y) + (z * vecOther.z));
	}
};