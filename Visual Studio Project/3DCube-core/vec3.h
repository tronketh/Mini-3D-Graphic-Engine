#pragma once
#include <math.h>
#include "mat4x4.h"

class vec3
{
public:
	float x, y, z, w;
	vec3();
	vec3(float a, float b, float c);
	float length() const;
	vec3 normal();
	vec3& operator*=(const float& n);
	vec3& operator/=(const float& n);
	vec3& operator+=(const vec3& n);
	vec3& operator-=(const vec3& n);
	vec3 operator+(const vec3& n);
	vec3 operator-(const vec3& n);
	float dotProduct(const vec3& n);
	float angle(const vec3& n);
	vec3 crossProduct(const vec3& n);

	friend vec3 operator*(const vec3& lhs, const float& rhs);
	friend vec3 operator*(const float& lhs, const vec3& rhs);
	friend vec3 operator/(const vec3& lhs, const float& rhs);
	friend vec3 operator/(const float& lhs, const vec3& rhs);

	friend vec3 operator*(const vec3& i, const mat4x4& m);
};

