#include "KahoAttack.h"
#include "DirectInput.h"
#include "Player.h"
PlayerEffect::PlayerEffect() : isEnd(true)
{}
PlayerEffect::~PlayerEffect()
{
	Release();
}
bool PlayerEffect::getEnd() const
{
	return isEnd;
}
void PlayerEffect::setDir(const INT& Dir)
{
	m_iDir = Dir;
}
INT	PlayerEffect::getDamage() const
{
	return m_iDamage;
}
COL	PlayerEffect::Collision(std::shared_ptr<Object> pObject, FLOAT* ColSize)
{
	FLOAT fSize;
	return Effect::Collision(pObject, &fSize);
}
COL PlayerEffect::Collision(EnemyPTR pEnemy)
{
	FLOAT fSize;
	COL col = Collision(pEnemy, &fSize);
	D2D1_RECT_F EnemyRT = pEnemy->getCollisionRT();
	if (col != COL::NONE && pEnemy->getCurrentState() != L"Hit")
	{
		D2D1_RECT_F CollisionArea;
		CollisionArea.left = (EnemyRT.left < m_rtCollision.left) ? m_rtCollision.left : EnemyRT.left;
		CollisionArea.right = (EnemyRT.right > m_rtCollision.right) ? m_rtCollision.right : EnemyRT.right;
		CollisionArea.top = (EnemyRT.top < m_rtCollision.top) ? m_rtCollision.top : EnemyRT.top;
		CollisionArea.bottom = (EnemyRT.bottom > m_rtCollision.bottom) ? m_rtCollision.bottom : EnemyRT.bottom;

		D3DXVECTOR2 vDir;
		vDir.x = CollisionArea.right - CollisionArea.left;
		vDir.y = CollisionArea.bottom - CollisionArea.top;

		D3DXVECTOR2 nDir;
		D3DXVec2Normalize(&nDir, &vDir);

		FLOAT fScale = (g_Player->isBuff() == true) ? 1.5f : 1.0f;
		pEnemy->setHP(m_iDamage * fScale);

		pEnemy->setTransition(E_EVENT::BEATTACKED);
		return col;
	}
	return COL::NONE;
}

KahoAttack::KahoAttack()
{}
bool KahoAttack::Frame()
{
	isEnd = Effect::Frame();
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
	return isEnd;
}

bool KahoAttack1::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
	const std::string& VSFunc, const std::string& PSFunc)
{
	m_iDamage = 12;
	m_pEffectSprite = S_Sprite.LoadSprite(L"Kaho", L"Leaf1");
	m_pEffectSprite->setIndex(0);
	m_pEffectSprite->setDivideTime(0.7f);
	m_iValidIndex = 3;
	return Effect::InitSet(pDevice, Name, TexFilepath, ShaderFilepath, VSFunc, PSFunc);
}

bool KahoAttack2::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
	const std::string& VSFunc, const std::string& PSFunc)
{
	m_iDamage = 17;
	m_pEffectSprite = S_Sprite.LoadSprite(L"Kaho", L"Leaf2");
	m_pEffectSprite->setIndex(0);
	m_pEffectSprite->setDivideTime(0.7f);
	m_iValidIndex = 3;
	return Effect::InitSet(pDevice, Name, TexFilepath, ShaderFilepath, VSFunc, PSFunc);
}

bool KahoAttack3::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
	const std::string& VSFunc, const std::string& PSFunc)
{
	m_iDamage = 26;
	m_pEffectSprite = S_Sprite.LoadSprite(L"Kaho", L"Leaf3");
	m_pEffectSprite->setIndex(0);
	m_pEffectSprite->setDivideTime(0.7f);
	m_iValidIndex = 5;
	return Effect::InitSet(pDevice, Name, TexFilepath, ShaderFilepath, VSFunc, PSFunc);
}

bool KahoAirAttack::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
	const std::string& VSFunc, const std::string& PSFunc)
{
	m_iDamage = 20;
	m_pEffectSprite = S_Sprite.LoadSprite(L"Kaho", L"AirLeaf");
	m_pEffectSprite->setIndex(0);
	m_pEffectSprite->setDivideTime(0.6f);
	m_iValidIndex = 2;
	return Effect::InitSet(pDevice, Name, TexFilepath, ShaderFilepath, VSFunc, PSFunc);
}

bool KahoBowAttack::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
	const std::string& VSFunc, const std::string& PSFunc)
{
	m_iDamage = 5;
	m_pEffectSprite = S_Sprite.LoadSprite(L"Kaho", L"ArrowS");
	m_pEffectSprite->setIndex(0);
	m_pEffectSprite->setDivideTime(0.5f);
	return Effect::InitSet(pDevice, Name, TexFilepath, ShaderFilepath, VSFunc, PSFunc);
}
COL	KahoBowAttack::Collision(EnemyPTR pEnemy)
{
	COL col = PlayerEffect::Collision(pEnemy);
	if (col > COL::NONE)
	{
		isEnd = false;
	}
	return col;
}
bool KahoBowAttack::Frame()
{
	m_Centerpos.x += g_fSecPerFrame * g_fSpeed * 7.0f * m_iDir;

	SetTexPos(m_pEffectSprite->getSpriteVt());

	m_LengthDiff.x = (m_VertexList[1].TexPos.x - m_VertexList[0].TexPos.x) * m_fScale * 0.5f;
	m_LengthDiff.y = (m_VertexList[2].TexPos.y - m_VertexList[0].TexPos.y) * m_fScale * 0.5f;

	if (m_Centerpos.x - m_LengthDiff.x >= g_fMapWidth || m_Centerpos.x + m_LengthDiff.x < 0)
	{
		isEnd = false;
		return isEnd;
	}

	if (m_iDir == -1)
	{
		reverseSet();
	}

	return Plane_Object::Frame();
}