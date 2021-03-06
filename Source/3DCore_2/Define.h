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
#include <functional>

#pragma comment(lib, "3DCore_2.lib")

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

// Direct Write
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

// Direct Input
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#define DIRECTINPUT_VERSION 0x800

enum class KEYSTATE : unsigned char
{
	KEY_FREE,
	KEY_PUSH,
	KEY_HOLD,
	KEY_UP
};
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

#define MAXKEYNUM 256
#define VOIDPTR(x) (LPVOID*)&(x)
#define GETPTR(x) if((x) == nullptr) {return nullptr;} return (x)
#define CASTING(x, y) static_cast<x>((y)) 
#define RE_CASTING(x, y) reinterpret_cast<x>((y))
#define RELEASE(x) if((x)) {(x)->Release();} (x) = nullptr
#define DXFAIL(x) hr = (x); if(FAILED(hr)) {return false;}


extern HINSTANCE g_hInstance;
extern HWND		 g_hWnd;
extern RECT		 g_rtClient;
extern FLOAT	 g_fSecPerFrame;
extern FLOAT	 g_fGameTime;
extern bool		 g_bActive;

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


inline std::tstring AnsiToWString(const std::string& str)
{
	TCHAR buffer[512] = { 0, };
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
	return std::tstring(buffer);
}

class DxException
{
public:
	DxException() = default;
	DxException(HRESULT hr, const std::tstring& functionName, const std::tstring& fileName, int lineNumber);

	std::tstring ToString() const;

	HRESULT ErrorCode = S_OK;
	std::tstring FunctionName;
	std::tstring Filename;
	int LineNumber = -1;
};

#ifndef ThrowifFailed
#define ThrowifFailed(x)																\
{																						\
	HRESULT hr__ = (x);																	\
	if (FAILED(hr__))																	\
	{																					\
		std::tstring wfn = AnsiToWString(__FILE__);										\
		throw DxException(hr__, L#x, wfn, __LINE__);									\
	}																					\
}
#endif

#ifndef ifShaderFailed
#define ifShaderFailed(x)																\
{																						\
	HRESULT hr__ = (x);																	\
	if(FAILED(hr__))																	\
	{																					\
		std::string Error = "\n\n\n";													\
		Error += (char*)pErrBlob->GetBufferPointer();									\
		Error += "\n\n\n";																\
		OutputDebugStringA((LPCSTR)Error.c_str());										\
		std::tstring wfn = AnsiToWString(__FILE__);										\
		throw DxException(hr__, L#x, wfn, __LINE__);									\
	}																					\
}																						
#endif