#pragma once
#include <cmath>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <unordered_set>

const float PI = 3.14159265358979323846;
const int width = 900;
const int height = 900;

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

		lookat_.x = lookat_matrix.m[0][0];
		lookat_.y = lookat_matrix.m[1][0];
		lookat_.z = lookat_matrix.m[2][0];
		
		return lookat_;
	};
};


//void OrthosTransform(Vector3& point, float z_near, float z_far) {
//	
//	Matrix point_matrix = point.PointToMatrix();
//
//
//
//	float xr = 0.5f;
//	float xl = -0.5f;
//	float yt = 0.5f;
//	float yb = -0.5f;
//
//	Matrix ortho_scale = Matrix();
//	ortho_scale.m[0][0] = 2.f / (xr - xl);
//	ortho_scale.m[1][1] = 2.f / (yt - yb);
//	ortho_scale.m[2][2] = 2.f / (z_near - z_far);
//	ortho_scale.m[3][3] = 1.f;
//
//	Matrix orthos_translate = Matrix();
//	orthos_translate.m[0][0] = 1.f;
//	orthos_translate.m[1][1] = 1.f;
//	orthos_translate.m[2][2] = 1.f;
//	orthos_translate.m[3][3] = 1.f;
//	orthos_translate.m[0][3] = -1 * (xr + xl) / 2.f;
//	orthos_translate.m[1][3] = -1 * (yt + yb) / 2.f;
//	orthos_translate.m[2][3] = -1 * (z_near + z_far) / 2.f;;
//
//	point_matrix = orthos_translate * point_matrix;
//	point_matrix = ortho_scale * point_matrix;
//
//	point.x = point_matrix.m[0][0];
//	point.y = point_matrix.m[1][0];
//	point.z = point_matrix.m[2][0];
//}

// MVP算法
float MVP(Vector3& point, Camera camera) {
	
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
	float z_near = -0.1f;
	float z_far = -1000.f;

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
void ViewportTransform(Vector3& point, float w) {
	Matrix point_matrix = point.PointToMatrix();
	point_matrix.m[3][0] = w;

	if (point_matrix.m[3][0] != 0.f) {
		point_matrix.m[0][0] = point_matrix.m[0][0] / point_matrix.m[3][0];
		point_matrix.m[1][0] = point_matrix.m[1][0] / point_matrix.m[3][0];
		point_matrix.m[2][0] = point_matrix.m[2][0] / point_matrix.m[3][0];
		point_matrix.m[3][0] = point_matrix.m[3][0] / point_matrix.m[3][0];
	}

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

// 扫描线算法绘制单个三角形
void DrawTriangle(HDC& hdc, ColoredVertex p1, ColoredVertex p2, ColoredVertex p3, float* z_buffer, uint32_t* backbuffer) {
	
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

	//std::string s = std::to_string(v1.z);
	//s += "-";
	////s += std::to_string(int(v1.z));
	//TextOut(hdc, 500, 500, std::wstring(s.begin(), s.end()).c_str(), 12);

	//std::string ss = std::to_string(v2.z);
	//ss += "-";
	////ss += std::to_string(int(v2.z));
	//TextOut(hdc, 500, 520, std::wstring(ss.begin(), ss.end()).c_str(), 12);

	//std::string sss = std::to_string(v3.z);
	//sss += "-";
	////sss += std::to_string(int(v3.z));
	//TextOut(hdc, 500, 540, std::wstring(sss.begin(), sss.end()).c_str(), 12);

	for (float i = 0; i <= total_height; i++) {
		// 确定每条扫描线的起始位置
		Vector3 start = (v1 - v3) * (i / total_height) + v3;
		if (start.y > 900.f) break;
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
		for (float j = 0; j <= total_width; j++) {
			MyColor color = (color_end - color_start) * (j / total_width) + color_start;
			Vector3 point = (end - start) * (j / total_width) + start;
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


// 传递三角信息
void Render(HDC& hdc, const std::vector<Triangle>& triangles, uint32_t* backbuffer, float* z_buffer) {

	// 给每个三角形进行线性插值上色
	for (Triangle t : triangles) {
		DrawTriangle(hdc, t.v1, t.v2, t.v3, z_buffer, backbuffer);
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


// 画立方体线框
void DrawCube(HDC& hdc, const Camera& camera, const Vector3& start, float length, float width, float height, uint32_t* backbuffer, float* z_buffer) {
	
	std::vector<std::vector<int>> triangles = {
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
	
	Vector3 bottom_1 = start;
	Vector3 bottom_2(bottom_1.x + length, bottom_1.y, bottom_1.z);
	Vector3 bottom_3(bottom_1.x + length, bottom_1.y, bottom_1.z - width);
	Vector3 bottom_4(bottom_1.x, bottom_1.y, bottom_1.z - width);
	Vector3 top_1(bottom_1.x, bottom_1.y + height, bottom_1.z);
	Vector3 top_2(bottom_2.x, bottom_2.y + height, bottom_2.z);
	Vector3 top_3(bottom_3.x, bottom_3.y + height, bottom_3.z);
	Vector3 top_4(bottom_4.x, bottom_4.y + height, bottom_4.z);

	// 视角变换
	float w_1 = MVP(bottom_1, camera);
	float w_2 = MVP(bottom_2, camera);
	float w_3 = MVP(bottom_3, camera);
	float w_4 = MVP(bottom_4, camera);
	float w_5 = MVP(top_1, camera);
	float w_6 = MVP(top_2, camera);
	float w_7 = MVP(top_3, camera);
	float w_8 = MVP(top_4, camera);

	std::vector<float> w_value = { w_1, w_2, w_3, w_4, w_5, w_6, w_7, w_8 };
	std::vector<Vector3> points = { bottom_1, bottom_2, bottom_3, bottom_4, top_1, top_2, top_3, top_4 };

	std::string s = std::to_string(points[0].x);
	s += "-";
	//s += std::to_string(int(v1.z));
	TextOut(hdc, 1500, 500, std::wstring(s.begin(), s.end()).c_str(), 12);

	std::string ss = std::to_string(points[0].y);
	ss += "-";
	//ss += std::to_string(int(v2.z));
	TextOut(hdc, 1500, 520, std::wstring(ss.begin(), ss.end()).c_str(), 12);

	std::string sss = std::to_string(points[0].z);
	sss += "-";
	//sss += std::to_string(int(v3.z));
	TextOut(hdc, 1500, 540, std::wstring(sss.begin(), sss.end()).c_str(), 12);

	std::vector<MyColor> colors{
								MyColor(255.f, 255.f, 255.f),
								MyColor(255.f, 255.f, 0.f),
								MyColor(255.f, 0.f, 0.f),
								MyColor(255.f, 0.f, 255.f),
								MyColor(0.f, 255.f, 255.f),
								MyColor(0.f, 255.f, 0.f),
								MyColor(0.f, 0.f, 0.f),
								MyColor(0.f, 0.f, 255.f) };
	
	// 需要剔除的三角形序号集合
	std::unordered_set<int> cute_triangles = std::unordered_set<int>();

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

		MyColor c1 = colors[triangles[i][0] - 1];
		MyColor c2 = colors[triangles[i][1] - 1];
		MyColor c3 = colors[triangles[i][2] - 1];

		// 首先，剔除所有不在视锥体里面的
		// --------------------------------------------------------------------------------------------------------------
		//bool f1 = (p1.z >= -w1);
		//bool f2 = (p2.z >= -w2);
		//bool f3 = (p3.z >= -w3);

		bool f1 = (p1.x <= w1 or p1.x >= -w1) or (p1.y <= w1 or p1.y >= -w1) or (p1.z >= -w1);
		bool f2 = (p2.x <= w2 or p2.x >= -w2) or (p2.y <= w2 or p2.y >= -w2) or (p2.z >= -w2);
		bool f3 = (p3.x <= w3 or p3.x >= -w3) or (p3.y <= w3 or p3.y >= -w3) or (p3.z >= -w3);


		if (f1 and f2 and f3) 
			cute_triangles.insert(i);
		else {
			// 超过近平面的三角形
			bool b1 = p1.z >= -w1;
			bool b2 = p2.z >= -w2;
			bool b3 = p3.z >= -w3;
			int result = b1 + b2 + b3;
			if (result == 0) continue;
			else {
				if (result == 1) {
					cute_triangles.insert(i);
					if (b2) {
						std::swap(p1, p2); 
						std::swap(i1, i2);
						std::swap(c1, c2);
						std::swap(w1, w2);
					} 
					if (b3) {
						std::swap(p1, p3); 
						std::swap(i1, i3);
						std::swap(c1, c3);
						std::swap(w1, w3);
					} 

					// 计算出交点和交点的颜色值
					float t1 = -(p1.z + w1) / ((p2.z - p1.z) + (w2 - w1));
					float t2 = -(p1.z + w1) / ((p3.z - p1.z) + (w3 - w1));

					Vector3 ins1 = p1 + (p2 - p1) * t1;
					Vector3 ins2 = p1 + (p3 - p1) * t2;

					float wins1 = w1 + (w2 - w1) * t1;
					float wins2 = w1 + (w3 - w1) * t2;

					MyColor colr1 = (c2 - c1) * t1 + c1;
					MyColor colr2 = (c3 - c1) * t2 + c1;

					// 将新生成的点的位置和颜色信息压入
					points.push_back(ins1);
					colors.push_back(colr1);
					w_value.push_back(wins1);
					int index1 = points.size(); // 新生成的点对应的index

					points.push_back(ins2);
					colors.push_back(colr2);
					w_value.push_back(wins2);
					int index2 = points.size(); // 新生成的点对应的index

					// 将新三角形塞入待渲染列表
					triangles.push_back({ i2, i3, index1 });
					triangles.push_back({ i3, index1, index2 });
				}
				else if (result == 2) {
					cute_triangles.insert(i);
					if (not b2) {
						std::swap(p1, p2);
						std::swap(i1, i2);
						std::swap(c1, c2);
						std::swap(w1, w2);
					}
					if (not b3) {
						std::swap(p1, p3);
						std::swap(i1, i3);
						std::swap(c1, c3);
						std::swap(w1, w3);
					} 
					
					// 计算出交点和交点的颜色值
					float t1 = -(p1.z + w1) / ((p2.z - p1.z) + (w2 - w1));
					float t2 = -(p1.z + w1) / ((p3.z - p1.z) + (w3 - w1));

					Vector3 ins1 = p1 + (p2 - p1) * t1;
					Vector3 ins2 = p1 + (p3 - p1) * t2;

					float wins1 = w1 + (w2 - w1) * t1;
					float wins2 = w1 + (w3 - w1) * t2;

					MyColor colr1 = (c2 - c1) * t1 + c1;
					MyColor colr2 = (c3 - c1) * t2 + c1;

					// 将新生成的点的位置和颜色信息压入
					points.push_back(ins1);
					colors.push_back(colr1);
					w_value.push_back(wins1);
					int index1 = points.size(); // 新生成的点对应的index

					points.push_back(ins2);
					colors.push_back(colr2);
					w_value.push_back(wins2);
					int index2 = points.size(); // 新生成的点对应的index

					// 将新三角形塞入待渲染列表
					triangles.push_back({ i1, index1, index2 });
				}
			}
		}
	}

	std::vector<std::vector<int>> new_triangles = std::vector<std::vector<int>>();

	// 剔除不需要的三角形
	for (int i = 0; i < triangles.size();i++) {
		if (cute_triangles.count(i) == 0) new_triangles.push_back(triangles[i]);
	}

	std::unordered_map<int, ColoredVertex> vertexes = std::unordered_map<int, ColoredVertex>();

	// 视口变换
	for (int i = 0; i < points.size(); i++) {
		ViewportTransform(points[i], w_value[i]);
		vertexes[i + 1] = ColoredVertex(points[i].x, points[i].y, points[i].z, colors[i]);
	}

	// 将顶点改为颜色顶点
	for (int i = 0; i < points.size(); i++) {
		vertexes[i + 1] = ColoredVertex(points[i].x, points[i].y, points[i].z, colors[i]);
	}

	// 画边框
	//DrawLine(hdc, bottom_1, bottom_2);
	for (int i = 0; i < new_triangles.size(); i++) {
		DrawLine(hdc, points[new_triangles[i][0] - 1], points[new_triangles[i][1] - 1]);
		DrawLine(hdc, points[new_triangles[i][1] - 1], points[new_triangles[i][2] - 1]);
		DrawLine(hdc, points[new_triangles[i][2] - 1], points[new_triangles[i][0] - 1]);
	}

	//// 标记顶点
	TextOut(hdc, points[0].x, points[0].y, TEXT("1"), 1);
	TextOut(hdc, points[1].x, points[0].y, TEXT("2"), 1);
	TextOut(hdc, bottom_3.x, bottom_3.y, TEXT("3"), 1);
	TextOut(hdc, bottom_4.x, bottom_4.y, TEXT("4"), 1);
	TextOut(hdc, top_1.x, top_1.y, TEXT("a"), 1);
	TextOut(hdc, top_2.x, top_2.y, TEXT("b"), 1);
	TextOut(hdc, top_3.x, top_3.y, TEXT("c"), 1);
	TextOut(hdc, top_4.x, top_4.y, TEXT("d"), 1);
	
	std::vector<Triangle> ts = std::vector<Triangle>();

	// 更改渲染的三角形的个数.
	for (int i = 0; i < new_triangles.size(); i++) {
		Triangle t = Triangle(vertexes[new_triangles[i][0]], vertexes[new_triangles[i][1]], vertexes[new_triangles[i][2]]);
		ts.push_back(t);
	}
	
	// 渲染操作
	Render(hdc, ts, backbuffer, z_buffer);
}

void tick() {}