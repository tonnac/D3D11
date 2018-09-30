#pragma once
#pragma warning(disable : 4005)
#define DIRECTINPUT_VERSION 0x800

#include <windows.h>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <tchar.h>
#include <cassert>
#include <iomanip>
#include <functional>
#include <D3D11.h>
#include <D3DX11.h>
#include <DXGI.h>
#include <D3Dcompiler.h>
#include <D3DX10math.h>
#include <memory>
#include <array>

#pragma region DirectWrite
#include "D2D1.h"
#include "DWrite.h"
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma endregion

#pragma region DirectInput
#include "dinput.h"
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")
#pragma endregion

#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dx11.lib")

#pragma comment(lib,"fmod64_vc.lib")

#pragma comment(lib,"Dx_Momodora.lib")

namespace std
{
	using tstring = std::basic_string<TCHAR>;
	using tistringstream = std::basic_istringstream<TCHAR>;
	using tostringstream = std::basic_ostringstream<TCHAR>;
	using tstringstream = std::basic_stringstream<TCHAR>;

	using tofstream = std::basic_ofstream<TCHAR>;
	using tifstream = std::basic_ifstream<TCHAR>;
	using tfstream = std::basic_fstream<TCHAR>;

	template <typename K>
	inline tstring to_tstring(const K& _Val)
	{
#ifdef UNICODE
		return to_wstring(_Val);
#else
		return to_string(_Val);
#endif
	}
}

#define MAXKEYNUM 256
#define Epsilon 0.001
#define GETPTR(x) if((x) == nullptr) {return nullptr;} return (x)
#define CASTING(x,y) static_cast<x>((y))
#define RE_CASTING(x,y) reinterpret_cast<x>((y))
#define RELEASE(x) if((x)) {x->Release();} (x) = nullptr
#define DEL_REL(x) if((x)) {x->Release(); delete (x);} (x) = nullptr
#define SUBTRACT(x,y) abs((x) - (y))

using FilePathMap = std::map<std::tstring, std::tstring>;

extern HINSTANCE		g_hInstance;
extern HWND				g_hWnd;
extern RECT				g_rtClient;
extern bool				g_bActive;
extern bool				g_DebugMode;
extern D3DXMATRIX		g_mToProj;
extern FLOAT			g_fSecPerFrame;
extern FLOAT			g_fGameTime;
extern FLOAT			g_fImageWidth;
extern FLOAT			g_fImageHeight;
extern FLOAT			g_fSpeed;
extern FLOAT			g_fMapWidth;
extern FLOAT			g_fEffectVolume;
extern FLOAT			g_fBGMVolume;

class PlayerEffect;
using PlayerEffectPtr = std::shared_ptr<PlayerEffect>;
extern PlayerEffectPtr g_Attack1;
extern PlayerEffectPtr g_Attack2;
extern PlayerEffectPtr g_Attack3;
extern PlayerEffectPtr g_AirAttack;

class Player;
extern std::shared_ptr<Player> g_Player;

class Fade;
extern std::shared_ptr<Fade> g_Fade;

class Setting;
extern std::shared_ptr<Setting> g_Setting;

template <typename K>
class Singleton
{
protected:
	Singleton() = default;
public:
	static K& getInst()
	{
		static K inst;
		return inst;
	}
};

inline std::tstring AnsiToTString(const std::string& str)
{
	TCHAR buffer[512] = { 0, };
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
	return std::tstring(buffer);
}

class DxException
{
public:
	DxException() = default;
	DxException(HRESULT hr, const std::tstring& functionname, const std::tstring& filename, const int& lineNumber);

	std::tstring ToString() const;

	HRESULT ErrorCode = S_OK;
	std::tstring Functionname;
	std::tstring Filename;
	int LineNumber = -1;
};

class FileException
{
public:
	FileException() = default;
	FileException(const std::tstring& Path,const std::tstring& filename, const std::tstring function, const int& line);
private:
	std::tstring Filepath;
	std::tstring FileName;
	std::tstring Function;
	int LineNumber = -1;
};

#ifndef ThrowifFailed
#define ThrowifFailed(x)								\
{														\
	HRESULT hr__ = (x);									\
	if(FAILED(hr__))									\
	{													\
		std::tstring wfn = AnsiToTString(__FILE__);		\
		std::tstring wfc = AnsiToTString(__FUNCTION__);	\
		throw DxException(hr__, wfc, wfn, __LINE__);	\
	}													\
}
#endif

#ifndef ShaderifFailed
#define ShaderifFailed(x)							   	\
{													   	\
	HRESULT hr__ = (x);								   	\
	if(FAILED(hr__))								   	\
	{												   	\
		std::string Err = "\n\n";					   	\
		Err += (char*)pErrBlob->GetBufferPointer();	   	\
		Err += "\n\n";								   	\
		OutputDebugStringA(Err.c_str());			   	\
		std::tstring wfn = AnsiToTString(__FILE__);	   	\
		std::tstring wfc = AnsiToTString(__FUNCTION__);	\
		throw DxException(hr__, wfc, wfn, __LINE__);   	\
	}												   	\
}
#endif

#ifndef FileExceptErr
#define FileExceptErr(x,y)								\
{														\
	(x).open((y).c_str(), std::ios::in);				\
	if ((x).is_open() == false)							\
	{													\
		std::tstring wfn = AnsiToTString(__FILE__);		\
		std::tstring wfc = AnsiToTString(__FUNCTION__);	\
		throw FileException(y, wfn, wfc, __LINE__);		\
	}													\
}
#endif

#ifndef VolumeSet
#define VolumeSet(x, y)			\
{								\
	if ((x) + (y) < 0.0f)		\
	{							\
		(x) = 0.0f;				\
	}							\
	else if ((x) + (y) > 1.0f)	\
	{							\
		(x) = 1.0f;				\
	}							\
	else						\
	{							\
		(x) += (y);				\
	}							\
}						
#endif

template <typename K>
struct CacheData_
{
	std::tstring Name;
	K			 Data;
};

class Filepath
{
public:
	static bool			Init(const std::tstring& path);
	static FilePathMap	m_Txtpath;
	static FilePathMap	m_Pngpath;
	static FilePathMap	m_Sndpath;
};