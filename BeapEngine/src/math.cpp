#include <cmath>
#include "math.h"

vector3::vector3(float value) : x(value), y(value), z(value) {}
vector3::vector3(float x, float y) : x(x), y(y), z(0) {}
vector3::vector3(float x, float y, float z) : x(x), y(y), z(z) {}

float vector3::dot(const vector3& other) const {
	return x * other.x + y * other.y;
}

vector3 vector3::cross(const vector3& other) const {
	return vector3(
		y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.z
	);
}

float vector3::magnitude() const {
	return std::sqrtf(x * x + y * y + z * z);
}
float vector3::magnitudesq() const {
	return x * x + y * y + z * z;
}

vector3 vector3::normalized() const {
	float m = 1 / this->magnitude();
	return vector3(x * m, y * m, z * m);
}

void vector3::normalize() {
	float m = 1 / this->magnitude();
	this->x *= m;
	this->y *= m;
	this->z *= m;
}

vector3 vector3::operator + (const vector3& other) {
	return vector3(x + other.x, y + other.y, z + other.z);
}
vector3 vector3::operator - (const vector3& other) {
	return vector3(x - other.x, y - other.y, z - other.z);
}

vector3 vector3::operator * (const vector3& other) {
	return vector3(x * other.x, y * other.y, z * other.z);
}

vector3 vector3::operator * (float scale) {
	return vector3(x * scale, y * scale, z * scale);
}

vector3 vector3::operator / (const vector3& other) {
	return vector3(x / other.x, y / other.y, z / other.z);
}

vector3 vector3::operator / (float scale) {
	float v = 1 / scale;
	return vector3(x * v, y * v, z * v);
}