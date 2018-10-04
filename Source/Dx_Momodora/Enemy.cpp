#include "Enemy.h"
#include "Player.h"
#include "mSound.h"

Enemy::Enemy() : m_fTimer(0.0f), m_Damage(8)
{
	EnemyBasicState * state = new EnemyMoveState(this);
	state = new EnemyAttackState(this);
	state = new EnemyHitState(this);
	state = new EnemyChaseState(this);
	state = new EnemyAngryState(this);
	state = new EnemyMoveLocationState(this);
	m_HP = 50;
	m_fSpeed = 55.0f;
	m_pCurrentState = m_StateList[L"Move"];
}
Enemy::~Enemy()
{}

bool Enemy::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
					const std::string& VSFunc, const std::string& PSFunc)
{
	for (int i = 0; i < 3; ++i)
	{
		m_Line[i].InitSet(pDevice, L"Terrain", Filepath::m_Txtpath[L"Shader"], "VS", "TerrainPS");
	}
	return Character::InitSet(pDevice, Name, TexFilepath, ShaderFilepath, VSFunc, PSFunc);
}
bool Enemy::Init()
{
	Character::Init();
	m_pCurrentState = m_StateList[L"Move"];
	m_CurState = L"Move";
	m_ConstantData.Util.z = 1.0f;
	return true;
}
bool Enemy::Frame()
{
	if (isDead == true)
	{
		m_ConstantData.Util.z -= g_fSecPerFrame;
		if (m_ConstantData.Util.z <= 0.0f)
		{
			return false;
		}
		return true;
	}
	Collision();
	m_Line[0].setVertex(m_rtArea);
	m_Line[1].setVertex(m_rtAttackRange);
	m_Line[2].setVertex(m_rtSight);
	return Character::Frame();
}
bool Enemy::Render(ID3D11DeviceContext* pContext)
{
	if (g_DebugMode)
	{
		for (int i = 0; i < 3; ++i)
		{
			m_Line[i].Render(pContext);
		}
	}
	return Character::Render(pContext);
}
bool Enemy::Release()
{
	for (int i = 0; i < 3; ++i)
	{
		m_Line[i].Release();
	}
	return Character::Release();
}
D2D1_RECT_F* Enemy::getArea()
{
	return &m_rtArea;
}
D2D1_RECT_F* Enemy::getSight()
{
	return &m_rtSight;
}
D2D1_RECT_F* Enemy::getAttackRange()
{
	return &m_rtAttackRange;
}
void Enemy::setHP(const FLOAT& iVal)
{
	m_HP -= CASTING(INT, iVal);
	if (m_HP <= 0)
	{
		setDead();
	}
}
void Enemy::setArea(const D2D1_RECT_F& area)
{
	m_rtArea = area;
}
void Enemy::setSight(const D2D1_RECT_F& sight)
{
	m_rtSight = sight;
}
void Enemy::setAttackRange(const D2D1_RECT_F& arange)
{
	m_rtAttackRange = arange;
}
void Enemy::setSpeed(const FLOAT& fSpeed)
{
	m_fSpeed = fSpeed;
}
std::tstring Enemy::setTransition(E_EVENT Event)
{
	m_CurState = m_fms->StateTransition(m_CurState, Event);
	m_pCurrentState = m_StateList[m_CurState];
	return m_CurState;
}
void Enemy::setDir(const INT& iDir)
{
	m_iDir *= iDir;
}
void Enemy::setFSM(FinateStateMachine* pFSM)
{
	m_fms = pFSM;
}
COL	Enemy::Collision(std::shared_ptr<Object> pObject, FLOAT* ColSize)
{
	return Character::Collision(pObject,ColSize);
}
COL	Enemy::Collision()
{
	FLOAT fSize;
	COL col = Collision(g_Player, &fSize);
	if (col != COL::NONE && g_Player->getCurrentState() != L"Roll" && g_Player->isInvincible() == false && m_CurState != L"Hit")
	{
		D2D1_RECT_F CollisionArea;
		D2D1_RECT_F playerRT = g_Player->getCollisionRT();
		CollisionArea.left = (playerRT.left < m_rtCollision.left) ? m_rtCollision.left : playerRT.left;
		CollisionArea.right = (playerRT.right > m_rtCollision.right) ? m_rtCollision.right : playerRT.right;
		CollisionArea.top = (playerRT.top < m_rtCollision.top) ? m_rtCollision.top : playerRT.top;
		CollisionArea.bottom = (playerRT.bottom > m_rtCollision.bottom) ? m_rtCollision.bottom : playerRT.bottom;

		D3DXVECTOR2 vDir;
		vDir.x = CollisionArea.right - CollisionArea.left;
		vDir.y = CollisionArea.bottom - CollisionArea.top;

		D3DXVECTOR2 nDir;
		D3DXVec2Normalize(&nDir, &vDir);

		g_Player->setHP(CASTING(FLOAT,m_Damage));

		S_Sound.PlayEffect(Effect_Snd::HURT);
		g_Player->setState(L"Hurt");
		return col;
	}
	return COL::NONE;
}
bool Enemy::Scroll(const FLOAT& pos)
{
	m_Centerpos.x += -pos;
	m_rtArea.left += -pos;
	m_rtArea.right += -pos;
	m_rtSight.left += -pos;
	m_rtSight.right += -pos;
	m_rtAttackRange.left += -pos;
	m_rtAttackRange.right += -pos;
	return true;
}
//bool Enemy::MoveScrollObj(const LONG& size)
//{
//	m_fScroll = (g_fSecPerFrame * g_fSpeed);
//	if (size < 0)
//	{
//		m_rtArea.left			+= m_fScroll;
//		m_rtArea.right			+= m_fScroll;
//		m_rtSight.left			+= m_fScroll;
//		m_rtSight.right			+= m_fScroll;
//		m_rtAttackRange.left	+= m_fScroll;
//		m_rtAttackRange.right	+= m_fScroll;
//	}
//	else
//	{
//		m_rtArea.left			-= m_fScroll;
//		m_rtArea.right			-= m_fScroll;
//		m_rtSight.left			-= m_fScroll;
//		m_rtSight.right			-= m_fScroll;
//		m_rtAttackRange.left	-= m_fScroll;
//		m_rtAttackRange.right	-= m_fScroll;
//	}
//	return true;
//}
