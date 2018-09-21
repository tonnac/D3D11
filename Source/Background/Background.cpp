#include "Background.h"
#include "DirectInput.h"

Background::Background()
{
	SetInitPos = [this](const D2D1_POINT_2F& pe, const D2D1_RECT_F& rect) {SetPos(pe, rect);};
	auto foo = [this](const D2D1_RECT_F& rect) {SetPos({ 0.0f,0.0f }, rect);};
}

void Background::SetPos(const D2D1_POINT_2F& pos, const D2D1_RECT_F& rect)
{
	m_VertexList.resize(4);
	m_VertexList[0].Pos.x = -1.0f;
	m_VertexList[0].Pos.y = 1.0f;
	m_VertexList[1].Pos.x = 1.0f;
	m_VertexList[1].Pos.y = 1.0f;
	m_VertexList[2].Pos.x = -1.0f;
	m_VertexList[2].Pos.y = -1.0f;
	m_VertexList[3].Pos.x = 1.0f;
	m_VertexList[3].Pos.y = -1.0f;
	m_rtDraw = rect;
	
}
bool Background::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath)
{
	Plane_Object::InitSet(pDevice, Name, TexFilepath, ShaderFilepath);
	return true;
}
bool Background::Frame()
{
	Texture* pTexture = m_Object.getTexture();
	DirectX::XMFLOAT2 pos = pTexture->getImageSize();
	m_VertexList[0].TexPos.x = m_rtDraw.left / pos.x;
	m_VertexList[0].TexPos.y = m_rtDraw.top / pos.y;

	m_VertexList[1].TexPos.x = m_rtDraw.right / pos.x;
	m_VertexList[1].TexPos.y = m_rtDraw.top / pos.y;

	m_VertexList[2].TexPos.x = m_rtDraw.left / pos.x;
	m_VertexList[2].TexPos.y = m_rtDraw.bottom / pos.y;

	m_VertexList[3].TexPos.x = m_rtDraw.right / pos.x;
	m_VertexList[3].TexPos.y = m_rtDraw.bottom / pos.y;
	return true;
}