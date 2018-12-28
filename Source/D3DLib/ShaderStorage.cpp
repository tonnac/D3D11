#include "ShaderStorage.h"

using Microsoft::WRL::ComPtr;

void ShaderStorage::Initialize(ID3D11Device * Device)
{
	ComPtr<ID3D11VertexShader> vertexShader = nullptr;
	ComPtr<ID3D11PixelShader> pixelShader = nullptr;
	ComPtr<ID3D11InputLayout> inputLayout = nullptr;
	ComPtr<ID3DBlob> vBlob = nullptr;

	{
		d3dUtil::LoadVertexShaderFile(Device, L"line.hlsl", nullptr, vertexShader.GetAddressOf(), "VS", vBlob.GetAddressOf());
		d3dUtil::LoadPixelShaderFile(Device, L"line.hlsl", nullptr, pixelShader.GetAddressOf());
		CreateInputLayout(Device, LINE, inputLayout.GetAddressOf(), vBlob.Get());

		mVertexShader["line"] = std::move(vertexShader);
		mPixelShader["line"] = std::move(pixelShader);
		mInputlayout["line"] = std::move(inputLayout);

		vBlob.Reset();
	}

	{
		d3dUtil::LoadVertexShaderFile(Device, L"Default.hlsl", nullptr, vertexShader.GetAddressOf(), "VS", vBlob.GetAddressOf());
		d3dUtil::LoadPixelShaderFile(Device, L"Default.hlsl", nullptr, pixelShader.GetAddressOf());
		CreateInputLayout(Device, DEFAULT, inputLayout.GetAddressOf(), vBlob.Get());

		mVertexShader["default"] = std::move(vertexShader);
		mPixelShader["default"] = std::move(pixelShader);
		mInputlayout["default"] = std::move(inputLayout);

		vBlob.Reset();
	}

	{
		d3dUtil::LoadVertexShaderFile(Device, L"sky.hlsl", nullptr, vertexShader.GetAddressOf(), "VS", vBlob.GetAddressOf());
		d3dUtil::LoadPixelShaderFile(Device, L"sky.hlsl", nullptr, pixelShader.GetAddressOf());
		CreateInputLayout(Device, SKY, inputLayout.GetAddressOf(), vBlob.Get());

		mVertexShader["sky"] = std::move(vertexShader);
		mPixelShader["sky"] = std::move(pixelShader);
		mInputlayout["sky"] = std::move(inputLayout);

		vBlob.Reset();
	}

	{
		const D3D_SHADER_MACRO defines[] =
		{
			"SKINNED", "1",
			NULL, NULL
		};

		d3dUtil::LoadVertexShaderFile(Device, L"Default.hlsl", defines, vertexShader.GetAddressOf(), "VS", vBlob.GetAddressOf());
		CreateInputLayout(Device, SKINNED, inputLayout.GetAddressOf(), vBlob.Get());

		mVertexShader["skinned"] = std::move(vertexShader);
		mInputlayout["skinned"] = std::move(inputLayout);
	}
}

ID3D11InputLayout * ShaderStorage::getInputLayout(const std::string & name)
{
	std::wstring text;
	if (!Finding(name, text, mInputlayout))
	{
		text += L"'s InputLayout nullptr";
		MessageBox(nullptr, text.c_str(), L"Error", 0);
	}
	return mInputlayout[name].Get();
}

ID3D11VertexShader * ShaderStorage::getVertexShader(const std::string & name)
{
	std::wstring text;
	if (!Finding(name, text, mVertexShader))
	{
		text += L"'s VertexShader nullptr";
		MessageBox(nullptr, text.c_str(), L"Error", 0);
	}
	return mVertexShader[name].Get();
}

ID3D11PixelShader * ShaderStorage::getPixelShader(const std::string & name)
{
	std::wstring text;
	if (!Finding(name, text, mPixelShader))
	{
		text += L"'s PixelShader nullptr";
		MessageBox(nullptr, text.c_str(), L"Error", 0);
	}
	return mPixelShader[name].Get();
}

ID3D11GeometryShader * ShaderStorage::getGeometryShader(const std::string & name)
{
	std::wstring text;
	if (!Finding(name, text, mGeometryShader))
	{
		text += L"'s GeometryShader nullptr";
		MessageBox(nullptr, text.c_str(), L"Error", 0);
	}
	return mGeometryShader[name].Get();
}

ID3D11DomainShader * ShaderStorage::getDomainShader(const std::string & name)
{
	std::wstring text;
	if (!Finding(name, text, mDomainShader))
	{
		text += L"'s DomainShader nullptr";
		MessageBox(nullptr, text.c_str(), L"Error", 0);
	}
	return mDomainShader[name].Get();
}

ID3D11ComputeShader * ShaderStorage::getComputeShader(const std::string & name)
{
	std::wstring text;
	if (!Finding(name, text, mComputeShader))
	{
		text += L"'s ComputeShader nullptr";
		MessageBox(nullptr, text.c_str(), L"Error", 0);
	}
	return mComputeShader[name].Get();
}

ID3D11HullShader * ShaderStorage::getHullShader(const std::string & name)
{
	std::wstring text;
	if (!Finding(name, text, mHullShader))
	{
		text += L"'s HullShader nullptr";
		MessageBox(nullptr, text.c_str(), L"Error", 0);
	}
	return mHullShader[name].Get();
}

void ShaderStorage::CreateInputLayout(ID3D11Device * Device, Inputlayout type, ID3D11InputLayout ** inputLayout, ID3DBlob * blob)
{
	switch (type)
	{
	case DEFAULT:
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{"POSITION"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 0	, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 12	, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, 24	, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD"	, 0, DXGI_FORMAT_R32G32_FLOAT		, 0, 40	, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		d3dUtil::CreateInputLayout(Device,
			(DWORD)blob->GetBufferSize(),
			blob->GetBufferPointer(),
			layout,
			(UINT)std::size(layout),
			inputLayout);
	}break;
	case SKINNED:
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{"POSITION"		, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 0	, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL"		, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 12	, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR"		, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, 24	, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD"		, 0, DXGI_FORMAT_R32G32_FLOAT		, 0, 40	, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"WEIGHTS"		, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, 48	, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BONEINDICES"	, 0, DXGI_FORMAT_R8G8B8A8_UINT		, 0, 64	, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		d3dUtil::CreateInputLayout(Device,
			(DWORD)blob->GetBufferSize(),
			blob->GetBufferPointer(),
			layout,
			(UINT)std::size(layout),
			inputLayout);
	}break;
	case LINE:
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		d3dUtil::CreateInputLayout(Device,
			(DWORD)blob->GetBufferSize(),
			blob->GetBufferPointer(),
			layout,
			(UINT)std::size(layout),
			inputLayout);
	}break;
	case SKY:
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		d3dUtil::CreateInputLayout(Device,
			(DWORD)blob->GetBufferSize(),
			blob->GetBufferPointer(),
			layout,
			(UINT)std::size(layout),
			inputLayout);
	}
	}
}
