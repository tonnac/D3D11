#pragma once
#include <windows.h>
#include <cassert>
#include <tchar.h>
#include <D3D11.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <iomanip>
#include <DirectXMath.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"legacy_stdio_definitions.lib")
#pragma comment(lib,"3DCore.lib")

namespace std
{
	using tstring = std::basic_string<TCHAR>;

	using tofstream = std::basic_ifstream<TCHAR>;
	using tifstream = std::basic_ofstream<TCHAR>;
	using tfstream = std::basic_fstream<TCHAR>;

	using tsstream = std::basic_stringstream<TCHAR>;
}

extern HWND		g_hWnd;
extern RECT		g_rtClient;
extern BOOL		g_bActiveApp;
extern FLOAT	g_fSecPerFrame;