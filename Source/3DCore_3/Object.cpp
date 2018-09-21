#include "Object.h"
#include "DirectInput.h"

bool Object::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath)
{
	std::tstring Filename;
	Filename.assign(TexFilepath, TexFilepath.find_first_of('/') + 1);
	CreateVertexBuffer(pDevice);
	m_Object.CreateTexture(pDevice, Name, TexFilepath);
	m_Object.CreateShader(pDevice, Name, ShaderFilepath);
	m_Object.CreateRasterizer(pDevice);
	Init();
	return true;
}
void Object::CreateVertexBuffer(ID3D11Device* pDevice)
{
	m_Object.CreateBuffer(pDevice, D3D11_BIND_VERTEX_BUFFER, &m_VertexList.at(0), sizeof(P3_VERTEX) * CASTING(UINT, m_VertexList.size()));
}
void Object::CreateConstantBuffer(ID3D11Device* pDevice)
{
	m_Object.CreateBuffer(pDevice);
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
	ID3D11Buffer * pConstant = m_Object.getConstantBuffer();
	ID3D11BlendState * pBlendState = m_Object.getBlendState();
	if (pConstant != nullptr)
	{
		pContext->PSSetConstantBuffers(0, 1, &pConstant);
		pContext->VSSetConstantBuffers(0, 1, &pConstant);
	}
	if (pBlendState != nullptr)
	{
		pContext->OMSetBlendState(pBlendState, 0, -1);
	}
#ifdef GPU
	pContext->UpdateSubresource(m_Object.getVertexBuffer(), 0, nullptr, &m_VertexList[0], 0, 0);
	if (pConstant != nullptr)
	{
		pContext->UpdateSubresource(pConstant, 0, nullptr, &m_ConstantData, 0, 0);
	}
#elif defined CPU
	D3D11_MAPPED_SUBRESOURCE SubRe;
	ZeroMemory(&SubRe, sizeof(D3D11_MAPPED_SUBRESOURCE));
	ThrowifFailed(pContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubRe));
	VS_CB* pe = (VS_CB*)SubRe.pData;
	*pe = m_ConstantData;
	pContext->Unmap(m_pConstantBuffer, 0);
#endif
	return m_Object.PreRender(pContext);
}
bool Object::Render(ID3D11DeviceContext* pContext)
{
	PreRender(pContext);
	PostRender(pContext);
	return true;
}
bool Object::PostRender(ID3D11DeviceContext* pContext)
{
	pContext->Draw(CASTING(UINT, m_VertexList.size()), 0);
	return true;
}
D2D1_POINT_2F Object::ComputeCoord()
{
	D2D1_POINT_2F pos;
	pos.x = (m_Centerpos.x * 2) / g_rtClient.right - 1;
	pos.y = 1 - (m_Centerpos.y * 2) / g_rtClient.bottom;
	return pos;
}
D2D1_POINT_2F Object::ComputeCoord(const D2D1_POINT_2F& pos)
{
	D2D1_POINT_2F retpos;
	retpos.x = (pos.x * 2) / g_rtClient.right - 1;
	retpos.y = 1 - (pos.y * 2) / g_rtClient.bottom;
	return retpos;
}
void Object::ComputeCollision(const D2D1_POINT_2F& col)
{
	m_rtCollision.left = m_Centerpos.x - col.x * 0.5f;
	m_rtCollision.top = m_Centerpos.y + col.y * 0.5f;
	m_rtCollision.right = m_Centerpos.x + col.x * 0.5f;
	m_rtCollision.bottom = m_Centerpos.y - col.y * 0.5f;
}
void Object::SetInitPos(const D2D1_POINT_2F& pos)
{
	m_Centerpos = pos;
}