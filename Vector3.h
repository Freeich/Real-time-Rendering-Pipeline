#pragma once
class Vector3 {
public:
	float x;
	float y;
	float z;
	Vector3():x(0.0), y(0.0), z(0.0) {}
	Vector3(float x_in = 0.0, float y_in = 0.0, float z_in = 0.0) :x(x_in), y(y_in), z(z_in) {}
	Vector3(const Vector3& v) :x(v.x), y(v.y), z(v.z) {};


	// 加法

	// 减法

	// 点积

	// 乘积

};