#include "vec3.h"

vec3::vec3()
{
	x = y = z = 0;
	w = 1;
}

vec3::vec3(float a, float b, float c) {
	x = a;
	y = b;
	z = c;
	w = 1;
}

float vec3::length() const {
	return sqrt((double)x * x + (double)y * y + (double)z * z);
}

vec3 vec3::normal() {
	return *this / length();
}

vec3& vec3::operator*=(const float& n) {
	this->x *= n;
	this->y *= n;
	this->z *= n;
	return *this;
}
vec3& vec3::operator/=(const float& n) {
	this->x /= n;
	this->y /= n;
	this->z /= n;
	return *this;
}

vec3& vec3::operator+=(const vec3 & n) {
	this->x += n.x;
	this->y += n.y;
	this->z += n.z;
	return *this;
}

vec3& vec3::operator-=(const vec3 & n) {
	this->x -= n.x;
	this->y -= n.y;
	this->z -= n.z;
	return *this;
}

vec3 vec3::operator+(const vec3 & n) {
	vec3 temp;
	temp.x = this->x + n.x;
	temp.y = this->y + n.y;
	temp.z = this->z + n.z;
	return temp;
}

vec3 vec3::operator-(const vec3 & n) {
	vec3 temp;
	temp.x = this->x - n.x;
	temp.y = this->y - n.y;
	temp.z = this->z - n.z;
	return temp;
}

float vec3::dotProduct(const vec3 & n) {
	return x * n.x + y * n.y + z * n.z;
}

float vec3::angle(const vec3 & n) {
	return acosf(dotProduct(n) / length() * n.length());
}

vec3 vec3::crossProduct(const vec3 & n) {
	vec3 temp;
	temp.x = y * n.z - z * n.y;
	temp.y = z * n.x - x * n.z;
	temp.z = x * n.y - y * n.x;
	return temp;
}

//normal multiplication
vec3 operator*(const vec3& lhs, const float& rhs) {
	vec3 temp;
	temp.x = lhs.x * rhs;
	temp.y = lhs.y * rhs;
	temp.z = lhs.z * rhs;
	return temp;
}
//commutative multiplication
vec3 operator*(const float& lhs, const vec3& rhs) {
	vec3 temp;
	temp.x = lhs * rhs.x;
	temp.y = lhs * rhs.y;
	temp.z = lhs * rhs.z;
	return temp;
}

vec3 operator/(const vec3& lhs, const float& rhs) {
	vec3 temp;
	temp.x = lhs.x / rhs;
	temp.y = lhs.y / rhs;
	temp.z = lhs.z / rhs;
	return temp;
}

vec3 operator/(const float& lhs, const vec3& rhs) {
	vec3 temp;
	temp.x = lhs / rhs.x;
	temp.y = lhs / rhs.y;
	temp.z = lhs / rhs.z;
	return temp;
}

vec3 operator*(const vec3& i, const mat4x4& m) {
	vec3 o;
	o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
	o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
	o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
	o.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
	return o;
}

