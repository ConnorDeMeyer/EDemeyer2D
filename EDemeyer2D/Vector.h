#pragma once
#include <math.h>

template <typename T>
struct Vector2 final {

	Vector2() = default;
	Vector2(const T& _x, const T& _y) : x{ _x }, y{ _y }{}

	T x = 0;
	T y = 0;

	
	inline Vector2<T> operator-() const
	{
		return Vector2<T>(-x, -y);
	}
	inline Vector2<T> operator+(const Vector2<T>& other) const
	{
		return Vector2<T>(x + other.x, y + other.y);
	}
	inline Vector2<T> operator-(const Vector2<T>& other) const
	{
		return Vector2<T>(x - other.x, y - other.y);
	}
	inline Vector2<T> operator*(T scale) const
	{
		return Vector2<T>(scale * x, scale * y);
	}
	inline Vector2<T> operator/(T scale) const
	{
		float inv = 1.f / scale;
		return Vector2<T>(inv * x, inv * y);
	}
	inline Vector2<T>& operator+=(const Vector2<T>& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}
	inline Vector2<T>& operator-=(const Vector2<T>& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}
	inline Vector2<T>& operator*=(float scale)
	{
		x *= scale;
		y *= scale;
		return *this;
	}
	inline Vector2<T>& operator/=(float scale)
	{
		float inv = 1.f / scale;
		x *= inv;
		y *= inv;
		return *this;
	}
	inline Vector2<T>& operator=(const Vector2<T>& other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}

};

template <typename T>
Vector2<T> operator*(T scalar, Vector2<T> vector)
{
	return vector * scalar;
}

template <typename T>
bool operator==(Vector2<T> vector0, Vector2<T> vector1)
{
	return (vector0.x == vector1.x && vector0.y == vector1.y);
}

template <typename T>
bool operator!=(Vector2<T> vector0, Vector2<T> vector1)
{
	return !(vector0 == vector1);
}

template <typename T>
inline float Normalize(Vector2<T>& v)
{
	float mag = Magnitude(v);
	if (abs(mag) < 0.0001f)
	{
		v.x = 0;
		v.y = 0;
		return mag;
	}
	v /= mag;
	return mag;
}

template <typename T>
inline Vector2<T> GetNormalized(const Vector2<T>& v)
{
	auto newVector = v;
	Normalize(v);
	return v;
}

template <typename T>
inline float SqrMagnitude(const Vector2<T>& v)
{
	return Dot(v, v);
}

template <typename T>
inline float Magnitude(const Vector2<T>& v)
{
	return sqrtf(SqrMagnitude(v));
}

template <typename T>
inline float Dot(const Vector2<T>& v0, const Vector2<T>& v1)
{
	return float(v0.x * v1.x + v0.y * v1.y);
}

template <typename T>
inline float Cross(const Vector2<T>& v0, const Vector2<T>& v1)
{
	return float(v0.x * v1.y - v0.y * v1.x);
}

template <typename T>
inline float SqrDistance(const Vector2<T>& v0, const Vector2<T>& v1)
{
	return float((v0.x - v1.x) * (v0.x - v1.x) + (v0.y - v1.y) * (v0.y - v1.y));
}

template <typename T>
inline float Distance(const Vector2<T>& v0, const Vector2<T>& v1)
{
	return sqrtf(SqrDistance(v0, v1));
}

template <typename T>
inline float GetAngle(const Vector2<T>& v0, const Vector2<T>& v1)
{
	return acosf(Dot(v0, v1) / (Magnitude(v0) * Magnitude(v1)));
}

template <typename T>
inline float Lerp(float t, const Vector2<T>& v0, const Vector2<T>& v1)
{
	return v1 + ((v1 - v0) * t);
}