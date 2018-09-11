#include "Sample.h"

Sample::Sample() : m_pVertexBuffer(nullptr), m_pVertexLayout(nullptr), m_pVertexShader(nullptr), m_pPixelShader(nullptr), m_pIndexBuffer(nullptr), m_pConstantBuffer(nullptr)
{}
Sample::~Sample() {}

HRESULT Sample::CreateVertexBuffer()
{
	m_VertexList.push_back({ -0.5f,	0.5f,	0.5f,	1.0f, 0.0f, 0.0f, 1.0f,		0.0f, 0.0f });
	m_VertexList.push_back({ 0.5f,	0.5f,	0.5f,	0.0f, 1.0f, 0.0f, 1.0f,		2.0f, 0.0f });
	m_VertexList.push_back({ -0.5f,	-0.5f,	0.5f,	0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 2.0f });
	m_VertexList.push_back({ 0.5f,	-0.5f,	0.5f,	1.0f, 1.0f, 1.0f, 1.0f,		2.0f, 2.0f });

	HRESULT hr;
	ID3D11Device* pDevice = getDevice();

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(P3VERTEX) * CASTING(UINT, m_VertexList.size());
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initdata;
	ZeroMemory(&initdata, sizeof(D3D11_SUBRESOURCE_DATA));
	initdata.pSysMem = &m_VertexList.at(0);

	DXFAIL(pDevice->CreateBuffer(&bd, &initdata, &m_pVertexBuffer));


	return hr;
}
HRESULT	Sample::CreateIndexBuffer()
{
	m_IndexList.push_back(0);
	m_IndexList.push_back(1);
	m_IndexList.push_back(2);
	m_IndexList.push_back(1);
	m_IndexList.push_back(3);
	m_IndexList.push_back(2);
	HRESULT hr;
	ID3D11Device* pDevice = getDevice();

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(DWORD) * CASTING(UINT, m_IndexList.size());
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initdata;
	ZeroMemory(&initdata, sizeof(D3D11_SUBRESOURCE_DATA));
	initdata.pSysMem = &m_IndexList.at(0);

	DXFAIL(pDevice->CreateBuffer(&bd, &initdata, &m_pIndexBuffer));

	return hr;
}
HRESULT	Sample::CreateConstantBuffer()
{
	HRESULT hr;
	ID3D11Device* pDevice = getDevice();

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(VS_CB);
#ifdef CPU
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
#elif defined GPU
	bd.Usage = D3D11_USAGE_DEFAULT;
#endif
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA initdata;
	ZeroMemory(&initdata, sizeof(D3D11_SUBRESOURCE_DATA));
	initdata.pSysMem = &m_IndexList.at(0);

	DXFAIL(pDevice->CreateBuffer(&bd, &initdata, &m_pConstantBuffer));

	return hr;
}
HRESULT Sample::LoadShaderAndInputLayout()
{
	HRESULT hr;
	ID3D11Device* pDevice = getDevice();
	ID3DBlob* pVSBlob = nullptr;
	ID3DBlob* pErrBlob = nullptr;
	DWORD dwFlag = D3DCOMPILE_DEBUG;
	if (FAILED(D3DX11CompileFromFile(L"VertexShader.txt", NULL,
		NULL, "VS", "vs_5_0", dwFlag, NULL, NULL, &pVSBlob, &pErrBlob, NULL)))
	{
		std::string Error((char*)pErrBlob->GetBufferPointer());
		std::ofstream pe("Error.txt");
		if (pe.is_open())
		{
			pe << Error;
		}
	}
	DXFAIL(pDevice->CreateVertexShader(pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), nullptr, &m_pVertexShader));

	//D3D11_INPUT_ELEMENT_DESC layout[] =
	//{ "POSITION" , 0 , DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0,	DXGI_FORMAT_R32G32_FLOAT,		0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	int iNum = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	DXFAIL(pDevice->CreateInputLayout(layout, iNum, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &m_pVertexLayout));


	ID3DBlob* pPSBlob = nullptr;
	if (FAILED(D3DX11CompileFromFile(L"VertexShader.txt", NULL,
		NULL, "PS", "ps_5_0", dwFlag, NULL, NULL, &pPSBlob, &pErrBlob, NULL)))
	{
		std::string Error((char*)pErrBlob->GetBufferPointer());
		std::fstream pe("Error.txt");
		if (pe.is_open())
		{
			pe << Error;
		}
	}
	DXFAIL(pDevice->CreatePixelShader(pPSBlob->GetBufferPointer(),
		pPSBlob->GetBufferSize(), nullptr, &m_pPixelShader));

	RELEASE(pVSBlob);
	RELEASE(pPSBlob);
	return hr;
}
HRESULT	Sample::LoadTextureFile(const TCHAR* szFileName)
{
	HRESULT hr;
	ID3D11Device* pDevice = getDevice();

	DXFAIL(D3DX11CreateShaderResourceViewFromFile(pDevice, szFileName, nullptr, nullptr, &m_pTextureSRV, nullptr));


	D3D11_SAMPLER_DESC samplerdesc;
	ZeroMemory(&samplerdesc, sizeof(samplerdesc));
	samplerdesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerdesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerdesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerdesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	DXFAIL(pDevice->CreateSamplerState(&samplerdesc, &m_pSamplerState));

	return hr;
}
bool Sample::Init()
{
	LoadTextureFile(L"../../data/png/MS.png");
	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateConstantBuffer();
	if (FAILED(LoadShaderAndInputLayout()))
	{
		return false;
	}

	return true;
}
bool Sample::Frame()
{
	float fSpeed = 0.0f;
	static float fScale = 0.0f;

	ID3D11DeviceContext* pContext = getContext();
	m_ConstantData.vColor[0] = sin(g_fGameTime) * 0.5f + 0.5f;
	m_ConstantData.vColor[1] = sin(-g_fGameTime) * 0.5f + 0.5f;
	m_ConstantData.vColor[2] = cos(g_fGameTime) * 0.5f + 0.5f;
	m_ConstantData.vColor[3] = 1.0f;
	m_ConstantData.fTime[0] = fScale;
	m_ConstantData.fTime[1] = g_fGameTime;
	m_ConstantData.fTime[2] = 1.2f;
	if (S_Input.getKeyState(DIK_INSERT) == KEYSTATE::KEY_HOLD)
	{
		fSpeed = 50.0f;
	}
	else
	{
		fSpeed = 0.0f;
	}
	fScale += g_fSecPerFrame * fSpeed;
#ifdef CPU
	D3D11_MAPPED_SUBRESOURCE address;
	if (SUCCEEDED(pContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &address)))
	{
		VS_CB* pCB = RE_CASTING(VS_CB*, address.pData);
		*pCB = m_ConstantData;
		pContext->Unmap(m_pConstantBuffer, 0);
	}
#elif defined GPU
	pContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &m_ConstantData, 0, 0);
#endif
	return true;
}
bool Sample::Render()
{
	ID3D11DeviceContext* pContext = getContext();
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pContext->VSSetShader(m_pVertexShader, NULL, 0);
	pContext->PSSetShader(m_pPixelShader, NULL, 0);

	UINT offset = 0;
	UINT stride = sizeof(P3VERTEX);
	pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	pContext->IASetInputLayout(m_pVertexLayout);

	pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	pContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	pContext->PSSetShaderResources(0, 1, &m_pTextureSRV);
	pContext->PSSetSamplers(0, 1, &m_pSamplerState);

	pContext->DrawIndexed(CASTING(UINT, m_IndexList.size()), 0, 0);
	return true;
}
bool Sample::Release()
{
	RELEASE(m_pVertexBuffer);
	RELEASE(m_pVertexLayout);
	RELEASE(m_pVertexShader);
	RELEASE(m_pPixelShader);
	RELEASE(m_pIndexBuffer);
	return true;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevhInstance, LPWSTR szCmdLine, int nCmdShow)
{
	Sample sd;
	sd.Set(hInstance, 800, 600, L"Sample");
	sd.Run();
	return 0;
}