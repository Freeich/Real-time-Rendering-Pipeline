#pragma once
#include <cmath>
#include <vector>
#include <iostream>
#include <string>
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

	// 数乘
	Vector3 operator*(float k) {
		return Vector3(k * x, k * y, k * z);
	}

	float length() {
		return sqrt(x * x + y * y + z * z);
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

	// 归一化
	Vector3 Normalize() {
		float len = length();
		if (len > 0) return Vector3(x / len, y / len, z / len);
		else return Vector3(0, 0, 0);
	}
};

class Vector2 {
public:
	float x;
	float y;
	Vector2() :x(0.0), y(0.0) {}
	Vector2(float x_in = 0.0, float y_in = 0.0) :x(x_in), y(y_in) {}
	Vector2(const Vector2& v) :x(v.x), y(v.y){};


	// 加法
	Vector2 operator+(const Vector3& vector_sec) {
		return Vector2(x + vector_sec.x, y + vector_sec.y);
	}
	// 减法
	Vector2 operator-(const Vector3& vector_sec) {
		return Vector2(x - vector_sec.x, y - vector_sec.y);
	}

	// 数乘
	Vector2 operator*(float k) {
		return Vector2(k * x, k * y);
	}

	float length() {
		return sqrt(x * x + y * y);
	}
};

class Matrix {
public:

	// 二维数组
	std::vector<std::vector<float>> m;
	// 行 列
	int rows;
	int cols;
	// 构造函数
	Matrix() :m(std::vector<std::vector<float>>(4, std::vector<float>(4, 0.0f))), rows(4), cols(4) {}
	Matrix(int r, int c):m(std::vector<std::vector<float>>(r, std::vector<float>(c, 0.0f))), rows(r), cols(c){}

	float at(const int row, const int col) {
		return m[row][col];
	}


	// 加法
	Matrix operator+(const Matrix& matrix_sec) {
		//if (m.rows != rows or m.cols != cols);
		Matrix matrix_new = Matrix();
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < cols; j++) {
				matrix_new.m[i][j] = matrix_sec.m[i][j] + m[i][j];
			}
		}
		return matrix_new;
	}

	// 减法
	Matrix operator-(const Matrix& matrix_sec) {
		//if (m.rows != rows or m.cols != cols);
		Matrix matrix_new = Matrix();
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < cols; j++) {
				matrix_new.m[i][j] = m[i][j] - matrix_sec.m[i][j];
			}
		}
		return matrix_new;
	}


	// 乘法
	Matrix operator*(const Matrix& matrix_sec) {
		Matrix matrix_new = Matrix(rows, cols);
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < matrix_sec.cols; j++) 
			{
				for (size_t o = 0; o < cols; o++) 
				{
					matrix_new.m[i][j] += m[i][o] * matrix_sec.m[o][j];
				}
			}
		}
		return matrix_new;
	}

	// 旋转矩阵的逆
	// 矩阵的转置
	Matrix transpose() {
		Matrix matrix_tp(cols, rows);
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < cols; j++) {
				matrix_tp.m[j][i] = m[i][j];
			}
		}
	}

	// 输出矩阵
	void print(HDC hdc) {
		
		int u = 0;
		for (size_t i = 0; i < rows; i++)
		{
			int o = 0;
			for (size_t j = 0; j < cols; j++) {
				/*std::cout << m[i][j] << " ";*/
				std::string s = std::to_string(m[i][j]);
				TextOut(hdc, 500 + o, 500 + u, std::wstring(s.begin(), s.end()).c_str(), 1);
				o += 20;
			}
			//std::cout << std::endl;
			u += 20;
		}
	}
};

// DDA画线
void DrawLine(HDC hdc, Vector2 v1, Vector2 v2) {
	int x_len = v2.x - v1.x;
	int y_len = v2.y - v1.y;
	int step = max(abs(x_len), abs(y_len));
	float x_inc = x_len / (float)step;
	float y_inc = y_len / (float)step;
	float x = v1.x;
	float y = v1.y;
	for (size_t i = 0; i <= step; i++)
	{
		SetPixelV(hdc, round(x), round(y), RGB(255, 0, 0));
		x = x + x_inc;
		y = y + y_inc;
	}
	Ellipse(hdc, v1.x - 3, v1.y + 3, v1.x + 3, v1.y - 3);
	Ellipse(hdc, v2.x - 3, v2.y + 3, v2.x + 3, v2.y - 3);
}

// 画立方体线框