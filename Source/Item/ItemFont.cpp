#include "ItemFont.h"

bool ItemFont::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& ShaderFilepath,
						const std::string& VSFunc, const std::string& PSFunc)
{
	CreateVertexBuffer(pDevice);
	m_Object.CreateShader(pDevice, Name, ShaderFilepath, VSFunc, PSFunc);
	CreateConstantBuffer(pDevice);
	m_Object.CreateRasterizer(pDevice);
	CreateIndexBuffer(pDevice);
	m_Object.setBlendState(pDevice);
	return true;
}
bool ItemFont::Frame()
{
	if (m_pTexture != nullptr)
	{
		m_Object.setTexture(m_pTexture);
		m_ConstantData.Util.x = m_pTexture->getImageSize().x;
		m_ConstantData.Util.y = m_pTexture->getImageSize().y;
		m_VertexList[0].TexPos = { 0.0f,0.0f };
		m_VertexList[1].TexPos = { m_ConstantData.Util.x, 0.0f };
		m_VertexList[2].TexPos = { 0.0f,m_ConstantData.Util.y };
		m_VertexList[3].TexPos = { m_ConstantData.Util.x,m_ConstantData.Util.y };
		m_VertexList[0].Pos = { m_Centerpos.x - m_ConstantData.Util.x * 0.5f, m_Centerpos.y - m_ConstantData.Util.y * 0.5f, 0.5f };
		m_VertexList[1].Pos = { m_Centerpos.x + m_ConstantData.Util.x * 0.5f, m_Centerpos.y - m_ConstantData.Util.y * 0.5f, 0.5f };
		m_VertexList[2].Pos = { m_Centerpos.x - m_ConstantData.Util.x * 0.5f, m_Centerpos.y + m_ConstantData.Util.y * 0.5f, 0.5f };
		m_VertexList[3].Pos = { m_Centerpos.x + m_ConstantData.Util.x * 0.5f, m_Centerpos.y + m_ConstantData.Util.y * 0.5f, 0.5f };
	}
	return true;
}
bool ItemFont::Render(ID3D11DeviceContext* pContext)
{
	if (m_pTexture != nullptr)
	{
		PreRender(pContext);
		PostRender(pContext);
	}
	return true;
}
void ItemFont::setTexture(Texture* pTexture)
{
	m_pTexture = pTexture;
	m_Object.setTexture(pTexture);
}
Texture * ItemFont::getTexture()
{
	GETPTR(m_pTexture);
}