#include "ShaderStorage.h"

using Microsoft::WRL::ComPtr;

void ShaderStorage::Initialize(ID3D11Device * Device)
{
	ComPtr<ID3D11VertexShader> vertexShader = nullptr;
	ComPtr<ID3D11PixelShader> pixelShader = nullptr;
	ComPtr<ID3D11GeometryShader> geometryShader = nullptr;
	ComPtr<ID3D11InputLayout> inputLayout = nullptr;
	ComPtr<ID3DBlob> vBlob = nullptr;

	{
		d3dUtil::LoadVertexShaderFile(Device, L"shaders\\line.hlsl", nullptr, vertexShader.GetAddressOf(), "VS", vBlob.GetAddressOf());
		d3dUtil::LoadPixelShaderFile(Device, L"shaders\\line.hlsl", nullptr, pixelShader.GetAddressOf());
		CreateInputLayout(Device, LINE, inputLayout.GetAddressOf(), vBlob.Get());

		mVertexShader[L"line"] = std::move(vertexShader);
		mPixelShader[L"line"] = std::move(pixelShader);
		mInputlayout[L"line"] = std::move(inputLayout);

		vBlob.Reset();
	}

	{
		d3dUtil::LoadVertexShaderFile(Device, L"shaders\\composite.hlsl", nullptr, vertexShader.GetAddressOf(), "VS", vBlob.GetAddressOf());
		d3dUtil::LoadPixelShaderFile(Device, L"shaders\\composite.hlsl", nullptr, pixelShader.GetAddressOf());

		mVertexShader[L"composite"] = std::move(vertexShader);
		mPixelShader[L"composite"] = std::move(pixelShader);

		vBlob.Reset();
	}

	{
		d3dUtil::LoadVertexShaderFile(Device, L"shaders\\normal.hlsl", nullptr, vertexShader.GetAddressOf(), "VS", vBlob.GetAddressOf());
		d3dUtil::LoadGeometryShaderFile(Device, L"shaders\\normal.hlsl", nullptr, geometryShader.GetAddressOf(), "GS");
		d3dUtil::LoadPixelShaderFile(Device, L"shaders\\normal.hlsl", nullptr, pixelShader.GetAddressOf());

		mVertexShader[L"normal"] = std::move(vertexShader);
		mGeometryShader[L"normal"] = std::move(geometryShader);
		mPixelShader[L"normal"] = std::move(pixelShader);

		vBlob.Reset();
	}

	{
		d3dUtil::LoadVertexShaderFile(Device, L"shaders\\Default.hlsl", nullptr, vertexShader.GetAddressOf(), "VS", vBlob.GetAddressOf());
		d3dUtil::LoadPixelShaderFile(Device, L"shaders\\Default.hlsl", nullptr, pixelShader.GetAddressOf());
		CreateInputLayout(Device, DEFAULT, inputLayout.GetAddressOf(), vBlob.Get());

		mVertexShader[L"default"] = std::move(vertexShader);
		mPixelShader[L"default"] = std::move(pixelShader);
		mInputlayout[L"default"] = std::move(inputLayout);

		vBlob.Reset();
	}

	{
		d3dUtil::LoadVertexShaderFile(Device, L"shaders\\sky.hlsl", nullptr, vertexShader.GetAddressOf(), "VS", vBlob.GetAddressOf());
		d3dUtil::LoadPixelShaderFile(Device, L"shaders\\sky.hlsl", nullptr, pixelShader.GetAddressOf());
		CreateInputLayout(Device, SKY, inputLayout.GetAddressOf(), vBlob.Get());

		mVertexShader[L"sky"] = std::move(vertexShader);
		mPixelShader[L"sky"] = std::move(pixelShader);
		mInputlayout[L"sky"] = std::move(inputLayout);

		vBlob.Reset();
	}

	{
		d3dUtil::LoadPixelShaderFile(Device, L"shaders\\MeshNoTex.hlsl", nullptr, pixelShader.GetAddressOf());
		mPixelShader[L"notex"] = std::move(pixelShader);
	}

	{
		const D3D_SHADER_MACRO defines[] =
		{
			"SKINNED", "1",
			NULL, NULL
		};

		d3dUtil::LoadVertexShaderFile(Device, L"shaders\\Default.hlsl", defines, vertexShader.GetAddressOf(), "VS", vBlob.GetAddressOf());
		CreateInputLayout(Device, SKINNED, inputLayout.GetAddressOf(), vBlob.Get());

		mVertexShader[L"skinned"] = std::move(vertexShader);
		mInputlayout[L"skinned"] = std::move(inputLayout);
	}

	{
		const D3D_SHADER_MACRO defines[] =
		{
			"NORMALMAP", "1",
			NULL, NULL
		};

		d3dUtil::LoadPixelShaderFile(Device, L"shaders\\Default.hlsl", defines, pixelShader.GetAddressOf());
		mPixelShader[L"bump"] = std::move(pixelShader);
	}
}

ID3D11InputLayout * ShaderStorage::getInputLayout(const std::wstring & name)
{
	std::wstring text;
	if (!Finding(name, text, mInputlayout))
	{
		text += L"'s InputLayout nullptr";
		MessageBox(nullptr, text.c_str(), L"Error", 0);
	}
	return mInputlayout[name].Get();
}

ID3D11VertexShader * ShaderStorage::getVertexShader(const std::wstring & name)
{
	std::wstring text;
	if (!Finding(name, text, mVertexShader))
	{
		text += L"'s VertexShader nullptr";
		MessageBox(nullptr, text.c_str(), L"Error", 0);
	}
	return mVertexShader[name].Get();
}

ID3D11PixelShader * ShaderStorage::getPixelShader(const std::wstring & name)
{
	std::wstring text;
	if (!Finding(name, text, mPixelShader))
	{
		text += L"'s PixelShader nullptr";
		MessageBox(nullptr, text.c_str(), L"Error", 0);
	}
	return mPixelShader[name].Get();
}

ID3D11GeometryShader * ShaderStorage::getGeometryShader(const std::wstring & name)
{
	std::wstring text;
	if (!Finding(name, text, mGeometryShader))
	{
		text += L"'s GeometryShader nullptr";
		MessageBox(nullptr, text.c_str(), L"Error", 0);
	}
	return mGeometryShader[name].Get();
}

ID3D11DomainShader * ShaderStorage::getDomainShader(const std::wstring & name)
{
	std::wstring text;
	if (!Finding(name, text, mDomainShader))
	{
		text += L"'s DomainShader nullptr";
		MessageBox(nullptr, text.c_str(), L"Error", 0);
	}
	return mDomainShader[name].Get();
}

ID3D11ComputeShader * ShaderStorage::getComputeShader(const std::wstring & name)
{
	std::wstring text;
	if (!Finding(name, text, mComputeShader))
	{
		text += L"'s ComputeShader nullptr";
		MessageBox(nullptr, text.c_str(), L"Error", 0);
	}
	return mComputeShader[name].Get();
}

ID3D11HullShader * ShaderStorage::getHullShader(const std::wstring & name)
{
	std::wstring text;
	if (!Finding(name, text, mHullShader))
	{
		text += L"'s HullShader nullptr";
		MessageBox(nullptr, text.c_str(), L"Error", 0);
	}
	return mHullShader[name].Get();
}

ID3D11ComputeShader* ShaderStorage::AddComputeShader(Microsoft::WRL::ComPtr<ID3D11ComputeShader> computeshader, const std::wstring& name)
{
	mComputeShader[name] = computeshader;
	return mComputeShader[name].Get();
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
			{"TANGENT"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 48	, D3D11_INPUT_PER_VERTEX_DATA, 0},
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
			{"TANGENT"		, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 48	, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"WEIGHTS"		, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, 60	, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BONEINDICES"	, 0, DXGI_FORMAT_R8G8B8A8_UINT		, 0, 76	, D3D11_INPUT_PER_VERTEX_DATA, 0}
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
	}break;
	}
}
