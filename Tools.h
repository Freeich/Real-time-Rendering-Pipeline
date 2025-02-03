#pragma once
#include <cmath>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <unordered_set>
#define STB_IMAGE_IMPLEMENTATION  //必须加上
#include "stb_image.h"


const float PI = 3.14159265358979323846;
const int width = 900;
const int height = 900;

enum class RenderState
{
	Counterclockwise,
	Clockwise,
	RenderAll
};

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

	// 相等
	bool operator==(const Vector3& vector_sec) {
		return x == vector_sec.x and y == vector_sec.y and z == vector_sec.z;
	}

	// 不相等
	bool operator!=(const Vector3& vector_sec) {
		return !(x == vector_sec.x and y == vector_sec.y and z == vector_sec.z);
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
	Matrix PointToMatrix(){
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


struct VertexInfo
{
	float x;
	float y;
	float z;
	float w = 0.01f;
	MyColor c = MyColor();
	Vector2 uv_coor = Vector2();
	Vector3 normal_vector = Vector3();
	Vector3 world_position = Vector3();

	VertexInfo() {};
	VertexInfo(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {};

	// 手动定义赋值运算符
	VertexInfo& operator=(const VertexInfo& other) {
		if (this != &other) { // 防止自我赋值
			x = other.x;
			y = other.y;
			z = other.z;
			c = other.c;
			uv_coor = other.uv_coor;
			normal_vector = other.normal_vector;
		}
		return *this;
	}

};

struct Triangle {
	VertexInfo v1;
	VertexInfo v2;
	VertexInfo v3;

	Triangle() {};
	Triangle(VertexInfo v11, VertexInfo v22, VertexInfo v33) : v1(v11), v2(v22), v3(v33) {};
};


// DDA画线t
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
	Vector3 look_point_;
	float radius = 300.f;

	float pitch_;
	float yaw_;
	float roll_;

	Camera() : 
		location_(Vector3(0.f, 0.f, 0.f)), 
		lookat_(Vector3(0.f, 0.f, -1.f)), 
		top_(Vector3(0.f, 1.f, 0.f)),
		look_point_(Vector3(200.f, 200.f, -200.f)),
		pitch_(0.0f),
		yaw_(0.0f),
		roll_(0.0f){};

	Camera(const Vector3& location, const Vector3& lookat = Vector3(0.f, 0.f, -1.f), const Vector3& top = Vector3(0.f, 1.f, 0.f)) :
		location_(location), 
		lookat_(lookat), 
		top_(top),
		look_point_(Vector3(200.f, 200.f, -200.f)),
		pitch_(0.0f),
		yaw_(0.0f),
		roll_(0.0f) {};

	Vector3 GetLocation() { 
		return location_; 
	};

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

		//lookat_.x = lookat_matrix.m[0][0];
		//lookat_.y = lookat_matrix.m[1][0];
		//lookat_.z = lookat_matrix.m[2][0];
		Vector3 la = Vector3(lookat_matrix.m[0][0], lookat_matrix.m[1][0], lookat_matrix.m[2][0]);
		
		return la.Normalize();
	};
};


// 读取材质颜色
MyColor ReadMatiral(unsigned char* material_data, const Vector2& uv, int img_width, int img_height) {
	int channels = 3;

	float u = uv.x;
	float v = uv.y;

	int x = round(u * img_width - 1);
	int y = round(v * img_height - 1);

	int index = (y * img_width + x) * channels;
	float r = (float)material_data[index + 0];
	float g = (float)material_data[index + 1];
	float b = (float)material_data[index + 2];

	MyColor color = MyColor();
	color.r = r;
	color.g = g;
	color.b = b;

	return color;
}

// MVP算法
float MVP(Vector3& point, Camera& camera) {
	
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

	Matrix pers_proj(4, 4);

	// 投影变换
	float z_near = -1.f;
	float z_far = -500.f;

	pers_proj.m[0][0] = z_near / 1.f;
	pers_proj.m[1][1] = z_near / 1.f;
	pers_proj.m[2][2] = (z_near + z_far) / (z_far - z_near);
	pers_proj.m[2][3] = -1 * (2 * z_near * z_far) / (z_far - z_near);
	pers_proj.m[3][2] = 1.f;

	//pers_proj.m[0][0] = z_near;
	//pers_proj.m[1][1] = z_near;
	//pers_proj.m[2][2] = (z_near + z_far);
	//pers_proj.m[2][3] = -1 * (z_near * z_far);
	//pers_proj.m[3][2] = 1.f;

	point_matrix = pers_proj * point_matrix;

	point.x = point_matrix.m[0][0];
	point.y = point_matrix.m[1][0];
	point.z = point_matrix.m[2][0];


	return point_matrix.m[3][0];
}

// 视口变换
void ViewportTransform(Vector3& point) {
	Matrix point_matrix = point.PointToMatrix();

	Matrix viewport_matrix(4, 4);
	viewport_matrix.m[0][0] = width / 2;
	viewport_matrix.m[0][3] = width / 2;
	viewport_matrix.m[1][1] = height / 2;
	viewport_matrix.m[1][3] = height / 2;
	viewport_matrix.m[2][2] = 1;
	viewport_matrix.m[2][3] = 1;
	viewport_matrix.m[3][3] = 1;

	point_matrix = viewport_matrix * point_matrix;
	point.x = point_matrix.m[0][0];
	point.y = point_matrix.m[1][0];
	point.z = point_matrix.m[2][0];
}

// 透视除法 -> NDC
void ProjectiveDivision(Vector3& point, float w) {
	Matrix point_matrix = point.PointToMatrix();
	point_matrix.m[3][0] = w;

	if (point_matrix.m[3][0] != 0.f) {
		point_matrix.m[0][0] = point_matrix.m[0][0] / point_matrix.m[3][0];
		point_matrix.m[1][0] = point_matrix.m[1][0] / point_matrix.m[3][0];
		point_matrix.m[2][0] = point_matrix.m[2][0] / point_matrix.m[3][0];
		point_matrix.m[3][0] = point_matrix.m[3][0] / point_matrix.m[3][0];
	}

	point.x = point_matrix.m[0][0];
	point.y = point_matrix.m[1][0];
	point.z = point_matrix.m[2][0];
}

// 扫描线算法绘制单个三角形
void DrawTriangle_LinebyLine(HDC& hdc, VertexInfo p1, VertexInfo p2, VertexInfo p3, float* z_buffer, uint32_t* backbuffer) {
	
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

	for (float i = 0; i <= total_height; i++) {
		// 确定每条扫描线的起始位置
		Vector3 start = (v1 - v3) * (i / total_height) + v3;
		if (start.y > 900.f) break;
		Vector3 end;
		MyColor color_start = (c1 - c3) * (i / total_height) + c3;
		MyColor color_end;

		float t1 = 0.f;
		if (i <= down_height) {
			t1 = i / down_height;
			end = (v2 - v3) * t1 + v3;
			color_end = (c2 - c3) * t1 + c3;
		}
		else {
			t1 = (i - down_height) / up_height;
			end = (v1 - v2) * t1 + v2;
			color_end = (c1 - c2) * t1 + c2;
		}

		// 确保start在左边
		if (start.x > end.x) {
			std::swap(start, end);
			std::swap(color_start, color_end);
		}

		// 画每一条扫描线的pixel
		float total_width = end.x - start.x;
		for (float j = 0; j < total_width; j++) {
			float t2 = j / total_width;
			MyColor color = (color_end - color_start) * t2 + color_start;
			Vector3 point = (end - start) * t2 + start;
			int x = (int)point.x;
			int y = (int)point.y;
			float z = point.z;
			int key = y * width + x;
			if (x > 900.f) break;
			if (point.x < 900.f and point.x > 0.f and point.y < 900.f and point.y > 0.f) {
				if (z_buffer[key] == 0.f or (z_buffer[key] != 0.f and z < z_buffer[key])) {
					z_buffer[key] = z;
					// 存储像素信息到backbuffer
					uint32_t red = static_cast<uint32_t>(color.r);
					uint32_t green = static_cast<uint32_t>(color.g);
					uint32_t blue = static_cast<uint32_t>(color.b);
					uint32_t alpha = 0xff;
					backbuffer[key] = alpha << 24 | red << 16 | green << 8 | blue;
					//SetPixelV(hdc, x, y, RGB(color.r, color.g, color.b));
				}
				
			}
		}

	}
}


// 片元着色器：重心坐标插值
void DrawTriangle_BarycentricCoordinates(HDC& hdc, Camera& camera, VertexInfo p1, VertexInfo p2, VertexInfo p3, float* z_buffer, uint32_t* backbuffer, unsigned char* material_data, int img_width, int img_height) {

	// 光照方向
	Vector3 light_dir = Vector3(-1, 1, -1).Normalize();

	// 首先确保三个坐标顺序
	if (p1.y < p2.y) std::swap(p1, p2);
	if (p1.y < p3.y) std::swap(p1, p3);
	if (p2.y < p3.y) std::swap(p2, p3);

	Vector3 v1 = Vector3(p1.x, p1.y, p1.z);
	Vector3 v2 = Vector3(p2.x, p2.y, p2.z);
	Vector3 v3 = Vector3(p3.x, p3.y, p3.z);

	float total_height = v1.y - v3.y;
	float down_height = v2.y - v3.y;
	float up_height = v1.y - v2.y;
	
	for (float i = 0; i <= total_height; i++) {
		// 确定每条扫描线的起始位置
		Vector3 start = (v1 - v3) * (i / total_height) + v3;
		if (start.y > 900.f) break;
		Vector3 end;

		float t1 = 0.f;
		if (i <= down_height) {
			t1 = i / down_height;
			end = (v2 - v3) * t1 + v3;
		}
		else {
			t1 = (i - down_height) / up_height;
			end = (v1 - v2) * t1 + v2;
		}

		// 确保start在左边
		if (start.x > end.x) {
			std::swap(start, end);
		}

		// 画每一条扫描线的pixel
		float total_width = end.x - start.x;
		for (float j = start.x; j <= end.x; j++) {
			
			// 当前着色点的坐标
			float x = j;
			float y = i + v3.y;

			if (x > 900.f) break;

			// 重心坐标系数
			float a = (-(x - v2.x) * (v3.y - v2.y) + (y - v2.y) * (v3.x - v2.x)) / (-(v1.x - v2.x) * (v3.y - v2.y) + (v1.y - v2.y) * (v3.x - v2.x));
			float b = (-(x - v3.x) * (v1.y - v3.y) + (y - v3.y) * (v1.x - v3.x)) / (-(v2.x - v3.x) * (v1.y - v3.y) + (v2.y - v3.y) * (v1.x - v3.x));
			float c = 1 - a - b;
			
			if (not (a >= 0 and b >= 0 and c >= 0)) continue;
			
			// 进行透视矫正，并插值计算各个值
			float w1 = p1.w;
			float w2 = p2.w;
			float w3 = p3.w;
			float w_inverse = (1 / w1) * a + (1 / w2) * b + (1 / w3) * c;

			// 利用世界空间坐标计算 施加高光时需要的视角方向
			Vector3 wp1 = p1.world_position * (1 / w1);
			Vector3 wp2 = p2.world_position * (1 / w2);
			Vector3 wp3 = p3.world_position * (1 / w3);
			Vector3 wp = wp1 * a + wp2 * b + wp3 * c;
			wp = wp * (1 / w_inverse);
			Vector3 v = (wp - camera.GetLocation()).Normalize();

			// uv坐标的透视修正
			Vector2 uv1 = p1.uv_coor * (1 / w1);
			Vector2 uv2 = p2.uv_coor * (1 / w2);
			Vector2 uv3 = p3.uv_coor * (1 / w3);
			Vector2 uv = uv1 * a + uv2 * b + uv3 * c;
			uv = uv * (1 / w_inverse);

			// 法线的透视修正
			Vector3 normal1 = p1.normal_vector * (1 / w1);
			Vector3 normal2 = p2.normal_vector * (1 / w2);
			Vector3 normal3 = p3.normal_vector * (1 / w3);
			Vector3 normal = normal1 * a + normal2 * b + normal3 * c;
			normal = (normal * (1 / w_inverse)).Normalize();
			
			// 深度测试用的就是投影变换后的z值，不需要也不能透视修正
			float z_depth = p1.z * a + p2.z * b + p3.z * c;

			int key = int(y) * width + int(x);
			if (x < 900.f and x > 0.f and y < 900.f and y > 0.f) {
				if (z_buffer[key] == 0.f or (z_buffer[key] != 0.f and z_depth < z_buffer[key])) {
					z_buffer[key] = z_depth;
					
					// 读取材质贴图信息
					MyColor color = ReadMatiral(material_data, uv, img_width, img_height);

					// 施加光照: Phong-Shading, 基于像素的光照计算
					float Intensity = 1.0f;

					// 计算光照：Blinn-Phong光照模型：漫反射光 + 高光 + 环境光。
					// 使用的是平行光，即，未实现光照强度随光源距离的衰减，光照强度是写死的。
					MyColor light_diffuse = color * max(0, normal.DotProduct(light_dir * -1)) * Intensity;
					MyColor light_specular = MyColor(255, 255, 255) * pow(max(0, ((light_dir * (-1) - v).Normalize()).DotProduct(normal)), 256) * Intensity;
					MyColor light_ambient = color * 0.2f;

					color = light_diffuse + light_specular + light_ambient;
					color.r = Clamp(color.r, 0.f, 255.f);
					color.g = Clamp(color.g, 0.f, 255.f);
					color.b = Clamp(color.b, 0.f, 255.f);
					
					// 存储像素信息到backbuffer
					uint32_t red = static_cast<uint32_t>(color.r);
					uint32_t green = static_cast<uint32_t>(color.g);
					uint32_t blue = static_cast<uint32_t>(color.b);
					uint32_t alpha = 0xff;
					backbuffer[key] = alpha << 24 | red << 16 | green << 8 | blue;


					//SetPixelV(hdc, x, y, RGB(color.r, color.g, color.b));
				}

			}
		}

	}
}



// 传递三角信息
void Render(HDC& hdc, Camera& camera, const std::vector<Triangle>& triangles, uint32_t* backbuffer, float* z_buffer, unsigned char* material_data, int img_width, int img_height) {

	// 给每个三角形进行线性插值上色
	for (Triangle t : triangles) {
		DrawTriangle_BarycentricCoordinates(hdc, camera, t.v1, t.v2, t.v3, z_buffer, backbuffer, material_data, img_width, img_height);
	}

	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = -height; // 从上到下扫描
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32; // 每像素 32 位 (RGB)
	bmi.bmiHeader.biCompression = BI_RGB;

	StretchDIBits(hdc,
		0, 0, width, height,        // 绘制到目标区域
		0, 0, width, height,        // 从源区域读取
		backbuffer, &bmi,          // 像素数据和位图信息
		DIB_RGB_COLORS, SRCCOPY);
}


// 三角形背面剔除
std::vector<std::vector<int>> RemoveBackTriangles(Camera& camera, std::vector<std::vector<int>> triangles, std::vector<Vector3>& points, RenderState renderstate = RenderState::RenderAll) {
	
	Vector3 lookdir = camera.GetLookat();

	std::vector<std::vector<int>> new_triangles = std::vector<std::vector<int>>();

	for (int i = 0; i < triangles.size(); i++) {
		Vector3 b1 = points[triangles[i][1] - 1] - points[triangles[i][0] - 1];
		Vector3 b2 = points[triangles[i][2] - 1] - points[triangles[i][0] - 1];
		Vector3 n = b1.Normalize().CrossProduct(b2.Normalize()).Normalize();
		n = n * (-1); // 因为窗口的左上角才是原点，y轴被倒置，所以法线向量应该取反。

		
		switch (renderstate)
		{
		case RenderState::Counterclockwise:

			if (lookdir.DotProduct(n) < 0) {
				new_triangles.push_back(triangles[i]);
			}
			break;

		case RenderState::Clockwise:

			if (lookdir.DotProduct(n) > 0) {
				new_triangles.push_back(triangles[i]);
			}
			break;

		case RenderState::RenderAll:

			new_triangles.push_back(triangles[i]);

			break;
		default:
			break;
		}
	}
	return new_triangles;
}

// 三角形裁剪
std::vector<std::vector<int>> CuteTriangles(
	std::vector<std::vector<int>>& triangles, 
	std::vector<Vector3>& points, 
	std::vector<float>& w_value, 
	std::vector<Vector2>& uv_coordinate,
	std::vector<Vector3>& normal_vectors,
	std::vector<Vector3>& world_positions) {

	// 需要剔除的三角形序号集合
	std::unordered_set<int> cut_triangles = std::unordered_set<int>();

	// 用现在的坐标进行视锥剔除和三角形裁剪
	int triangles_num = triangles.size();
	for (int i = 0; i < triangles_num; i++) {

		int i1 = triangles[i][0];
		int i2 = triangles[i][1];
		int i3 = triangles[i][2];

		float w1 = w_value[triangles[i][0] - 1];
		float w2 = w_value[triangles[i][1] - 1];
		float w3 = w_value[triangles[i][2] - 1];

		Vector3 p1 = points[triangles[i][0] - 1];
		Vector3 p2 = points[triangles[i][1] - 1];
		Vector3 p3 = points[triangles[i][2] - 1];

		Vector2 uv1 = uv_coordinate[triangles[i][0] - 1];
		Vector2 uv2 = uv_coordinate[triangles[i][1] - 1];
		Vector2 uv3 = uv_coordinate[triangles[i][2] - 1];

		Vector3 normal1 = normal_vectors[triangles[i][0] - 1];
		Vector3 normal2 = normal_vectors[triangles[i][1] - 1];
		Vector3 normal3 = normal_vectors[triangles[i][2] - 1];

		Vector3 wp1 = world_positions[triangles[i][0] - 1];
		Vector3 wp2 = world_positions[triangles[i][1] - 1];
		Vector3 wp3 = world_positions[triangles[i][2] - 1];


		// 首先，剔除所有不在视锥体里面的

		bool f1 = (p1.x <= w1 or p1.x >= -w1) or (p1.y <= w1 or p1.y >= -w1) or (p1.z >= -w1);
		bool f2 = (p2.x <= w2 or p2.x >= -w2) or (p2.y <= w2 or p2.y >= -w2) or (p2.z >= -w2);
		bool f3 = (p3.x <= w3 or p3.x >= -w3) or (p3.y <= w3 or p3.y >= -w3) or (p3.z >= -w3);


		if (f1 and f2 and f3)
			cut_triangles.insert(i);
		else {
			// 超过近平面的三角形
			bool b1 = p1.z >= -w1;
			bool b2 = p2.z >= -w2;
			bool b3 = p3.z >= -w3;
			int result = b1 + b2 + b3;
			if (result == 0) continue;
			else {
				if (result == 1) {
					cut_triangles.insert(i);
					bool inverse = false;
					if (b2) {
						std::swap(p1, p2);
						std::swap(i1, i2);
						std::swap(w1, w2);
						std::swap(uv1, uv2);
						std::swap(normal1, normal2);
						std::swap(wp1, wp2);
						inverse = true;
					}
					if (b3) {
						std::swap(p1, p3);
						std::swap(i1, i3);
						std::swap(w1, w3);
						std::swap(uv1, uv3);
						std::swap(normal1, normal3);
						std::swap(wp1, wp3);
						inverse = true;
					}

					// 计算出交点和交点的颜色值
					float t1 = -(p1.z + w1) / ((p2.z - p1.z) + (w2 - w1));
					float t2 = -(p1.z + w1) / ((p3.z - p1.z) + (w3 - w1));

					Vector3 ins1 = p1 + (p2 - p1) * t1;
					Vector3 ins2 = p1 + (p3 - p1) * t2;

					float wins1 = w1 + (w2 - w1) * t1;
					float wins2 = w1 + (w3 - w1) * t2;

					Vector2 uv_ins1 = uv1 + (uv2 - uv1) * t1;
					Vector2 uv_ins2 = uv2 + (uv3 - uv1) * t2;

					Vector3 normal_ins1 = normal1 + (normal2 - normal1) * t1;
					Vector3 normal_ins2 = normal1 + (normal3 - normal1) * t2;

					Vector3 wp_ins1 = wp1 + (wp2 - wp1) * t1;
					Vector3 wp_ins2 = wp1 + (wp3 - wp1) * t2;

					// 将新生成的点的位置和颜色信息压入
					points.push_back(ins1);
					w_value.push_back(wins1);
					uv_coordinate.push_back(uv_ins1);
					normal_vectors.push_back(normal_ins1);
					world_positions.push_back(wp_ins1);
					int index1 = points.size(); // 新生成的点对应的index

					points.push_back(ins2);
					w_value.push_back(wins2);
					uv_coordinate.push_back(uv_ins2);
					normal_vectors.push_back(normal_ins2);
					world_positions.push_back(wp_ins2);
					int index2 = points.size(); // 新生成的点对应的index

					// 将新三角形塞入待渲染列表
					if (inverse) {
						triangles.push_back({ i3, i2, index1 });
						triangles.push_back({ i3, index1, index2 });
					}
					else {
						triangles.push_back({ i2, i3, index1 });
						triangles.push_back({ i3, index2, index1 });
					}
					//triangles.push_back({ i2, i3, index1 });
					//triangles.push_back({ i3, index2, index1 });
				}
				else if (result == 2) {
					cut_triangles.insert(i);
					bool inverse = false;
					if (not b2) {
						std::swap(p1, p2);
						std::swap(i1, i2);
						std::swap(w1, w2);
						std::swap(uv1, uv2);
						std::swap(normal1, normal2);
						std::swap(wp1, wp2);
						inverse = true;
					}
					if (not b3) {
						std::swap(p1, p3);
						std::swap(i1, i3);
						std::swap(w1, w3);
						std::swap(uv1, uv3);
						std::swap(normal1, normal3);
						std::swap(wp1, wp3);
						inverse = true;
					}

					// 计算出交点和交点的颜色值
					float t1 = -(p1.z + w1) / ((p2.z - p1.z) + (w2 - w1));
					float t2 = -(p1.z + w1) / ((p3.z - p1.z) + (w3 - w1));

					Vector3 ins1 = p1 + (p2 - p1) * t1;
					Vector3 ins2 = p1 + (p3 - p1) * t2;

					float wins1 = w1 + (w2 - w1) * t1;
					float wins2 = w1 + (w3 - w1) * t2;

					Vector2 uv_ins1 = uv1 + (uv2 - uv1) * t1;
					Vector2 uv_ins2 = uv2 + (uv3 - uv1) * t2;

					Vector3 normal_ins1 = normal1 + (normal2 - normal1) * t1;
					Vector3 normal_ins2 = normal1 + (normal3 - normal1) * t2;

					Vector3 wp_ins1 = wp1 + (wp2 - wp1) * t1;
					Vector3 wp_ins2 = wp1 + (wp3 - wp1) * t2;

					// 将新生成的点的位置和颜色信息压入
					points.push_back(ins1);
					w_value.push_back(wins1);
					uv_coordinate.push_back(uv_ins1);
					normal_vectors.push_back(normal_ins1);
					world_positions.push_back(wp_ins1);
					int index1 = points.size(); // 新生成的点对应的index

					points.push_back(ins2);
					w_value.push_back(wins2);
					uv_coordinate.push_back(uv_ins2);
					normal_vectors.push_back(normal_ins2);
					world_positions.push_back(wp_ins2);
					int index2 = points.size(); // 新生成的点对应的index

					// 将新三角形塞入待渲染列表
					if (inverse) {
						triangles.push_back({ i1, index2, index1 });
					}
					else {
						triangles.push_back({ i1, index1, index2 });
					}
				}
			}
		}
	}

	std::vector<std::vector<int>> new_triangles = std::vector<std::vector<int>>();

	// 剔除不需要的三角形
	for (int i = 0; i < triangles.size(); i++) {
		if (cut_triangles.count(i) == 0) new_triangles.push_back(triangles[i]);
	}

	return new_triangles;
}


// 画立方体
void RenderCube(HDC& hdc, Camera& camera, const Vector3& start, float length, float width, float height, uint32_t* backbuffer, float* z_buffer, Vector3& light_dir, unsigned char* material_data, int img_width, int img_height) {
	
	std::vector<std::vector<int>> triangles = {
	{1, 2, 3},
	{1, 3, 4},
	{2, 7, 3},
	{2, 6, 7},
	{4, 3, 7},
	{4, 7, 8},
	{1, 4, 8},
	{1, 8, 5},
	{1, 6, 2},
	{1, 5, 6},
	{5, 7, 6},
	{5, 8, 7} };
	
	Vector3 bottom_1 = start;
	Vector3 bottom_2(bottom_1.x + length, bottom_1.y, bottom_1.z);
	Vector3 bottom_3(bottom_1.x + length, bottom_1.y, bottom_1.z - width);
	Vector3 bottom_4(bottom_1.x, bottom_1.y, bottom_1.z - width);
	Vector3 top_1(bottom_1.x, bottom_1.y + height, bottom_1.z);
	Vector3 top_2(bottom_2.x, bottom_2.y + height, bottom_2.z);
	Vector3 top_3(bottom_3.x, bottom_3.y + height, bottom_3.z);
	Vector3 top_4(bottom_4.x, bottom_4.y + height, bottom_4.z);
	
	std::vector<Vector3> points = { bottom_1, bottom_2, bottom_3, bottom_4, top_1, top_2, top_3, top_4 };
	
	std::vector<Vector3> world_positions = { bottom_1, bottom_2, bottom_3, bottom_4, top_1, top_2, top_3, top_4 };

	std::vector<float> w_value = {};
	
	std::vector<Vector2> uv_coordinate{
		{0.001f, 0.001f},
		{0.333f, 0.001f},
		{0.666f, 0.001f},
		{0.999f, 0.001f},
		{0.001f, 0.333f},
		{0.333f, 0.333f},
		{0.666f, 0.333f},
		{0.999f, 0.333f}
	};
	
	std::vector<MyColor> colors{
							MyColor(255.f, 255.f, 255.f),
							MyColor(255.f, 255.f, 0.f),
							MyColor(255.f, 0.f, 0.f),
							MyColor(255.f, 0.f, 255.f),
							MyColor(0.f, 255.f, 255.f),
							MyColor(0.f, 255.f, 0.f),
							MyColor(0.f, 0.f, 0.f),
							MyColor(0.f, 0.f, 255.f) };
	
	std::vector<Vector3> normal_vectors = std::vector<Vector3>(8);

	for (int i = 0; i < points.size(); i++) {
		std::vector<int> relative_triagnles = std::vector<int>();
		for (int j = 0; j < triangles.size(); j++) {
			// 如果这个三角形包含这个顶点
			if (triangles[j][0] == i + 1 or triangles[j][1] == i + 1 or triangles[j][2] == i + 1) {
				relative_triagnles.push_back(j);
			}
		}

		std::vector<std::pair<Vector3, float>> normals = std::vector<std::pair<Vector3, float>>();
		float area_all = 0.f;
		for (int index : relative_triagnles) {
			Vector3 b1 = points[triangles[index][1] - 1] - points[triangles[index][0] - 1];
			Vector3 b2 = points[triangles[index][2] - 1] - points[triangles[index][0] - 1];
			Vector3 n1 = b1.Normalize().CrossProduct(b2.Normalize()).Normalize();
			n1 = n1 * (-1);

			// 计算三角形的面积，通过面积算顶点法线的加权平均值
			float area = b1.CrossProduct(b2).length() / 2;
			area_all = area_all + area;

			normals.push_back(std::make_pair(n1, area));
		}

		if (area_all == 0) continue;

		// 计算顶点的加权法线
		Vector3 normal_avg = Vector3();
		for (std::pair<Vector3, float>& n : normals) {
			normal_avg = normal_avg + n.first * (n.second / area_all);
		}

		normal_avg = normal_avg.Normalize();

		normal_vectors[i] = normal_avg;
	}

	// MVP变换
	for (int i = 0; i < points.size(); i++) {
		w_value.push_back(MVP(points[i], camera));
	}

	//std::string s = std::to_string(points[0].x);
	//s += "-";
	////s += std::to_string(int(v1.z));
	//TextOut(hdc, 1500, 500, std::wstring(s.begin(), s.end()).c_str(), 12);
	//std::string ss = std::to_string(points[0].y);
	//ss += "-";
	////ss += std::to_string(int(v2.z));
	//TextOut(hdc, 1500, 520, std::wstring(ss.begin(), ss.end()).c_str(), 12);
	//std::string sss = std::to_string(points[0].z);
	//sss += "-";
	////sss += std::to_string(int(v3.z));
	//TextOut(hdc, 1500, 540, std::wstring(sss.begin(), sss.end()).c_str(), 12);

	// 基于视锥的三角形裁剪：只实现了基于近裁面的裁剪
	std::vector<std::vector<int>> new_triangles = std::vector<std::vector<int>>();
	new_triangles = CuteTriangles(triangles, points, w_value, uv_coordinate, normal_vectors, world_positions);
	

	// 透视除法
	for (int i = 0; i < points.size(); i++) {
		ProjectiveDivision(points[i], w_value[i]);
	}

	// 背面裁剪
	new_triangles = RemoveBackTriangles(camera, new_triangles, points, RenderState::Counterclockwise);

	// 视口变换
	for (int i = 0; i < points.size(); i++) {
		ViewportTransform(points[i]);
	}

	// 将顶点改为颜色顶点
	std::unordered_map<int, VertexInfo> vertexes = std::unordered_map<int, VertexInfo>();
	for (int i = 0; i < points.size(); i++) {
		vertexes[i + 1] = VertexInfo(points[i].x, points[i].y, points[i].z);
		vertexes[i + 1].normal_vector = normal_vectors[i];
		vertexes[i + 1].uv_coor = uv_coordinate[i];
		vertexes[i + 1].w = w_value[i];
		vertexes[i + 1].world_position = world_positions[i];
	}

	// 画边框
	//DrawLine(hdc, bottom_1, bottom_2);
	//for (int i = 0; i < new_triangles.size(); i++) {
	//	DrawLine(hdc, points[new_triangles[i][0] - 1], points[new_triangles[i][1] - 1]);
	//	DrawLine(hdc, points[new_triangles[i][1] - 1], points[new_triangles[i][2] - 1]);
	//	DrawLine(hdc, points[new_triangles[i][2] - 1], points[new_triangles[i][0] - 1]);
	//}

	//// 标记顶点
	//TextOut(hdc, points[0].x, points[0].y, TEXT("1"), 1);
	//TextOut(hdc, points[1].x, points[0].y, TEXT("2"), 1);
	//TextOut(hdc, bottom_3.x, bottom_3.y, TEXT("3"), 1);
	//TextOut(hdc, bottom_4.x, bottom_4.y, TEXT("4"), 1);
	//TextOut(hdc, top_1.x, top_1.y, TEXT("a"), 1);
	//TextOut(hdc, top_2.x, top_2.y, TEXT("b"), 1);
	//TextOut(hdc, top_3.x, top_3.y, TEXT("c"), 1);
	//TextOut(hdc, top_4.x, top_4.y, TEXT("d"), 1);
	
	std::vector<Triangle> ts = std::vector<Triangle>();

	// 更改渲染的三角形的个数.
	for (int i = 0; i < new_triangles.size(); i++) {
		Triangle t = Triangle(vertexes[new_triangles[i][0]], vertexes[new_triangles[i][1]], vertexes[new_triangles[i][2]]);
		ts.push_back(t);
	}

	// 查看渲染三角形的个数
	std::string render_info = "Number Of Triangles: ";
	render_info += std::to_string(new_triangles.size());
	TextOut(hdc, 1000, 150, std::wstring(render_info.begin(), render_info.end()).c_str(), 23);
	
	// 渲染操作
	Render(hdc, camera, ts, backbuffer, z_buffer, material_data, img_width, img_height);
}





// 画OBJ文件(尚未完成)
void Draw(HDC& hdc, Camera& camera, uint32_t* backbuffer, float* z_buffer, unsigned char* material_data, int img_width, int img_height, 
	tinyobj::attrib_t attrib, std::vector<tinyobj::shape_t> shapes) {

	// 现在的逻辑中：以下的变量都还只是顶点和索引池子，并没有实现poins，uv_coordinate, normal_vectors的一一对应
	std::vector<std::vector<int>> triangles{};

	std::vector<Vector3> points = {};

	std::vector<Vector3> world_positions = {};

	std::vector<float> w_value = {};

	std::vector<Vector2> uv_coordinate = std::vector<Vector2>();

	std::vector<Vector3> normal_vectors = std::vector<Vector3>();


	// 处理顶点数据
	for (size_t i = 0; i < attrib.vertices.size() / 3; i++) {
		Vector3 point = { attrib.vertices[3 * i + 0], attrib.vertices[3 * i + 1], attrib.vertices[3 * i + 2] };
		points.push_back(point);
		world_positions.push_back(point);
	}

	// 处理纹理坐标数据
	for (size_t i = 0; i < attrib.texcoords.size() / 2; i++) {
		Vector2 uv = { attrib.texcoords[2 * i + 0], attrib.texcoords[2 * i + 1] };
		uv_coordinate.push_back(uv);
	}

	// 处理法向量数据
	for (size_t i = 0; i < attrib.normals.size() / 3; i++) {
		Vector3 normal = { attrib.normals[3 * i + 0], attrib.normals[3 * i + 1], attrib.normals[3 * i + 2] };
		normal_vectors.push_back(normal);
	}

	// 遍历每个形状，提取三角形的顶点索引
	for (const auto& shape : shapes) {
		size_t index_offset = 0;

		for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
			int face_vertices = shape.mesh.num_face_vertices[f]; // 每个面的顶点数量（应该始终是 3）

			if (face_vertices != 3) {
				std::cerr << "Non-triangular face detected, skipping!" << std::endl;
				index_offset += face_vertices;
				continue;
			}

			std::vector<int> triangle; // 三角形的顶点索引

			for (size_t v = 0; v < 3; v++) {
				tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

				// 顶点索引
				int vertex_index = idx.vertex_index;
				triangle.push_back(vertex_index);
			}

			triangles.push_back(triangle);
			index_offset += face_vertices;
		}
	}

	
	// MVP变换
	for (int i = 0; i < points.size(); i++) {
		w_value.push_back(MVP(points[i], camera));
	}


	// 基于视锥的三角形裁剪：只实现了基于近裁面的裁剪
	std::vector<std::vector<int>> new_triangles = std::vector<std::vector<int>>();
	new_triangles = CuteTriangles(triangles, points, w_value, uv_coordinate, normal_vectors, world_positions);


	// 透视除法
	for (int i = 0; i < points.size(); i++) {
		ProjectiveDivision(points[i], w_value[i]);
	}

	// 背面裁剪
	new_triangles = RemoveBackTriangles(camera, new_triangles, points, RenderState::RenderAll);

	// 视口变换
	for (int i = 0; i < points.size(); i++) {
		ViewportTransform(points[i]);
	}

	// 将顶点改为颜色顶点
	std::vector<VertexInfo> vertexes = std::vector<VertexInfo>();
	vertexes.push_back(VertexInfo());
	for (int i = 0; i < points.size(); i++) {
		VertexInfo vertex_info = VertexInfo(points[i].x, points[i].y, points[i].z);
		vertex_info.normal_vector = normal_vectors[i];
		vertex_info.uv_coor = uv_coordinate[i];
		vertex_info.w = w_value[i];
		vertex_info.world_position = world_positions[i];
		vertexes.push_back(vertex_info);
	}

	std::vector<Triangle> ts = std::vector<Triangle>();

	// 更改渲染的三角形的个数.
	for (int i = 0; i < new_triangles.size(); i++) {
		Triangle t = Triangle(vertexes[new_triangles[i][0]], vertexes[new_triangles[i][1]], vertexes[new_triangles[i][2]]);
		ts.push_back(t);
	}

	// 查看渲染三角形的个数
	std::string render_info = "Number Of Triangles: ";
	render_info += std::to_string(new_triangles.size());
	TextOut(hdc, 1000, 150, std::wstring(render_info.begin(), render_info.end()).c_str(), 23);

	// 渲染操作
	Render(hdc, camera, ts, backbuffer, z_buffer, material_data, img_width, img_height);
}




void tick() {}






// 施加光照(废弃)------------------------------------------------废弃代码-------------------------------------------------------------------
void AddIllumination(Camera& camera, std::vector<std::vector<int>>& triangles, std::vector<Vector3>& points, std::vector<MyColor>& colors, Vector3 light_dir) {

	//Vector3 light_dir = Vector3(-1, 1, -1).Normalize();
	Vector3 lookat = camera.GetLookat().Normalize();

	for (int i = 0; i < points.size(); i++) {
		std::vector<int> relative_triagnles = std::vector<int>();
		for (int j = 0; j < triangles.size(); j++) {
			// 如果这个三角形包含这个顶点
			if (triangles[j][0] == i + 1 or triangles[j][1] == i + 1 or triangles[j][2] == i + 1) {
				relative_triagnles.push_back(j);
			}
		}

		std::vector<std::pair<Vector3, float>> normals = std::vector<std::pair<Vector3, float>>();
		float area_all = 0.f;
		for (int index : relative_triagnles) {
			Vector3 b1 = points[triangles[index][1] - 1] - points[triangles[index][0] - 1];
			Vector3 b2 = points[triangles[index][2] - 1] - points[triangles[index][0] - 1];
			Vector3 n1 = b1.Normalize().CrossProduct(b2.Normalize()).Normalize();
			n1 = n1 * (-1);

			// 计算三角形的面积，通过面积算顶点法线的加权平均值
			float area = b1.CrossProduct(b2).length() / 2;
			area_all = area_all + area;

			normals.push_back(std::make_pair(n1, area));
		}

		if (area_all == 0) continue;

		// 计算顶点的加权法线
		Vector3 normal_avg = Vector3();
		for (std::pair<Vector3, float>& n : normals) {
			normal_avg = normal_avg + n.first * (n.second / area_all);
		}

		normal_avg = normal_avg.Normalize();

		float Intensity = 2.0f;

		// 计算光照：根据光照信息重新上色。
		MyColor light_diffuse = colors[i] * max(0, normal_avg.DotProduct(light_dir * -1)) * Intensity;
		MyColor light_specular = MyColor(50, 50, 50) * std::pow(max(0, ((light_dir * (-1) - lookat).Normalize()).DotProduct(light_dir * -1)), 256) * Intensity;
		MyColor light_ambient = colors[i] * 0.2f;

		colors[i] = light_diffuse + light_specular + light_ambient;
		colors[i].r = Clamp(colors[i].r, 0.f, 255.f);
		colors[i].g = Clamp(colors[i].g, 0.f, 255.f);
		colors[i].b = Clamp(colors[i].b, 0.f, 255.f);
	}

}
//-------------------------------------------------------------废弃代码---------------------------------------------------------------------
