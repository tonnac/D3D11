#include "Object.h"


bool Object::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath)
{
	CreateVertexBuffer(pDevice);
	CreateIndexBuffer(pDevice);
	CreateConstantBuffer(pDevice);
	CreateTexture(pDevice, Name, TexFilepath);
	CreateShader(pDevice, Name, ShaderFilepath);
	setBlendState(pDevice);
	Init();
	return true;
}
bool Object::Init()
{
	return true;
}
bool Object::Frame()
{
	return true;
}
bool Object::PreRender(ID3D11DeviceContext* pContext)
{
	ID3D11VertexShader* pVertexShader = m_pShader->getVertexShader();
	ID3D11PixelShader* pPixelShader = m_pShader->getPixelShader();
	ID3D11SamplerState* pSamplerState = m_pTexture->getSamplerState();
	ID3D11ShaderResourceView* m_TexSRV = m_pTexture->getResourceView();
#ifdef GPU
	pContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, &m_VertexList[0], 0, 0);
	pContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &m_ConstantData, 0, 0);
#elif defined CPU
	D3D11_MAPPED_SUBRESOURCE SubRe;
	ZeroMemory(&SubRe, sizeof(D3D11_MAPPED_SUBRESOURCE));
	ThrowifFailed(pContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubRe));
	VS_CB* pe = (VS_CB*)SubRe.pData;
	*pe = m_ConstantData;
	pContext->Unmap(m_pConstantBuffer, 0);
#endif

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pContext->VSSetShader(pVertexShader, nullptr, 0);
	pContext->PSSetShader(pPixelShader, nullptr, 0);

	UINT stride = sizeof(P3_VERTEX);
	UINT offset = 0;
	pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	pContext->IASetInputLayout(m_pInputLayout);

	pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	pContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	pContext->PSSetSamplers(0, 1, &pSamplerState);
	pContext->PSSetShaderResources(0, 1, &m_TexSRV);

	pContext->OMSetBlendState(m_pBlendState, 0, -1);
	return true;
}
bool Object::Render(ID3D11DeviceContext* pContext)
{
	PreRender(pContext);
	PostRender(pContext);
	return true;
}
bool Object::PostRender(ID3D11DeviceContext* pContext)
{
	pContext->DrawIndexed(CASTING(UINT, m_indiciesList.size()), 0, 0);
	return true;
}
bool Object::Release()
{
	RELEASE(m_pVertexBuffer);
	RELEASE(m_pInputLayout);
	RELEASE(m_pConstantBuffer);
	RELEASE(m_pIndexBuffer);
	RELEASE(m_pBlendState);
	return true;
}
RECT* Object::getCollisionRT()
{
	return &m_rtColiision;
}
POINT* Object::getCenterPos()
{
	return &m_CenterPos;
}
void Object::setTexturePos(const DirectX::XMFLOAT2& pos, const int& index)
{
	m_VertexList[index].TexPos = pos;
}
void Object::CreateVertexBuffer(ID3D11Device* pDevice)
{
	D3D11_BUFFER_DESC BufferDesc;
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(P3_VERTEX) * CASTING(UINT, m_VertexList.size());
#ifdef CPU
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
#else
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;
#endif
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA InitialData;
	ZeroMemory(&InitialData, sizeof(D3D11_SUBRESOURCE_DATA));
	InitialData.pSysMem = &m_VertexList.at(0);
	ThrowifFailed(pDevice->CreateBuffer(&BufferDesc, &InitialData, &m_pVertexBuffer));
}
void Object::CreateIndexBuffer(ID3D11Device* pDevice)
{
	D3D11_BUFFER_DESC BufferDesc;
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(DWORD) * CASTING(UINT, m_indiciesList.size());
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA InitialData;
	ZeroMemory(&InitialData, sizeof(D3D11_SUBRESOURCE_DATA));
	InitialData.pSysMem = &m_indiciesList.at(0);
	ThrowifFailed(pDevice->CreateBuffer(&BufferDesc, &InitialData, &m_pIndexBuffer));
}
void Object::CreateConstantBuffer(ID3D11Device* pDevice)
{
	D3D11_BUFFER_DESC BufferDesc;
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(VS_CB);
#ifdef GPU
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;
#elif defined CPU
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
#endif
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	ThrowifFailed(pDevice->CreateBuffer(&BufferDesc, nullptr, &m_pConstantBuffer));
}
void Object::CreateaInputLayout(ID3D11Device* pDevice)
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION" ,	0,	DXGI_FORMAT_R32G32B32_FLOAT ,	0,	0,	D3D11_INPUT_PER_VERTEX_DATA ,	0 },
		{ "TEXCOORD" ,	0,	DXGI_FORMAT_R32G32_FLOAT ,		0,	12,	D3D11_INPUT_PER_VERTEX_DATA ,	0 }
	};
	int layoutNum = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	ID3DBlob* pVSBlob = m_pShader->getVSBlob();
	ThrowifFailed(pDevice->CreateInputLayout(layout, layoutNum, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pInputLayout));
}
void Object::CreateTexture(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& Filepath)
{
	S_Texture.LoadTexture(pDevice, Name, Filepath);
	m_pTexture = S_Texture.getTexture(Name);
}
void Object::CreateShader(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& Filepath)
{
	S_Shader.LoadShader(pDevice, Name, Filepath);
	m_pShader = S_Shader.getShader(Name);
	CreateaInputLayout(pDevice);
}
void Object::setBlendState(ID3D11Device* pDevice)
{
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(D3D11_BLEND_DESC));
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	ThrowifFailed(pDevice->CreateBlendState(&BlendDesc, &m_pBlendState));
	return;
}