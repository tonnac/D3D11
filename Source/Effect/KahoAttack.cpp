#include "KahoAttack.h"

bool KahoAttack::Frame()
{
	Effect::Frame();
	if (m_pEffectSprite->getIndex() < m_iValidIndex)
	{
		for (int i = 0; i < m_VertexList.size(); ++i)
		{
			m_VertexList[i].Pos = { 0.0f,0.0f, 0.5f };
		}
		m_rtCollision.left = 0;
		m_rtCollision.right = 0;
		m_rtCollision.bottom = 0;
		m_rtCollision.top = 0;
	}
	return true;
}

void KahoAttack1::SetPos(ID3D11Device * pDevice, const D3DXVECTOR2& Centerpos)
{
	Effect::SetPos(pDevice, Centerpos);
	m_pEffectSprite = S_Sprite.LoadSprite(L"Kaho", L"Leaf1");
	m_pEffectSprite->setIndex(0);
	m_pEffectSprite->setDivideTime(0.7f);
	m_iValidIndex = 4;
}
void KahoAttack2::SetPos(ID3D11Device * pDevice, const D3DXVECTOR2& Centerpos)
{
	Effect::SetPos(pDevice, Centerpos);
	m_pEffectSprite = S_Sprite.LoadSprite(L"Kaho", L"Leaf2");
	m_pEffectSprite->setIndex(0);
	m_pEffectSprite->setDivideTime(0.7f);
	m_iValidIndex = 4;
}
void KahoAttack3::SetPos(ID3D11Device * pDevice, const D3DXVECTOR2& Centerpos)
{
	Effect::SetPos(pDevice, Centerpos);
	m_pEffectSprite = S_Sprite.LoadSprite(L"Kaho", L"Leaf3");
	m_pEffectSprite->setIndex(0);
	m_pEffectSprite->setDivideTime(0.7f);
	m_iValidIndex = 5;
}
void KahoAirAttack::SetPos(ID3D11Device * pDevice, const D3DXVECTOR2& Centerpos)
{
	Effect::SetPos(pDevice, Centerpos);
	m_pEffectSprite = S_Sprite.LoadSprite(L"Kaho", L"AirLeaf");
	m_pEffectSprite->setIndex(0);
	m_pEffectSprite->setDivideTime(0.7f);
	m_iValidIndex = 2;
}
//void KahoBowAttack::SetPos(ID3D11Device * pDevice, const D3DXVECTOR2& Centerpos)
//{
//	LoadFile(L"PLAYER", L"../../../../data/bmp/KahoColor.bmp", L"../../../../data/bmp/KahoMask.bmp");
//	m_pEffectSprite = S_Sprite.LoadSprite(L"Kaho", L"ArrowS");
//	m_pEffectSprite->setIndex(0);
//	m_pEffectSprite->setDivideTime(0.5f);
//	setRendering(2.8f, INVERSE::DEFAULT);
//	return true;
//}
//bool KahoBowAttack::Frame()
//{
//	m_CenterPos.x += g_fSecPerFrame * m_fSpeed * 7.0f;
//	Effect::Frame();
//	return true;
//}