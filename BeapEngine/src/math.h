#pragma once

class vector3 {
public:

	float x;
	float y;
	float z;


	explicit vector3(float value);
	vector3(float x, float y);
	vector3(float x, float y, float z);
	vector3() = default;

	float dot(const vector3& other) const;
	vector3 cross(const vector3& other) const;
	//vector3 rotate() const;
	float magnitude() const;
	float magnitudesq() const;
	vector3 normalized() const;
	void normalize();


	vector3 operator + (const vector3& other);
	vector3 operator - (const vector3& other);
	vector3 operator * (const vector3& other);
	vector3 operator * (float scale);
	vector3 operator / (const vector3& other);
	vector3 operator / (float scale);
	bool operator==(const vector3& other) const
	{
		return (x == other.x && y == other.y && z == other.z);
	}
};