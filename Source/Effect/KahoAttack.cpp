#include "KahoAttack.h"

KahoAttack::~KahoAttack()
{
	Plane_Object::Release();
}

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
bool KahoAttack::Release()
{
	return true;
}

bool KahoAttack1::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
	const std::string& VSFunc, const std::string& PSFunc)
{
	m_pEffectSprite = S_Sprite.LoadSprite(L"Kaho", L"Leaf1");
	m_pEffectSprite->setIndex(0);
	m_pEffectSprite->setDivideTime(0.7f);
	m_iValidIndex = 4;
	return Effect::InitSet(pDevice, Name, TexFilepath, ShaderFilepath, VSFunc, PSFunc);
}

bool KahoAttack2::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
	const std::string& VSFunc, const std::string& PSFunc)
{
	m_pEffectSprite = S_Sprite.LoadSprite(L"Kaho", L"Leaf2");
	m_pEffectSprite->setIndex(0);
	m_pEffectSprite->setDivideTime(0.7f);
	m_iValidIndex = 4;
	return Effect::InitSet(pDevice, Name, TexFilepath, ShaderFilepath, VSFunc, PSFunc);
}

bool KahoAttack3::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
	const std::string& VSFunc, const std::string& PSFunc)
{
	m_pEffectSprite = S_Sprite.LoadSprite(L"Kaho", L"Leaf3");
	m_pEffectSprite->setIndex(0);
	m_pEffectSprite->setDivideTime(0.7f);
	m_iValidIndex = 5;
	return Effect::InitSet(pDevice, Name, TexFilepath, ShaderFilepath, VSFunc, PSFunc);
}

bool KahoAirAttack::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
	const std::string& VSFunc, const std::string& PSFunc)
{
	m_pEffectSprite = S_Sprite.LoadSprite(L"Kaho", L"AirLeaf");
	m_pEffectSprite->setIndex(0);
	m_pEffectSprite->setDivideTime(0.7f);
	m_iValidIndex = 2;
	return Effect::InitSet(pDevice, Name, TexFilepath, ShaderFilepath, VSFunc, PSFunc);
}

bool KahoBowAttack::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
	const std::string& VSFunc, const std::string& PSFunc)
{
	m_pEffectSprite = S_Sprite.LoadSprite(L"Kaho", L"ArrowS");
	m_pEffectSprite->setIndex(0);
	m_pEffectSprite->setDivideTime(0.5f);
	return Effect::InitSet(pDevice, Name, TexFilepath, ShaderFilepath, VSFunc, PSFunc);
}
bool KahoBowAttack::Frame()
{
	m_Centerpos.x += g_fSecPerFrame * g_fSpeed * 7.0f * m_iDir;
	Effect::Frame();
	return true;
}