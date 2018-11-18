#include "d3dUtil.h"
#include <comdef.h>

using Microsoft::WRL::ComPtr;

DxException::DxException(HRESULT hr, const std::tstring& functionname, const std::tstring& filename, const int& lineNumber)
	: ErrorCode(hr), Functionname(functionname), LineNumber(lineNumber)
{
	Filename = filename.substr(filename.find_last_of('\\') + 1, filename.length() - (filename.find_last_of(L".cpp") + 4));
}

std::tstring DxException::ToString() const
{
	_com_error err(ErrorCode);
	std::tstring msg = err.ErrorMessage();

	return  L"Failed: " + Functionname + L"\nFile: " + Filename + L"\nLine: " + std::to_tstring(LineNumber) + L"\nError: " + msg;
}

void d3dUtil::CreateVertexBuffer(
	ID3D11Device* pDevice,
	UINT iNumVertex,
	UINT iVertexSize,
	LPVOID pData,
	ID3D11Buffer** ppBuffer)
{
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	bd.ByteWidth = iVertexSize * iNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	sd.pSysMem = pData;

	ThrowifFailed(pDevice->CreateBuffer(&bd, &sd, ppBuffer));
}

void d3dUtil::CreateIndexBuffer(
	ID3D11Device* pDevice,
	UINT iNumCount,
	UINT iIndexSize,
	LPVOID pData,
	ID3D11Buffer** ppBuffer)
{
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	bd.ByteWidth = iNumCount * iIndexSize;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	sd.pSysMem = pData;

	ThrowifFailed(pDevice->CreateBuffer(&bd, &sd, ppBuffer));
}

void d3dUtil::CreateConstantBuffer(
	ID3D11Device* pDevice,
	UINT iNumCount,
	UINT iIndexSize,
	ID3D11Buffer** ppBuffer,
	LPVOID pData,
	bool bDynamic)
{
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));

	if (!bDynamic)
	{
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
	}
	else
	{
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	bd.ByteWidth = iNumCount * iIndexSize;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	if (pData != nullptr)
	{
		sd.pSysMem = pData;
		ThrowifFailed(pDevice->CreateBuffer(&bd, &sd, ppBuffer));
	}
	else
	{
		ThrowifFailed(pDevice->CreateBuffer(&bd, nullptr, ppBuffer));
	}
}

void d3dUtil::CreateInputLayout(
	ID3D11Device* pDevice,
	DWORD dwSize,
	LPCVOID lpData,
	D3D11_INPUT_ELEMENT_DESC* layout,
	UINT numElements,
	ID3D11InputLayout ** ppInputLayout)
{
	ThrowifFailed(pDevice->CreateInputLayout(layout, numElements, lpData, dwSize, ppInputLayout));
}

void d3dUtil::LoadVertexShaderFile(
	ID3D11Device* pDevice,
	const void* pShaderFile,
	ID3D11VertexShader** pVertexShader,
	const char * pFuncName,
	ID3DBlob** pBlobOut)
{
	CompileShaderFromFile((TCHAR*)pShaderFile, pFuncName, "vs_5_0").CopyTo(pBlobOut);
	ThrowifFailed(pDevice->CreateVertexShader((*pBlobOut)->GetBufferPointer(), (*pBlobOut)->GetBufferSize(), nullptr, pVertexShader));
}

void d3dUtil::LoadPixelShaderFile(
	ID3D11Device* pDevice,
	const void* pShaderFile,
	ID3D11PixelShader** pPixelShader,
	const char * pFuncName)
{
	ComPtr<ID3DBlob> byteCode = nullptr;
	byteCode = CompileShaderFromFile((TCHAR*)pShaderFile, pFuncName, "ps_5_0");
	ThrowifFailed(pDevice->CreatePixelShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), nullptr, pPixelShader));
}

void d3dUtil::LoadGeometryShaderFile(
	ID3D11Device* pDevice,
	const void* pShaderFile,
	ID3D11GeometryShader** pGeometryShader,
	const char * pFuncName)
{
	ComPtr<ID3DBlob> byteCode = nullptr;
	byteCode = CompileShaderFromFile((TCHAR*)pShaderFile, pFuncName, "gs_5_0");
	ThrowifFailed(pDevice->CreateGeometryShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), nullptr, pGeometryShader));
}

ComPtr<ID3DBlob> d3dUtil::CompileShaderFromFile(
	const WCHAR* szFileName,
	LPCSTR szEntryPoint,
	LPCSTR szShaderModel)
{
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined( DEBUG ) || defined( _DEBUG )	
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	HRESULT hr = S_OK;

	ComPtr<ID3DBlob> byteCode = nullptr;
	ComPtr<ID3DBlob> errors = nullptr;
	hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel, dwShaderFlags, 0, NULL, byteCode.GetAddressOf(), errors.GetAddressOf(), NULL);

	if (errors != nullptr)
		::OutputDebugStringA((char*)errors->GetBufferPointer());

	ThrowifFailed(hr);

	return byteCode;
}

void d3dUtil::CreateShaderResourceView(ID3D11Device* pDevice, const std::tstring& filepath, ID3D11ShaderResourceView** pShaderResource)
{
	D3DX11_IMAGE_LOAD_INFO loadinfo;
	ZeroMemory(&loadinfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
	loadinfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	loadinfo.Format = DXGI_FORMAT_FROM_FILE;

	ThrowifFailed(D3DX11CreateShaderResourceViewFromFile(pDevice, filepath.c_str(), &loadinfo, nullptr, pShaderResource, nullptr));
}