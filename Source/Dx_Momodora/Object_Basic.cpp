#include "Object_Basic.h"
#include "DirectInput.h"


bool Object_Basic::PreRender(ID3D11DeviceContext* pContext)
{
	ID3D11VertexShader* pVertexShader = m_pShader->getVertexShader();
	ID3D11PixelShader* pPixelShader = m_pShader->getPixelShader();
	if (m_pTexture)
	{
		ID3D11SamplerState* pSamplerState = m_pTexture->getSamplerState();
		ID3D11ShaderResourceView* m_TexSRV = m_pTexture->getResourceView();

		pContext->PSSetSamplers(0, 1, &pSamplerState);
		pContext->PSSetShaderResources(0, 1, &m_TexSRV);
	}
	if (S_Input.getKeyState(DIK_PGDN) == Input::KEYSTATE::KEY_HOLD)
	{
		pContext->RSSetState(m_pWireFrameRS);
	}
	else
		pContext->RSSetState(m_pSolidRS);

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pContext->VSSetShader(pVertexShader, nullptr, 0);
	pContext->PSSetShader(pPixelShader, nullptr, 0);

	UINT stride = sizeof(P3_VERTEX);
	UINT offset = 0;
	pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	pContext->IASetInputLayout(m_pInputLayout);

	return true;
}
bool Object_Basic::Release()
{
	RELEASE(m_pVertexBuffer);
	RELEASE(m_pInputLayout);
	RELEASE(m_pConstantBuffer);
	RELEASE(m_pIndexBuffer);
	RELEASE(m_pBlendState);
	RELEASE(m_pWireFrameRS);
	RELEASE(m_pSolidRS);
	return true;
}

void Object_Basic::CreateBuffer(ID3D11Device * pDevice, const D3D11_BIND_FLAG& type, void * pData, const UINT& Size)
{
	D3D11_BUFFER_DESC BufferDesc;
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = Size;
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	BufferDesc.BindFlags = type;
	switch (type)
	{
	case D3D11_BIND_VERTEX_BUFFER:
	{
		D3D11_SUBRESOURCE_DATA InitialData;
		ZeroMemory(&InitialData, sizeof(D3D11_SUBRESOURCE_DATA));
		InitialData.pSysMem = pData;
		ThrowifFailed(pDevice->CreateBuffer(&BufferDesc, &InitialData, &m_pVertexBuffer));
	}break;
	case D3D11_BIND_INDEX_BUFFER:
	{
		D3D11_SUBRESOURCE_DATA InitialData;
		ZeroMemory(&InitialData, sizeof(D3D11_SUBRESOURCE_DATA));
		InitialData.pSysMem = pData;
		ThrowifFailed(pDevice->CreateBuffer(&BufferDesc, &InitialData, &m_pIndexBuffer));
	}break;
	case D3D11_BIND_CONSTANT_BUFFER:
	{
#ifdef CPU
		BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
#endif
		ThrowifFailed(pDevice->CreateBuffer(&BufferDesc, nullptr, &m_pConstantBuffer));
	}
	}
}
void Object_Basic::CreateTexture(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& Filepath)
{
	m_pTexture = S_Texture.LoadTexture(pDevice, Name, Filepath);
}
void Object_Basic::CreateShader(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& Filepath, const std::string& VSFunc, const std::string& PSFunc)
{
	m_pShader = S_Shader.LoadShader(pDevice, Name, Filepath, VSFunc, PSFunc);
	CreateaInputLayout(pDevice);
}
void Object_Basic::setBlendState(ID3D11Device* pDevice)
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
}
void Object_Basic::CreateaInputLayout(ID3D11Device* pDevice)
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION" ,	0,	DXGI_FORMAT_R32G32B32_FLOAT ,	0,	0,	D3D11_INPUT_PER_VERTEX_DATA ,	0 },
		{ "COLOR"	 ,	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	12,	D3D11_INPUT_PER_VERTEX_DATA ,	0 },
		{ "TEXCOORD" ,	0,	DXGI_FORMAT_R32G32_FLOAT ,		0,	28,	D3D11_INPUT_PER_VERTEX_DATA ,	0 },
	};
	int layoutNum = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	ID3DBlob* pVSBlob = m_pShader->getVSBlob();
	ThrowifFailed(pDevice->CreateInputLayout(layout, layoutNum, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pInputLayout));
}
void Object_Basic::CreateRasterizer(ID3D11Device* pDevice)
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_NONE;
	desc.DepthClipEnable = TRUE;
	ThrowifFailed(pDevice->CreateRasterizerState(&desc, &m_pSolidRS));
	desc.FillMode = D3D11_FILL_WIREFRAME;
	ThrowifFailed(pDevice->CreateRasterizerState(&desc, &m_pWireFrameRS));
}
ID3D11Buffer * Object_Basic::getVertexBuffer() const
{
	GETPTR(m_pVertexBuffer);
}
ID3D11Buffer * Object_Basic::getIndexBuffer() const
{
	GETPTR(m_pIndexBuffer);
}
ID3D11Buffer * Object_Basic::getConstantBuffer() const
{
	GETPTR(m_pConstantBuffer);
}
ID3D11BlendState* Object_Basic::getBlendState() const
{
	GETPTR(m_pBlendState);
}
Texture* Object_Basic::getTexture() const
{
	GETPTR(m_pTexture);
}