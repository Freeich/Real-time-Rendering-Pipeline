#pragma once
#include <cmath>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <unordered_map>

const float PI = 3.14159265358979323846;

float Clamp(float value, float min, float max) {
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

struct MyColor {
	float r;
	float g;
	float b;

	MyColor() : r(0.f), g(0.f), b(0.f) {}
	MyColor(float rr, float gg, float bb) : r(rr), g(gg), b(bb) {}


	MyColor operator-(MyColor& color_sec) {
		float rn = r - color_sec.r;
		float gn = g - color_sec.g;
		float bn = b - color_sec.b;
		return MyColor(rn, gn, bn);
	}

	MyColor operator+(MyColor& color_sec) {
		float rn = r + color_sec.r;
		float gn = g + color_sec.g;
		float bn = b + color_sec.b;
		return MyColor(rn, gn, bn);
	}

	MyColor operator*(float k) {
		float rn = r * k;
		float gn = g * k;
		float bn = b * k;
		return MyColor(rn, gn, bn);
	}

	MyColor& operator=(const MyColor& other){
		if (this != &other) { // 防止自我赋值
			r = other.r;
			g = other.g;
			b = other.b;
		}
		return *this;
	}

};
struct ColoredVertex
{
	float x;
	float y;
	float z;
	MyColor c = MyColor();

	ColoredVertex() {};
	ColoredVertex(float xx, float yy, float zz, MyColor cc): x(xx), y(yy), z(zz), c(cc) {};

	// 手动定义赋值运算符
	ColoredVertex& operator=(const ColoredVertex& other) {
		if (this != &other) { // 防止自我赋值
			x = other.x;
			y = other.y;
			z = other.z;
			c = other.c;
		}
		return *this;
	}

};



struct Triangle {
	ColoredVertex v1;
	ColoredVertex v2;
	ColoredVertex v3;

	Triangle() {};
	Triangle(ColoredVertex v11, ColoredVertex v22, ColoredVertex v33) : v1(v11), v2(v22), v3(v33) {};
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

	int key() {
		/*int flag = x - y < 0 ? -1 : 1;*/
		return ((int)x) * 1000  + ((int)(y));
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
	Vector2(float x_in, float y_in) :x(x_in), y(y_in) {}
	Vector2(const Vector2& v) :x(v.x), y(v.y){};


	// 加法
	Vector2 operator+(const Vector2& vector_sec) {
		return Vector2(x + vector_sec.x, y + vector_sec.y);
	}
	// 减法
	Vector2 operator-(const Vector2& vector_sec) {
		return Vector2(x - vector_sec.x, y - vector_sec.y);
	}

	bool operator==(const Vector2& vector_sec) const {
		return x == vector_sec.x && y == vector_sec.y;
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
		if (round(x) < 800.f and round(x) > 0.f and round(y) < 450.f and round(y) > 0.f) {
			SetPixelV(hdc, round(x), round(y), RGB(255, 0, 0));
		}
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

	Vector3 r = g.CrossProduct(top).Normalize();

	// 矫正相机top向量
	Vector3 t = r.CrossProduct(g).Normalize();

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
	float z_near = -0.3f;
	float z_far = -500.f;

	Matrix pers_proj(4, 4);

	pers_proj.m[0][0] = z_near / 1.f;
	pers_proj.m[1][1] = z_near / 1.f;
	pers_proj.m[2][2] = (z_near + z_far) / (z_far - z_near);
	pers_proj.m[2][3] = -1 * (2 * z_near * z_far) / (z_far - z_near);
	pers_proj.m[3][2] = 1.f;



	/*pers_proj.m[0][0] = z_near;
	pers_proj.m[1][1] = z_near;
	pers_proj.m[2][2] = (z_near + z_far);
	pers_proj.m[2][3] = -1 * (z_near * z_far);
	pers_proj.m[3][2] = 1.f;*/

	point_matrix = pers_proj * point_matrix;

	/*float xr = 2.f;
	float xl = -2.f;
	float yt = 1.f;
	float yb = -1.f;

	Matrix ortho_scale = Matrix();
	ortho_scale.m[0][0] = 2.f / (xr - xl);
	ortho_scale.m[1][1] = 2.f / (yt - yb);
	ortho_scale.m[2][2] = 2.f / (z_near - z_far);
	ortho_scale.m[3][3] = 1.f;

	Matrix orthos_translate = Matrix();
	orthos_translate.m[0][0] = 1.f;
	orthos_translate.m[1][1] = 1.f;
	orthos_translate.m[2][2] = 1.f;
	orthos_translate.m[3][3] = 1.f;
	orthos_translate.m[0][3] = -1 * (xr + xl) / 2.f;
	orthos_translate.m[1][3] = -1 * (yt + yb) / 2.f;
	orthos_translate.m[2][3] = -1 * (z_near + z_far) / 2.f;

	point_matrix = orthos_translate * point_matrix;
	point_matrix = ortho_scale * point_matrix;*/

	// 裁剪
	if (point_matrix.m[3][0] != 0.f) {
		point_matrix.m[0][0] = point_matrix.m[0][0] / point_matrix.m[3][0];
		point_matrix.m[1][0] = point_matrix.m[1][0] / point_matrix.m[3][0];
		point_matrix.m[2][0] = point_matrix.m[2][0] / point_matrix.m[3][0];
		point_matrix.m[3][0] = point_matrix.m[3][0] / point_matrix.m[3][0];
	}

	// 视口变换
	float width = 800.f;
	float height = 450.f;
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

// 扫描线算法绘制单个三角形
void DrawTriangle(HDC& hdc, ColoredVertex p1, ColoredVertex p2, ColoredVertex p3, std::unordered_map<int, float>& z_buffer, const Camera& camera) {
	
	// 保护操作

	
	// 首先确保三个坐标顺序
	if (p1.y < p2.y) std::swap(p1, p2);
	if (p1.y < p3.y) std::swap(p1, p3);
	if (p2.y < p3.y) std::swap(p2, p3);
	

	Vector3 v1 = Vector3(p1.x, p1.y, p1.z);
	Vector3 v2 = Vector3(p2.x, p2.y, p2.z);
	Vector3 v3 = Vector3(p3.x, p3.y, p3.z);
	MyColor c1 = p1.c;
	MyColor c2 = p2.c;
	MyColor c3 = p3.c;

	float total_height = v1.y - v3.y;
	float down_height = v2.y - v3.y;
	float up_height = v1.y - v2.y;

	//std::string s = std::to_string(int(v1.x));
	//s += "-";
	//s += std::to_string(int(v1.y));
	//TextOut(hdc, 500, 500, std::wstring(s.begin(), s.end()).c_str(), 12);

	//std::string ss = std::to_string(int(v2.x));
	//ss += "-";
	//ss += std::to_string(int(v2.y));
	//TextOut(hdc, 500, 520, std::wstring(ss.begin(), ss.end()).c_str(), 12);

	//std::string sss = std::to_string(int(v3.x));
	//sss += "-";
	//sss += std::to_string(int(v3.y));
	//TextOut(hdc, 500, 540, std::wstring(sss.begin(), sss.end()).c_str(), 12);

	for (float i = 0; i < total_height; i++) {

		// 确定每条扫描线的起始位置
		Vector3 start = (v1 - v3) * (i / total_height) + v3;
		Vector3 end;
		MyColor color_start = (c1 - c3) * (i / total_height) + c3;
		MyColor color_end;

		if (i <= down_height) {
			end = (v2 - v3) * (i / down_height) + v3;
			color_end = (c2 - c3) * (i / down_height) + c3;
		}
		else {
			end = (v1 - v2) * ((i - down_height) / up_height) + v2;
			color_end = (c1 - c2) * ((i - down_height) / up_height) + c2;
		}

		// 确保start在左边
		if (start.x > end.x) {
			std::swap(start, end);
			std::swap(color_start, color_end);
		}

		// 画每一条扫描线的pixel
		float total_width = end.x - start.x;
		for (float j = 0; j < total_width; j++) {
			MyColor color = (color_end - color_start) * (j / total_width) + color_start;
			Vector3 point = (end - start) * (j / total_width) + start;
			int key = point.key();
			if (point.x < 800.f and point.x > 0.f and point.y < 450.f and point.y > 0.f) {
				if (not z_buffer.count(key) or (z_buffer.count(key) and point.z < z_buffer[key])) {
					z_buffer[key] = point.z;
					SetPixelV(hdc, point.x, point.y, RGB(color.r, color.g, color.b));
				}
				
			}
		}

	}
}


// 传递三角信息
void Render(HDC& hdc, const std::vector<Triangle>& triangles, const Camera& camera) {

	std::unordered_map<int, float> z_buffer = std::unordered_map<int, float>();

	// 给每个三角形进行线性插值上色
	for (Triangle t : triangles) {
		DrawTriangle(hdc, t.v1, t.v2, t.v3, z_buffer, camera);
	}
}


// 画立方体线框
void DrawCube(HDC& hdc, const Camera& camera, const Vector3& start, float length, float width, float height) {
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

	std::map<int, ColoredVertex> vertexes{
									{1, ColoredVertex(bottom_1.x, bottom_1.y, bottom_1.z, MyColor(255.f, 255.f, 255.f))},
									{2, ColoredVertex(bottom_2.x, bottom_2.y, bottom_2.z, MyColor(255.f, 255.f, 0.f))},
									{3, ColoredVertex(bottom_3.x, bottom_3.y, bottom_3.z, MyColor(255.f, 0.f, 0.f))},
									{4, ColoredVertex(bottom_4.x, bottom_4.y, bottom_4.z, MyColor(255.f, 0.f, 255.f))},
									{5, ColoredVertex(top_1.x, top_1.y, top_1.z, MyColor(0.f, 255.f, 255.f))},
									{6, ColoredVertex(top_2.x, top_2.y, top_2.z, MyColor(0.f, 255.f, 0.f))},
									{7, ColoredVertex(top_3.x, top_3.y, top_3.z, MyColor(0.f, 0.f, 0.f))},
									{8, ColoredVertex(top_4.x, top_4.y, top_4.z, MyColor(0.f, 0.f, 255.f))}};

	std::vector<std::vector<int>> triangle_indexes = {
		{1, 2, 3},
		{1, 4, 3},
		{2, 3, 7},
		{2, 6, 7},
		{4, 3, 7},
		{4, 8, 7},
		{1, 4, 8},
		{1, 5, 8},
		{1, 2, 6},
		{1, 5, 6},
		{5, 6, 7},
		{5, 8, 7} };


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
	
	std::vector<Triangle> ts = std::vector<Triangle>();

	// 更改渲染的三角形的个数.
	for (int i = 0; i < 4; i++) {
		Triangle t = Triangle(vertexes[triangle_indexes[i][0]], vertexes[triangle_indexes[i][1]], vertexes[triangle_indexes[i][2]]);
		ts.push_back(t);
	}
	
	Render(hdc, ts, camera);
}

void tick() {}