#include "Background.h"
#include "DirectInput.h"

void Background::SetPos(const D2D1_POINT_2F& pos, const D2D1_RECT_F& rect)
{
	Plane_Object::SetPos(pos, rect);
	m_VertexList.resize(4);
	m_VertexList[0].Pos.x = -1.0f;
	m_VertexList[0].Pos.y = 1.0f;
	m_VertexList[1].Pos.x = 1.0f;
	m_VertexList[1].Pos.y = 1.0f;
	m_VertexList[2].Pos.x = -1.0f;
	m_VertexList[2].Pos.y = -1.0f;
	m_VertexList[3].Pos.x = 1.0f;
	m_VertexList[3].Pos.y = -1.0f;
}
bool Background::Frame()
{
	if (S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_HOLD)
	{
		m_rtDraw.left += g_fSecPerFrame * 200.0f;
		m_rtDraw.right += g_fSecPerFrame * 200.0f;
	}
	if (S_Input.getKeyState(DIK_LEFT) == Input::KEYSTATE::KEY_HOLD)
	{
		m_rtDraw.left -= g_fSecPerFrame * 200.0f;
		m_rtDraw.right -= g_fSecPerFrame * 200.0f;
	}
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