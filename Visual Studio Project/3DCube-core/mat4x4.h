#pragma once
class mat4x4
{
public:
	float m[4][4] = { 0.0f };
	mat4x4 operator*(const mat4x4 rhs);
	mat4x4& operator*=(const mat4x4 rhs);
};

