#pragma once
#include <cmath>
#include <vector>
#include <iostream>
#include <string>

const float PI = 3.14159265358979323846;

class Matrix {
public:

	// 二维数组
	std::vector<std::vector<float>> m;
	// 行 列
	int rows;
	int cols;
	// 构造函数
	Matrix() :m(std::vector<std::vector<float>>(4, std::vector<float>(4, 0.0f))), rows(4), cols(4) {}
	Matrix(int r, int c) :m(std::vector<std::vector<float>>(r, std::vector<float>(c, 0.0f))), rows(r), cols(c) {}

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
		Matrix matrix_new = Matrix(rows, matrix_sec.cols);
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
		return matrix_tp;
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

class Vector3 {
public:
	float x;
	float y;
	float z;
	//Vector3():x(0.0f), y(0.0f), z(0.0f) {}
	Vector3(float x_in = 0.0f, float y_in = 0.0f, float z_in = 0.0f) :x(x_in), y(y_in), z(z_in) {}
	Vector3(const Vector3& v) :x(v.x), y(v.y), z(v.z) {};


	// 加法
	Vector3 operator+(const Vector3& vector_sec) const {
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
	Vector3 CrossProduct(const Vector3& vector_sec) const {
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

	// 转化为矩阵
	Matrix PointToMatrix() {
		Matrix vm(4, 1);
		vm.m[0][0] = x;
		vm.m[1][0] = y;
		vm.m[2][0] = z;
		vm.m[3][0] = 1.f;
		return vm;
	}

	Matrix VectorToMatrix() {
		Matrix vm(4, 1);
		vm.m[0][0] = x;
		vm.m[1][0] = y;
		vm.m[2][0] = z;
		vm.m[3][0] = 0.f;
		return vm;
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


// DDA画线
void DrawLine(HDC hdc, const Vector2& v1, const Vector2& v2) {
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
	// 
	Ellipse(hdc, v1.x - 3, v1.y + 3, v1.x + 3, v1.y - 3);
	Ellipse(hdc, v2.x - 3, v2.y + 3, v2.x + 3, v2.y - 3);
}

void DrawLine(HDC hdc, const Vector3& v1, const Vector3& v2) {
	int x_len = v2.x - v1.x;
	int y_len = v2.y - v1.y;
	int step = max(abs(x_len), abs(y_len));
	if (step == 0) return;
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
	// 
	Ellipse(hdc, v1.x - 3, v1.y + 3, v1.x + 3, v1.y - 3);
	Ellipse(hdc, v2.x - 3, v2.y + 3, v2.x + 3, v2.y - 3);
}

// 摄像机类
class Camera {
public:
	Vector3 location_;
	Vector3 lookat_;
	Vector3 top_;
	float pitch_;
	float yaw_;
	float roll_;

	Camera() : 
		location_(Vector3(0.f, 0.f, 0.f)), 
		lookat_(Vector3(0.f, 0.f, -1.f)), 
		top_(Vector3(0.f, 1.f, 0.f)),
		pitch_(0.0f),
		yaw_(0.0f),
		roll_(0.0f){};

	Camera(const Vector3& location, const Vector3& lookat = Vector3(0.f, 0.f, -1.f), const Vector3& top = Vector3(0.f, 1.f, 0.f)) :
		location_(location), 
		lookat_(lookat), 
		top_(top),
		pitch_(0.0f),
		yaw_(0.0f),
		roll_(0.0f) {};

	Vector3 GetLocation() { return location_; };
	Vector3 GetTop() { return top_; };

	Vector3 GetLookat() { 
		Matrix pitch_rotate = Matrix();
		Matrix yaw_rotate = Matrix();
		Matrix roll_rotate = Matrix();

		float pitch = pitch_ * PI / 180.f;
		float yaw = yaw_ * PI / 180.f;
		float roll = roll_ * PI / 180.f;


		pitch_rotate.m[0][0] = cos(pitch);
		pitch_rotate.m[0][2] = sin(pitch);
		pitch_rotate.m[1][1] = 1.f;
		pitch_rotate.m[2][0] = -sin(pitch);
		pitch_rotate.m[2][2] = cos(pitch);
		pitch_rotate.m[3][3] = 1.f;

		yaw_rotate.m[0][0] = cos(yaw);
		yaw_rotate.m[0][1] = -sin(yaw);
		yaw_rotate.m[1][0] = sin(yaw);
		yaw_rotate.m[1][1] = cos(yaw);
		yaw_rotate.m[2][2] = 1.f;
		yaw_rotate.m[3][3] = 1.f;

		roll_rotate.m[0][0] = 1.f;
		roll_rotate.m[1][1] = cos(roll);
		roll_rotate.m[1][2] = -sin(roll);
		roll_rotate.m[2][1] = sin(roll);
		roll_rotate.m[2][2] = cos(roll);
		roll_rotate.m[3][3] = 1.f;
		
		Matrix lookat_m = lookat_.VectorToMatrix();
		Matrix lookat_matrix = pitch_rotate * yaw_rotate * roll_rotate * lookat_m;

		lookat_.x = lookat_matrix.m[0][0];
		lookat_.y = lookat_matrix.m[1][0];
		lookat_.z = lookat_matrix.m[2][0];
		
		return lookat_;
	};
};

// MVP算法
Vector3 MVP(Vector3& point, Camera camera) {
	
	// Vector转换为Matrix方便计算
	Matrix point_matrix = point.PointToMatrix();

	Vector3 e = camera.GetLocation();
	Vector3 g = camera.GetLookat();
	Vector3 top = camera.GetTop();

	//g = (g - e).Normalize();
	g = g.Normalize();

	// 视角变换
	Matrix translate(4, 4);
	translate.m[0][0] = 1.f;
	translate.m[1][1] = 1.f;
	translate.m[2][2] = 1.f;
	translate.m[3][3] = 1.f;
	translate.m[0][3] = -e.x;
	translate.m[1][3] = -e.y;
	translate.m[2][3] = -e.z;

	Vector3 r = g.CrossProduct(top);

	// 矫正相机top向量
	Vector3 t = r.CrossProduct(g);

	// 旋转矩阵的逆矩阵
	Matrix rotate_inverse(4, 4);
	rotate_inverse.m[0][0] = r.x;
	rotate_inverse.m[1][0] = r.y;
	rotate_inverse.m[2][0] = r.z;
	rotate_inverse.m[0][1] = t.x;
	rotate_inverse.m[1][1] = t.y;
	rotate_inverse.m[2][1] = t.z;
	rotate_inverse.m[0][2] = -g.x;
	rotate_inverse.m[1][2] = -g.y;
	rotate_inverse.m[2][2] = -g.z;
	rotate_inverse.m[3][3] = 1.f;

	// 旋转矩阵
	Matrix rotate = rotate_inverse.transpose();

	point_matrix = translate * point_matrix;
	point_matrix = rotate * point_matrix;
	
	// 透视矩阵
	float fov = 90.f;
	float aspect = 16.f / 9.f;
	float z_near = - 0.3f;
	float z_far = - 500.f;

	Matrix pers_proj(4, 4);

	pers_proj.m[0][0] = z_near / 1.f;
	pers_proj.m[1][1] = z_near / 1.f;
	pers_proj.m[2][2] = (z_near + z_far) / (z_far - z_near);
	pers_proj.m[2][3] = -1 * (2 * z_near * z_far) / (z_far - z_near);
	pers_proj.m[3][2] = 1.f;

	point_matrix = pers_proj * point_matrix;

	// 裁剪
	if (point_matrix.m[3][0] != 0.f) {
		point_matrix.m[0][0] = point_matrix.m[0][0] / point_matrix.m[3][0];
		point_matrix.m[1][0] = point_matrix.m[1][0] / point_matrix.m[3][0];
		point_matrix.m[2][0] = point_matrix.m[2][0] / point_matrix.m[3][0];
		point_matrix.m[3][0] = point_matrix.m[3][0] / point_matrix.m[3][0];
	}

	// 视口变换
	float width = 1600.f;
	float height = 900.f;
	Matrix viewport_matrix(4, 4);
	viewport_matrix.m[0][0] = width / 2;
	viewport_matrix.m[0][3] = width / 2;
	viewport_matrix.m[1][1] = height / 2;
	viewport_matrix.m[1][3] = height / 2;
	viewport_matrix.m[2][2] = 1.f;
	viewport_matrix.m[3][3] = 1.f;

	point_matrix = viewport_matrix * point_matrix;
	return Vector3(point_matrix.m[0][0], point_matrix.m[1][0], point_matrix.m[2][0]);
}



// 画立方体线框
void DrawCube(HDC hdc, const Camera& camera, const Vector3& start, float length, float width, float height) {
	Vector3 bottom_1 = start;
	Vector3 bottom_2(bottom_1.x + length, bottom_1.y, bottom_1.z);
	Vector3 bottom_3(bottom_1.x + length, bottom_1.y, bottom_1.z - width);
	Vector3 bottom_4(bottom_1.x, bottom_1.y, bottom_1.z - width);
	Vector3 top_1(bottom_1.x, bottom_1.y + height, bottom_1.z);
	Vector3 top_2(bottom_2.x, bottom_2.y + height, bottom_2.z);
	Vector3 top_3(bottom_3.x, bottom_3.y + height, bottom_3.z);
	Vector3 top_4(bottom_4.x, bottom_4.y + height, bottom_4.z);

	bottom_1 = MVP(bottom_1, camera);
	bottom_2 = MVP(bottom_2, camera);
	bottom_3 = MVP(bottom_3, camera);
	bottom_4 = MVP(bottom_4, camera);
	top_1 = MVP(top_1, camera);
	top_2 = MVP(top_2, camera);
	top_3 = MVP(top_3, camera);
	top_4 = MVP(top_4, camera);

	DrawLine(hdc, bottom_1, bottom_2);
	DrawLine(hdc, bottom_2, bottom_3);
	DrawLine(hdc, bottom_3, bottom_4);
	DrawLine(hdc, bottom_4, bottom_1);
	DrawLine(hdc, top_1, top_2);
	DrawLine(hdc, top_2, top_3);
	DrawLine(hdc, top_3, top_4);
	DrawLine(hdc, top_4, top_1);
	DrawLine(hdc, top_1, bottom_1);
	DrawLine(hdc, top_2, bottom_2);
	DrawLine(hdc, top_3, bottom_3);
	DrawLine(hdc, top_4, bottom_4);

	TextOut(hdc, bottom_1.x, bottom_1.y, TEXT("1"), 1);
	TextOut(hdc, bottom_2.x, bottom_2.y, TEXT("2"), 1);
	TextOut(hdc, bottom_3.x, bottom_3.y, TEXT("3"), 1);
	TextOut(hdc, bottom_4.x, bottom_4.y, TEXT("4"), 1);
	TextOut(hdc, top_1.x, top_1.y, TEXT("a"), 1);
	TextOut(hdc, top_2.x, top_2.y, TEXT("b"), 1);
	TextOut(hdc, top_3.x, top_3.y, TEXT("c"), 1);
	TextOut(hdc, top_4.x, top_4.y, TEXT("d"), 1);

}


void tick() {}

void render() {}