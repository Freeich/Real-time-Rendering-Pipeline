// WindowDemo.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "WindowDemo.h"
#include <iostream>
#include <cmath>
#include "Tools.h"
#include <windowsx.h>
#include <chrono>


#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

Camera camera = Camera(); // 创建全局相机
float translate_step = 10.1f;
float roate_step = 3.f;

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    using Clock = std::chrono::high_resolution_clock;
    auto lastTime = Clock::now();  // 上一帧时间

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWDEMO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWDEMO));

    MSG msg;
    bool running = true;
    
    // Z-Buffer
    float* z_buffer = (float*)calloc(width * height, sizeof(float));

    // SwapBuffer
    uint32_t* backbuffer = new uint32_t[width * height];

    // 主消息循环: peekmessage
    while (running) {
        if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            //if (msg.message == WM_QUIT) running = false;
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        HDC hdc = GetDC(msg.hwnd);



        // 当前帧时间
        auto currentTime = Clock::now();
        std::chrono::duration<float> elapsedTime = currentTime - lastTime;
        lastTime = currentTime;

        // 计算帧时间（秒）和 FPS
        float frame_time = elapsedTime.count();
        float fps = 1.0f / frame_time;

        //translate_step = 100.f * frame_time;
        //roate_step = 5.f * frame_time;
        

        // 输出 FPS
        std::string s = std::to_string((int)fps);
        TextOut(hdc, 1000, 100, std::wstring(s.begin(), s.end()).c_str(), 10);

        // 画立方体
        DrawCube(hdc, camera, Vector3(100.f, 100.f, -0.2f), 200.f, 200.f, 200.f, backbuffer, z_buffer);
        
        // 重置backbuffer和zbuffer
        for (int i = 0; i < width * height; i++) {
            backbuffer[i] = 0xff000000;
            z_buffer[i] = 0.f;
        }

        ReleaseDC(msg.hwnd, hdc);
    }

    // 释放堆内存
    free(z_buffer);
    delete backbuffer;
    return (int) msg.wParam;

}


//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWDEMO));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWDEMO);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    /*case WM_MOUSEMOVE:
        {
            float x = 0.f;
            float y = 0.f;

            x = GET_X_LPARAM(lParam);
            y = GET_Y_LPARAM(lParam);
            camera.pitch_ = x;
            camera.roll_ = y;
        }
        break;*/
    case WM_CHAR:
    {   
        switch (wParam)
        {
            case 'a':
                camera.location_.x -= translate_step;
                break;

            case 'd':
                camera.location_.x += translate_step;
                break;

            case 'w':
                camera.location_.z += translate_step;
                break;

            case 's':
                camera.location_.z -= translate_step;
                break;

            default:
                break;
        }
        break;
    }

    case WM_KEYDOWN:
        switch (wParam)
        {
            case VK_LEFT:
                camera.pitch_ -= roate_step;
                break;

            case VK_RIGHT:
                camera.pitch_ += roate_step;


                break;

            case VK_UP:
                camera.roll_ = camera.roll_ - roate_step;


                break;

            case VK_DOWN:
                camera.roll_ = camera.roll_ + roate_step;
 

                break;

            default:
                break;
        }


    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            //TextOut(hdc, 50, 50, L"Hello, Windows!", 15);
            //MoveToEx(hdc, 50, 50, NULL);
            //LineTo(hdc, 200, 200);

            /*DrawLine(hdc, Vector2(800, 900), Vector2(1920, 900));
            Matrix mm = Matrix();
            mm.m[1][0] = 2.f;
            mm.m[1][1] = 2.f;
            mm.m[1][2] = 2.f;
            mm.m[1][3] = 2.f;

            Matrix mm1 = Matrix();
            mm1.m[0][1] = 1.f;
            mm1.m[1][1] = 1.f;
            mm1.m[2][1] = 1.f;
            mm1.m[3][1] = 1.f;

            Matrix mm2 = mm * mm1;

            mm2.print(hdc);*/

            //DrawCube(hdc, Vector3(100.f, 100.f, -100.f), 100.f, 100.f, 100.f);


            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

