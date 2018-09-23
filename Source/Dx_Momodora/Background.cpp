#include "Background.h"
#include "DirectInput.h"
#include "DirectWrite.h"

Background::Background()
{
}

void Background::SetPos(const FLOAT& rtLeft, const FLOAT& rtTop, const FLOAT& MaxWidth, const FLOAT& MaxHeight)
{
	m_MaxSize.left = rtLeft;
	m_MaxSize.right = MaxWidth;
	Plane_Object::SetPos({ 0.0f,0.0f }, { rtLeft, rtTop, rtLeft + g_fImageWidth, rtTop + g_fImageHeight });
	m_VertexList.resize(4);
	m_VertexList[0].Pos = { 0.0f, 0.0f, 0.5f };
	m_VertexList[1].Pos = { CASTING(FLOAT,g_rtClient.right), 0.0f, 0.5f };
	m_VertexList[2].Pos = { 0.0f, CASTING(FLOAT,g_rtClient.bottom), 0.5f };
	m_VertexList[3].Pos = { m_VertexList[1].Pos.x, m_VertexList[2].Pos.y, 0.5f };
}
void Background::SetInversePos(const FLOAT& rtLeft, const FLOAT& rtTop, const FLOAT& MaxWidth, const FLOAT& MaxHeight)
{
	m_MaxSize.left = rtLeft;
	m_MaxSize.right = MaxWidth;
	Plane_Object::SetPos({ 0.0f,0.0f }, { MaxWidth - g_fImageWidth, rtTop, MaxWidth, rtTop + g_fImageHeight });
	m_VertexList.resize(4);
	m_VertexList[0].Pos = { 0.0f, 0.0f, 0.5f };
	m_VertexList[1].Pos = { CASTING(FLOAT,g_rtClient.right), 0.0f, 0.5f };
	m_VertexList[2].Pos = { 0.0f, CASTING(FLOAT,g_rtClient.bottom), 0.5f };
	m_VertexList[3].Pos = { m_VertexList[1].Pos.x, m_VertexList[2].Pos.y, 0.5f };
}
bool Background::Frame()
{
	TexPosUpdate();
	return true;
}
bool Background::Scroll(const FLOAT& pos)
{
	if (pos < 0.0f)
	{
		if (m_DrawVec.x >= m_MaxSize.left)
		{
			m_DrawVec.x += pos;
			m_DrawVec.z += pos;
			return true;
		}
	}
	else
	{
		if (m_DrawVec.z <= m_MaxSize.right)
		{
			m_DrawVec.x += pos;
			m_DrawVec.z += pos;
			return true;
		}
	}
	return false;
}
void Background::TexPosUpdate()
{
	Texture* pTexture = m_Object.getTexture();
	D3DXVECTOR2 pos = pTexture->getImageSize();
	m_VertexList[0].TexPos = { m_DrawVec.x / pos.x, m_DrawVec.y / pos.y };
	m_VertexList[1].TexPos = { m_DrawVec.z / pos.x, m_DrawVec.y / pos.y };
	m_VertexList[2].TexPos = { m_DrawVec.x / pos.x, m_DrawVec.w / pos.y };
	m_VertexList[3].TexPos = { m_DrawVec.z / pos.x, m_DrawVec.w / pos.y };
}