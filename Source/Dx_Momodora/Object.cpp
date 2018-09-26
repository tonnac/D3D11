#include "Object.h"
#include "DirectInput.h"


Object::Object()
{}
bool Object::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
					const std::string& VSFunc, const std::string& PSFunc)
{
	std::tstring Filename;
	Filename.assign(TexFilepath, TexFilepath.find_last_of('/') + 1, TexFilepath.find_last_of('.'));
	CreateVertexBuffer(pDevice);
	m_Object.CreateTexture(pDevice, Filename, TexFilepath);
	m_Object.CreateShader(pDevice, Name, ShaderFilepath, VSFunc, PSFunc);
	CreateConstantBuffer(pDevice);
	m_Object.CreateRasterizer(pDevice);
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
bool Object::Render(ID3D11DeviceContext* pContext)
{
	PreRender(pContext);
	PostRender(pContext);
	return true;
}
bool Object::Release()
{
	return m_Object.Release();
}
void Object::SetTexPos(const D3DXVECTOR4& DrawVec)
{
	return;
}
void Object::SetCenterPos(const D3DXVECTOR2& Centerpos)
{
	m_Centerpos = Centerpos;
}
void Object::SetPos(const D3DXVECTOR4& Pos)
{
	return;
}
bool Object::Scroll(const FLOAT& pos)
{
	return true;
}
void Object::MoveCenterPos(const D3DXVECTOR2& vec)
{
	m_Centerpos += vec;
}
D3DXVECTOR2 Object::getCenterPos() const
{
	return m_Centerpos;
}
void Object::CreateVertexBuffer(ID3D11Device* pDevice)
{
	m_Object.CreateBuffer(pDevice, D3D11_BIND_VERTEX_BUFFER, &m_VertexList.at(0), sizeof(P3_VERTEX) * CASTING(UINT, m_VertexList.size()));
}
void Object::CreateConstantBuffer(ID3D11Device* pDevice)
{
	m_ConstantData.Proj = g_mToProj;
	D3DXMatrixTranspose(&m_ConstantData.Proj, &m_ConstantData.Proj);
	if (m_Object.getTexture() != nullptr)
	{
		D3DXVECTOR2 ImageSize = m_Object.getTexture()->getImageSize();
		m_ConstantData.Util.x = ImageSize.x;
		m_ConstantData.Util.y = ImageSize.y;
	}
	m_Object.CreateBuffer(pDevice);
}
bool Object::PreRender(ID3D11DeviceContext* pContext)
{
	ID3D11Buffer * pConstant = m_Object.getConstantBuffer();
	ID3D11BlendState * pBlendState = m_Object.getBlendState();
	if (pBlendState != nullptr)
	{
		pContext->OMSetBlendState(pBlendState, 0, -1);
	}

	pContext->UpdateSubresource(m_Object.getVertexBuffer(), 0, nullptr, &m_VertexList[0], 0, 0);
	if (pConstant != nullptr)
	{
		pContext->PSSetConstantBuffers(0, 1, &pConstant);
		pContext->VSSetConstantBuffers(0, 1, &pConstant);
		pContext->UpdateSubresource(pConstant, 0, nullptr, &m_ConstantData, 0, 0);
	}

	return m_Object.PreRender(pContext);
}
bool Object::PostRender(ID3D11DeviceContext* pContext)
{
	pContext->Draw(CASTING(UINT, m_VertexList.size()), 0);
	return true;
}