#pragma once
#pragma warning(disable : 4005)
#define DIRECTINPUT_VERSION 0x800

#include <windows.h>
#include <wrl.h>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <tchar.h>
#include <cassert>
#include <iomanip>
#include <array>
#include <cstdint>
#include <functional>
#include <D3D11.h>
#include <D3DX11.h>
#include <DXGI.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>

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

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"D3DLib.lib")

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

#define Casting(x,y) static_cast<x>((y))
#define RE_Casting(x,y) reinterpret_cast<x>((y))
#define ReleaseCom(x) if((x)) {x->Release();} (x) = nullptr

extern HINSTANCE g_hInstance;
extern HWND		 g_hWnd;
extern int		 g_ClientWidth;
extern int		 g_ClientHeight;
extern FLOAT	 g_fSecPerFrame;
extern FLOAT	 g_fGameTime;

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

class d3dUtil
{
public:
	static void CreateVertexBuffer(
		ID3D11Device* pDevice,
		UINT iNumVertex,
		UINT iVertexSize,
		LPVOID pData,
		ID3D11Buffer** ppBuffer);

	static void CreateIndexBuffer(
		ID3D11Device* pDevice,
		UINT iNumCount,
		UINT iIndexSize,
		LPVOID pData,
		ID3D11Buffer** ppBuffer);

	static void CreateConstantBuffer(
		ID3D11Device* pDevice,
		UINT iNumCount,
		UINT iIndexSize,
		ID3D11Buffer** ppBuffer,
		LPVOID pData = nullptr,
		bool bDynamic = false);

	static void CreateInputLayout(
		ID3D11Device* pDevice,
		DWORD dwSize,
		LPCVOID lpData,
		D3D11_INPUT_ELEMENT_DESC* layout,
		UINT numElements,
		ID3D11InputLayout ** ppInputLayout);

	static void LoadVertexShaderFile(
		ID3D11Device* pDevice,
		const void* pShaderFile,
		ID3D11VertexShader** pVertexShader,
		const char * pFuncName,
		ID3DBlob** ppBlobOut);

	static void LoadPixelShaderFile(
		ID3D11Device* pDevice,
		const void* pShaderFile,
		ID3D11PixelShader** pPixelShader,
		const char * pFuncName = "PS");

	static void LoadGeometryShaderFile(
		ID3D11Device* pDevice,
		const void* pShaderFile,
		ID3D11GeometryShader** pGeometryShader,
		const char * pFuncName = "GS");

	static ID3DBlob* CompileShaderFromFile(
		const WCHAR* szFileName,
		LPCSTR szEntryPoint,
		LPCSTR szShaderModel);

	static void CreateShaderResourceView(ID3D11Device* pDevice, const std::tstring& filepath, ID3D11ShaderResourceView** pShaderResource);
};

#ifndef ThrowifFailed
#define ThrowifFailed(x)											\
{																	\
	HRESULT hr__ = (x);												\
	if(FAILED(hr__))												\
	{																\
		std::tstring wfn = AnsiToTString(__FILE__);					\
		std::tstring wfc = AnsiToTString(__FUNCTION__);				\
		throw DxException(hr__, wfc, wfn, __LINE__);				\
	}																\
}
#endif

template <typename K>
struct CacheData_
{
	std::tstring Name;
	K			 Data;
};
