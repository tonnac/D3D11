#include "Background.h"
#include "DirectInput.h"

Background::Background() : m_fWidth(370.0f), m_fHeight(240.0f)
{}

void Background::SetPos(const FLOAT& rtLeft, const FLOAT& rtTop, const FLOAT& MaxWidth, const FLOAT& MaxHeight)
{
	m_MaxSize.left = rtLeft;
	m_MaxSize.right = MaxWidth;
	Plane_Object::SetPos({ 0.0f,0.0f }, { rtLeft, rtTop, rtLeft + m_fWidth, rtTop + m_fHeight });
	m_VertexList.resize(4);
	m_VertexList[0].Pos = { -1.0f, 1.0f, 0.5f };
	m_VertexList[1].Pos = { 1.0f, 1.0f, 0.5f };
	m_VertexList[2].Pos = { -1.0f, -1.0f, 0.5f };
	m_VertexList[3].Pos = { 1.0f, -1.0f, 0.5f };
}
bool Background::Frame()
{
	if (S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_HOLD && m_rtDraw.right <= m_MaxSize.right)
	{
		m_rtDraw.left += g_fSecPerFrame * 200.0f;
		m_rtDraw.right += g_fSecPerFrame * 200.0f;
	}
	if (S_Input.getKeyState(DIK_LEFT) == Input::KEYSTATE::KEY_HOLD && m_rtDraw.left >= m_MaxSize.left)
	{
		m_rtDraw.left -= g_fSecPerFrame * 200.0f;
		m_rtDraw.right -= g_fSecPerFrame * 200.0f;
	}
	TexPosUpdate();
	return true;
}
void Background::TexPosUpdate()
{
	Texture* pTexture = m_Object.getTexture();
	D3DXVECTOR2 pos = pTexture->getImageSize();
	m_VertexList[0].TexPos = { m_rtDraw.left / pos.x, m_rtDraw.top / pos.y };
	m_VertexList[1].TexPos = { m_rtDraw.right / pos.x, m_rtDraw.top / pos.y };
	m_VertexList[2].TexPos = { m_rtDraw.left / pos.x, m_rtDraw.bottom / pos.y };
	m_VertexList[3].TexPos = { m_rtDraw.right / pos.x, m_rtDraw.bottom / pos.y };
}