#pragma once

#include <string>

#include "Math.h"
#include "Vector2.h"
#include "Vector3.h"

struct Vector4
{
public:
	float x;
	float y;
	float z;
	float w;

public:

	Vector4(const Vector3& inVector, float inW = 1.0f);

	explicit Vector4(float inX = 0.0f, float inY = 0.0f, float inZ = 0.0f, float inW = 1.0f);

	explicit Vector4(Vector2 inXY, Vector2 inZW);

	inline float& operator[](int32 index);

	inline float operator[](int32 index) const;

	inline Vector4 operator-() const;

	inline Vector4 operator+(const Vector4& v) const;

	inline Vector4 operator+=(const Vector4& v);

	inline Vector4 operator-(const Vector4& v) const;

	inline Vector4 operator-=(const Vector4& v);

	inline Vector4 operator*(float scale) const;

	inline Vector4 operator/(float scale) const;

	inline Vector4 operator/(const Vector4& v) const;

	inline Vector4 operator*(const Vector4& v) const;

	inline Vector4 operator*=(const Vector4& v);

	inline Vector4 operator/=(const Vector4& v);

	inline Vector4 operator*=(float f);

	inline bool operator==(const Vector4& v) const;

	inline bool operator!=(const Vector4& v) const;

	inline Vector4 operator^(const Vector4& v) const;

	inline float& Component(int32 index);

	inline const float& Component(int32 index) const;

	inline bool Equals(const Vector4& v, float tolerance = KINDA_SMALL_NUMBER) const;

	inline bool IsUnit3(float LengthSquaredTolerance = KINDA_SMALL_NUMBER) const;

	inline Vector4 GetSafeNormal(float tolerance = SMALL_NUMBER) const;

	inline Vector4 GetUnsafeNormal3() const;

	inline void Set(float inX, float inY, float inZ, float inW);

	inline float Size3() const;

	inline float SizeSquared3() const;

	inline float Size() const;

	inline float SizeSquared() const;

	inline bool ContainsNaN() const;

	inline bool IsNearlyZero3(float tolerance = KINDA_SMALL_NUMBER) const;

	inline Vector4 Reflect3(const Vector4& normal) const;

	inline void FindBestAxisVectors3(Vector4& axis1, Vector4& axis2) const;

	inline static Vector4 Min(const Vector4& a, const Vector4& b);

	inline static Vector4 Max(const Vector4& a, const Vector4& b);

	friend inline float Dot3(const Vector4& v1, const Vector4& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	friend inline float Dot4(const Vector4& v1, const Vector4& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
	}

	friend inline Vector4 operator*(float scale, const Vector4& v)
	{
		return v.operator*(scale);
	}
};

inline Vector4::Vector4(const Vector3& inVector, float inW)
	: x(inVector.x)
	, y(inVector.y)
	, z(inVector.z)
	, w(inW)
{

}

inline Vector4::Vector4(float inX, float inY, float inZ, float inW)
	: x(inX)
	, y(inY)
	, z(inZ)
	, w(inW)
{

}

inline Vector4::Vector4(Vector2 inXY, Vector2 inZW)
	: x(inXY.x)
	, y(inXY.y)
	, z(inZW.x)
	, w(inZW.y)
{

}

inline float& Vector4::operator[](int32 index)
{
	return (&x)[index];
}

inline float Vector4::operator[](int32 index) const
{
	return (&x)[index];
}

inline void Vector4::Set(float inX, float inY, float inZ, float inW)
{
	x = inX;
	y = inY;
	z = inZ;
	w = inW;
}

inline Vector4 Vector4::operator-() const
{
	return Vector4(-x, -y, -z, -w);
}

inline Vector4 Vector4::operator+(const Vector4& v) const
{
	return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

inline Vector4 Vector4::operator+=(const Vector4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;

	return *this;
}

inline Vector4 Vector4::operator-(const Vector4& v) const
{
	return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

inline Vector4 Vector4::operator-=(const Vector4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;

	return *this;
}

inline Vector4 Vector4::operator*(float scale) const
{
	return Vector4(x * scale, y * scale, z * scale, w * scale);
}

inline Vector4 Vector4::operator/(float scale) const
{
	const float invScale = 1.f / scale;
	return Vector4(x * invScale, y * invScale, z * invScale, w * invScale);
}

inline Vector4 Vector4::operator*(const Vector4& v) const
{
	return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
}

inline Vector4 Vector4::operator^(const Vector4& v) const
{
	return Vector4(
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x,
		0.0f
	);
}

inline float& Vector4::Component(int32 index)
{
	return (&x)[index];
}

inline const float& Vector4::Component(int32 index) const
{
	return (&x)[index];
}

inline bool Vector4::operator==(const Vector4& v) const
{
	return ((x == v.x) && (y == v.y) && (z == v.z) && (w == v.w));
}

inline bool Vector4::operator!=(const Vector4& v) const
{
	return ((x != v.x) || (y != v.y) || (z != v.z) || (w != v.w));
}

inline bool Vector4::Equals(const Vector4& v, float tolerance) const
{
	return math::Abs(x - v.x) <= tolerance && math::Abs(y - v.y) <= tolerance && math::Abs(z - v.z) <= tolerance && math::Abs(w - v.w) <= tolerance;
}

inline Vector4 Vector4::GetSafeNormal(float tolerance) const
{
	const float squareSum = x * x + y * y + z * z;
	if (squareSum > tolerance)
	{
		const float scale = math::InvSqrt(squareSum);
		return Vector4(x * scale, y * scale, z * scale, 0.0f);
	}
	return Vector4(0.f);
}

inline Vector4 Vector4::GetUnsafeNormal3() const
{
	const float scale = math::InvSqrt(x * x + y * y + z * z);
	return Vector4(x * scale, y * scale, z * scale, 0.0f);
}

inline float Vector4::Size3() const
{
	return math::Sqrt(x * x + y * y + z * z);
}

inline float Vector4::SizeSquared3() const
{
	return x * x + y * y + z * z;
}

inline float Vector4::Size() const
{
	return math::Sqrt(x * x + y * y + z * z + w * w);
}

inline float Vector4::SizeSquared() const
{
	return x * x + y * y + z * z + w * w;
}

inline bool Vector4::IsUnit3(float LengthSquaredTolerance) const
{
	return math::Abs(1.0f - SizeSquared3()) < LengthSquaredTolerance;
}

inline bool Vector4::ContainsNaN() const
{
	return (
		!math::IsFinite(x) ||
		!math::IsFinite(y) ||
		!math::IsFinite(z) ||
		!math::IsFinite(w)
		);
}

inline bool Vector4::IsNearlyZero3(float tolerance) const
{
	return
		math::Abs(x) <= tolerance &&
		math::Abs(y) <= tolerance &&
		math::Abs(z) <= tolerance;
}

inline Vector4 Vector4::Reflect3(const Vector4& normal) const
{
	return 2.0f * Dot3(*this, normal) * normal - *this;
}

inline Vector4 Vector4::Min(const Vector4& a, const Vector4& b)
{
	Vector4 result;
	result.x = math::Min(a.x, b.x);
	result.y = math::Min(a.y, b.y);
	result.z = math::Min(a.z, b.z);
	result.w = math::Min(a.w, b.w);
	return result;
}

inline Vector4 Vector4::Max(const Vector4& a, const Vector4& b)
{
	Vector4 result;
	result.x = math::Max(a.x, b.x);
	result.y = math::Max(a.y, b.y);
	result.z = math::Max(a.z, b.z);
	result.w = math::Max(a.w, b.w);
	return result;
}

inline void Vector4::FindBestAxisVectors3(Vector4& axis1, Vector4& axis2) const
{
	const float nx = math::Abs(x);
	const float ny = math::Abs(y);
	const float nz = math::Abs(z);

	if (nz > nx && nz > ny)
	{
		axis1 = Vector4(1, 0, 0);
	}
	else
	{
		axis1 = Vector4(0, 0, 1);
	}

	axis1 = (axis1 - *this * Dot3(axis1, *this)).GetSafeNormal();
	axis2 = axis1 ^ *this;
}

inline Vector4 Vector4::operator*=(const Vector4& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;

	return *this;
}

inline Vector4 Vector4::operator/=(const Vector4& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;

	return *this;
}

inline Vector4 Vector4::operator*=(float f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;

	return *this;
}

inline Vector4 Vector4::operator/(const Vector4& v) const
{
	return Vector4(x / v.x, y / v.y, z / v.z, w / v.w);
}