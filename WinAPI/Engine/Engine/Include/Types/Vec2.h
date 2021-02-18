#pragma once
#include <cmath>

template <typename T>
class Vec2_
{
public:
	Vec2_() = default;
	Vec2_(T x_in, T y_in)
		:
		x(x_in),
		y(y_in)
	{}
	template<typename S>
	explicit Vec2_(const Vec2_<S>& src)
		:
		x( (T)src.x ),
		y( (T)src.y )
	{}
	Vec2_ operator+(const Vec2_& rhs) const
	{
		return Vec2_(x + rhs.x, y + rhs.y);
	}
	Vec2_& operator+=(const Vec2_& rhs)
	{
		return *this = *this + rhs;
	}
	Vec2_ operator*(T rhs) const
	{
		return Vec2_(x * rhs, y * rhs);
	}
	Vec2_& operator*=(T rhs)
	{
		return *this = *this * rhs;
	}
	Vec2_ operator-(const Vec2_& rhs) const
	{
		return Vec2_(x - rhs.x, y - rhs.y);
	}
	Vec2_& operator-=(const Vec2_& rhs)
	{
		return *this = *this - rhs;
	}
	float GetLength() const
	{
		return std::sqrtf((float) GetLengthSq());
	}
	T GetLengthSq() const
	{
		return x * x + y * y;
	}
	Vec2_& Normalize()
	{
		return *this = GetNormalized();
	}
	Vec2_ GetNormalized() const
	{
		const float len = GetLength();
		if (len != 0.0f)
		{
			return *this * T(1.0f / len);
		}
		return *this;
	}
public:
	T x;
	T y;
};

typedef Vec2_<float> Vec2F;
typedef Vec2_<int> Vec2I;