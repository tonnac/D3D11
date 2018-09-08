#pragma once
#include <windows.h>
#include <cassert>
#include <tchar.h>
#include <D3D11.h>
#include <DXGI.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#define VOIDPTR(x) (LPVOID*)&(x)
#define GETPTR(x) if((x) == nullptr) {return nullptr;} return (x)
#define CASTING(x, y) static_cast<x>((y)) 

extern HINSTANCE g_hInstance;
extern HWND		 g_hWnd;
extern RECT		 g_rtClient;
