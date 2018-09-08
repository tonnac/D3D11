#pragma once
#include <windows.h>
#include <cassert>
#include <tchar.h>
#include <D3D11.h>
#include <DXGI.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <iomanip>
#include <cmath>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

namespace std
{
	using tstring = std::basic_string<TCHAR>;

	using tistringstream = std::basic_istringstream<TCHAR>;
	using tostringstream = std::basic_ostringstream<TCHAR>;
	using tstringstream = std::basic_stringstream<TCHAR>;

	using tofstream = std::basic_ofstream<TCHAR>;
	using tifstream = std::basic_ofstream<TCHAR>;
	using tfstream = std::basic_fstream<TCHAR>;

	template <typename K>
	inline tstring to_tstring(K _Val)
	{
#ifdef UNICODE
		return to_wstring(_Val);
#else
		return to_string(_Val);
#endif
	}
}

#define VOIDPTR(x) (LPVOID*)&(x)
#define GETPTR(x) if((x) == nullptr) {return nullptr;} return (x)
#define CASTING(x, y) static_cast<x>((y)) 
#define RELEASE(x) if((x)) {(x)->Release();} (x) = nullptr
#define DXFAIL(x) hr = (x); if(FAILED(hr)) {return false;}

extern HINSTANCE g_hInstance;
extern HWND		 g_hWnd;
extern RECT		 g_rtClient;
extern FLOAT	 g_fSecPerFrame;


template <class K>
class Singleton
{
protected:
	Singleton() {}
public:
	static K& getInst()
	{
		static K inst;
		return inst;
	}
};