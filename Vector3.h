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
	Vector3 operator+(const Vector3& vector_sec) {
		return Vector3(x + vector_sec.x, y + vector_sec.y, z + vector_sec.z);
	}
	// 减法
	Vector3 operator-(const Vector3& vector_sec) {
		return Vector3(x - vector_sec.x, y - vector_sec.y, z - vector_sec.z);
	}
	// 点积
	float DotProduct(const Vector3& vector_sec) {
		float result = x * vector_sec.x + y * vector_sec.y + z * vector_sec.z;
		return result;
	}
	// 叉积
	Vector3 CrossProduct(const Vector3& vector_sec) {
		float x_normal = y * vector_sec.z - vector_sec.y * z;
		float y_normal = z * vector_sec.x - x * vector_sec.z;
		float z_normal = x * vector_sec.y - y * vector_sec.x;
		return Vector3(x_normal, y_normal, z_normal);
	}
};