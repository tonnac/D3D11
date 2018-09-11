#include "Sample.h"

Sample::Sample() : m_pVertexBuffer(nullptr), m_pVertexLayout(nullptr), m_pVertexShader(nullptr), m_pPixelShader(nullptr), m_pIndexBuffer(nullptr)
{}				   
Sample::~Sample() {}

HRESULT Sample::CreateVertexBuffer()
{
	m_VertexList.push_back({ -0.5f,	0.5f,	0.5f });
	m_VertexList.push_back({ 0.5f,	0.5f,	0.5f });
	m_VertexList.push_back({ -0.5f,	-0.5f,	0.5f });
	m_VertexList.push_back({ 0.5f,	-0.5f,	0.5f });

	HRESULT hr;
	ID3D11Device* pDevice = getDevice();

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(P3VERTEX) * CASTING(UINT,m_VertexList.size());
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
	bd.ByteWidth = sizeof(DWORD) * CASTING(UINT,m_IndexList.size());
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initdata;
	ZeroMemory(&initdata, sizeof(D3D11_SUBRESOURCE_DATA));
	initdata.pSysMem = &m_IndexList.at(0);

	DXFAIL(pDevice->CreateBuffer(&bd, &initdata, &m_pIndexBuffer));

	return hr;
}
HRESULT Sample::LoadShaderAndInputLayout()
{
	HRESULT hr;
	ID3D11Device* pDevice = getDevice();
	ID3DBlob* pVSBlob = nullptr;
	ID3DBlob* pErrBlob = nullptr;
	DWORD dwFlag = D3DCOMPILE_DEBUG;
	if (FAILED(D3DX11CompileFromFile(L"../../data/VertexShader.txt", NULL,
		NULL, "VS", "vs_5_0", dwFlag, NULL, NULL, &pVSBlob, &pErrBlob, NULL)))
	{
		std::string Error((char*)pErrBlob->GetBufferPointer());
		MessageBox(nullptr, (LPCWSTR)pErrBlob->GetBufferPointer(), L"Error", MB_OK);
	}
	DXFAIL(pDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), 
		pVSBlob->GetBufferSize(), nullptr, &m_pVertexShader));

	//D3D11_INPUT_ELEMENT_DESC layout[] =
	//{ "POSITION" , 0 , DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	D3D11_INPUT_ELEMENT_DESC layout;
	layout.SemanticName = "POSITION";
	layout.SemanticIndex = 0;
	layout.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layout.InputSlot = 0;
	layout.AlignedByteOffset = 0;
	layout.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout.InstanceDataStepRate = 0;

	int iNum = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	DXFAIL(pDevice->CreateInputLayout(&layout, iNum, pVSBlob->GetBufferPointer(), 
		pVSBlob->GetBufferSize(), &m_pVertexLayout));


	ID3DBlob* pPSBlob = nullptr;
	if (FAILED(D3DX11CompileFromFile(L"../../data/VertexShader.txt", NULL,
		NULL, "PS", "ps_5_0", dwFlag, NULL, NULL, &pPSBlob, &pErrBlob, NULL)))
	{
		MessageBox(nullptr, (LPCWSTR)pErrBlob->GetBufferPointer(), L"Error", MB_OK);
	}
	DXFAIL(pDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), 
		pPSBlob->GetBufferSize(), nullptr, &m_pPixelShader));

	RELEASE(pVSBlob);
	RELEASE(pPSBlob);
	return hr;
}
bool Sample::Init()
{
	CreateVertexBuffer();
	CreateIndexBuffer();
	if (FAILED(LoadShaderAndInputLayout()))
	{
		return false;
	}
	return true;
}
bool Sample::Frame()
{
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

	pContext->DrawIndexed(CASTING(UINT,m_IndexList.size()), 0, 0);
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