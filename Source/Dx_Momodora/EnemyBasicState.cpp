#include "EnemyBasicState.h"
#include "Enemy.h"
#include "Player.h"
#include "mSound.h"

EnemyBasicState::EnemyBasicState(Enemy* pEnemy) : m_pEnemy(pEnemy)
{
	m_rtArea = m_pEnemy->getArea();
	m_rtSight = m_pEnemy->getSight();
	m_rtAttackRange = m_pEnemy->getAttackRange();
}
bool EnemyBasicState::Frame()
{
	m_pEnemy->SetTexPos(m_pSprite->getSpriteVt());
	if (m_pSprite->Frame() == false)
	{
		m_pSprite->setIndex(m_iResetindex);
		return false;
	}
	return true;
}
bool EnemyBasicState::Release()
{
	return true;
}
bool EnemyBasicState::Collision(D2D1_RECT_F* Rect)
{
	D2D1_RECT_F ColRT = g_Player->getCollisionRT();
	D3DXVECTOR2 Cen = g_Player->getCenterPos();

	D3DXVECTOR2 Center = {(Rect->right + Rect->left) * 0.5f, (Rect->bottom + Rect->top) * 0.5f};

	FLOAT xDiff = abs(Center.x - Cen.x);
	FLOAT yDiff = abs(Center.y - Cen.y);

	FLOAT xLen1 = Rect->right - Center.x;
	FLOAT yLen1 = Rect->bottom - Center.y;

	FLOAT xLen2 = ColRT.right - Cen.x;
	FLOAT yLen2 = ColRT.bottom - Cen.y;

	if (xLen1 + xLen2 >= xDiff && yLen1 + yLen2 >= yDiff)
	{
		return true;
	}
	return false;
}
EnemyMoveState::EnemyMoveState(Enemy* pEnemy) : EnemyBasicState(pEnemy)
{
	pEnemy->AddState(std::tstring(L"Move"), this);
}
bool EnemyMoveState::Init()
{
	setSprite(L"Monkey", L"Labeled");
	m_pSprite->setDivideTime(1.0f);
	return true;
}
bool EnemyMoveState::Frame()
{
	FLOAT fSpeed = m_pEnemy->getSpeed();
	INT iDir = m_pEnemy->getDir();
	D2D1_RECT_F ColRT = m_pEnemy->getCollisionRT();
	D3DXVECTOR2 Cen = m_pEnemy->getCenterPos();
	m_pEnemy->MoveCenterPos({ iDir * g_fSecPerFrame * fSpeed , g_fSecPerFrame * 50.0f });
	if (iDir == -1)
	{
		m_rtSight->left = ColRT.left - 100.0f;
		m_rtSight->top = ColRT.top - 150.0f;
		m_rtSight->right = Cen.x;
		m_rtSight->bottom = ColRT.bottom;

		m_rtAttackRange->left = ColRT.left - 10.0f;
		m_rtAttackRange->top = ColRT.top;
		m_rtAttackRange->right = ColRT.left;
		m_rtAttackRange->bottom = ColRT.bottom;
	}
	else
	{
		m_rtSight->left = Cen.x;
		m_rtSight->top = ColRT.top - 150.0f;
		m_rtSight->right = ColRT.right + 100.0f;
		m_rtSight->bottom = ColRT.bottom;

		m_rtAttackRange->left = ColRT.right + 10.0f;
		m_rtAttackRange->top = ColRT.top;
		m_rtAttackRange->right = ColRT.right;
		m_rtAttackRange->bottom = ColRT.bottom;
	}

	if (Collision(m_rtSight))
	{
		m_pSprite->setIndex(0);
		m_pEnemy->setSpeed(140.0f);
		m_pEnemy->setTransition(E_EVENT::FINDTARGET);
		return true;
	}
	if (m_rtArea->right <= m_rtSight->right || m_rtArea->left >= m_rtSight->left)
	{
		if (m_pEnemy->getDir() == 1)
		{
			m_pEnemy->MoveCenterPos({ -30.0f, 0.0f });
		}
		else
		{
			m_pEnemy->MoveCenterPos({ 30.0f,0.0f });
		}
		m_pEnemy->setDir(-1);
	}

	EnemyBasicState::Frame();
	return true;
}

EnemyChaseState::EnemyChaseState(Enemy* pEnemy) : EnemyBasicState(pEnemy)
{
	pEnemy->AddState(std::tstring(L"Chase"), this);
}
bool EnemyChaseState::Init()
{
	setSprite(L"Monkey", L"Labeled");
	m_pSprite->setDivideTime(1.0f);
	return true;
}
bool EnemyChaseState::Frame()
{
	FLOAT fSpeed = m_pEnemy->getSpeed();
	INT iDir = m_pEnemy->getDir();
	m_pEnemy->MoveCenterPos({ iDir * g_fSecPerFrame * fSpeed, g_fSecPerFrame * 10.0f });
	D2D1_RECT_F collisionRT = m_pEnemy->getCollisionRT();
	D3DXVECTOR2 Center = m_pEnemy->getCenterPos();
	if (iDir == -1)
	{
		m_rtSight->left = collisionRT.left - 100.0f;
		m_rtSight->top = collisionRT.top - 150.0f;
		m_rtSight->right = Center.x;
		m_rtSight->bottom = collisionRT.bottom;

		m_rtAttackRange->left = collisionRT.left - 5.0f;
		m_rtAttackRange->top = collisionRT.top;
		m_rtAttackRange->right = collisionRT.left;
		m_rtAttackRange->bottom = collisionRT.bottom;
	}
	else
	{
		m_rtSight->left = collisionRT.right + 100.0f;
		m_rtSight->top = collisionRT.top - 150.0f;
		m_rtSight->right = Center.x;
		m_rtSight->bottom = collisionRT.bottom;

		m_rtAttackRange->left = collisionRT.right + 5.0f;
		m_rtAttackRange->top = collisionRT.top;
		m_rtAttackRange->right = collisionRT.right;
		m_rtAttackRange->bottom = collisionRT.bottom;
	}

	if (Collision(m_rtAttackRange))
	{
		m_pSprite->setIndex(0);
		m_pEnemy->setTransition(E_EVENT::INATTACKRANGE);
		return true;
	}
	if (Collision(m_rtArea))
	{
		D2D1_RECT_F playerRT = g_Player->getCollisionRT();
		if (m_pEnemy->getDir() == 1 && collisionRT.left > playerRT.right)
		{
			m_pEnemy->setDir(-1);
			return true;
		}
		if (m_pEnemy->getDir() == -1 && collisionRT.right < playerRT.left)
		{
			m_pEnemy->setDir(-1);
			return true;
		}
	}
	if (Collision(m_rtArea) == false)
	{
		m_pSprite->setIndex(0);
		m_pEnemy->setSpeed(80.0f);
		m_pEnemy->setTransition(E_EVENT::LOSTTARGET);
		return true;
	}
	EnemyBasicState::Frame();
	return true;
}

EnemyAttackState::EnemyAttackState(Enemy* pEnemy) : EnemyBasicState(pEnemy)
{
	pEnemy->AddState(std::tstring(L"Attack"), this);
}
bool EnemyAttackState::Init()
{
	setSprite(L"Monkey", L"Attack");
	m_pSprite->setDivideTime(0.8f);
	return true;
}
bool EnemyAttackState::Frame()
{
	D2D1_RECT_F collisionRT = m_pEnemy->getCollisionRT();
	D2D1_RECT_F playerRT = g_Player->getCollisionRT();
	m_pEnemy->MoveCenterPos({ 0.0f,g_fSecPerFrame * 10.0f });
	if (EnemyBasicState::Frame() == false)
	{
		m_pSprite->setIndex(0);
		if (Collision(m_rtAttackRange) == false)
		{
			m_pEnemy->setTransition(E_EVENT::OUTATTACKRANGE);
			return true;
		}
	}
	return true;
}

EnemyHitState::EnemyHitState(Enemy* pEnemy) : EnemyBasicState(pEnemy)
{
	pEnemy->AddState(std::tstring(L"Hit"), this);
}
bool EnemyHitState::Init()
{
	setSprite(L"Monkey", L"Hit");
	m_pSprite->setDivideTime(0.3f);
	return true;
}
bool EnemyHitState::Frame()
{
	if (EnemyBasicState::Frame() == false)
	{
		if (Collision(m_rtArea) == false)
		{
			m_pSprite->setIndex(0);
			m_pEnemy->setTransition(E_EVENT::LOSTTARGET);
			return true;
		}
		m_pSprite->setIndex(0);
		m_pEnemy->setTransition(E_EVENT::NOHIT);
		return true;
	}
	return true;
}

EnemyAngryState::EnemyAngryState(Enemy* pEnemy) : EnemyBasicState(pEnemy)
{
	pEnemy->AddState(std::tstring(L"Angry"), this);
}
bool EnemyAngryState::Init()
{
	setSprite(L"Monkey", L"Labeled");
	m_pSprite->setDivideTime(0.3f);
	return true;
}
bool EnemyAngryState::Frame()
{
	m_pEnemy->setSpeed(120.0f);
	m_fTimer += g_fSecPerFrame;
	D2D1_RECT_F collisionRT = m_pEnemy->getCollisionRT();
	D2D1_RECT_F playerRT = g_Player->getCollisionRT();
	D3DXVECTOR2 Center = m_pEnemy->getCenterPos();
	FLOAT fSpeed = m_pEnemy->getSpeed();
	INT iDir = m_pEnemy->getDir();
	m_pEnemy->MoveCenterPos({ iDir * g_fSecPerFrame * fSpeed , g_fSecPerFrame * 150.0f });

	if (m_fTimer >= 7.0f)
	{
		m_pSprite->setIndex(0);
		m_fTimer = 0.0f;
		m_pEnemy->setTransition(E_EVENT::LOSTTARGET);
		return true;
	}
	if (iDir == -1)
	{
		m_rtSight->left = collisionRT.left - 100.0f;
		m_rtSight->top = collisionRT.top - 150.0f;
		m_rtSight->right = Center.x;
		m_rtSight->bottom = collisionRT.bottom;

		m_rtAttackRange->left = collisionRT.left - 5.0f;
		m_rtAttackRange->top = collisionRT.top;
		m_rtAttackRange->right = collisionRT.left;
		m_rtAttackRange->bottom = collisionRT.bottom;
	}
	else
	{
		m_rtSight->left = collisionRT.right + 100.0f;
		m_rtSight->top = collisionRT.top - 150.0f;
		m_rtSight->right = Center.x;
		m_rtSight->bottom = collisionRT.bottom;

		m_rtAttackRange->left = collisionRT.right + 5.0f;
		m_rtAttackRange->top = collisionRT.top;
		m_rtAttackRange->right = collisionRT.right;
		m_rtAttackRange->bottom = collisionRT.bottom;
	}

	if (!m_pSprite->Frame())
	{
		m_pSprite->setIndex(0);
	}
	if (Collision(m_rtAttackRange))
	{
		m_pSprite->setIndex(0);
		m_pEnemy->setTransition(E_EVENT::INATTACKRANGE);
		return true;
	}
	if (m_pEnemy->getDir() == 1 && collisionRT.left > playerRT.right)
	{
		m_pEnemy->setDir(-1);
		return true;
	}
	if (m_pEnemy->getDir() == -1 && collisionRT.right < playerRT.left)
	{
		m_pEnemy->setDir(-1);
		return true;
	}
	EnemyBasicState::Frame();
	return true;
}

EnemyMoveLocationState::EnemyMoveLocationState(Enemy* pEnemy) : EnemyBasicState(pEnemy)
{
	pEnemy->AddState(std::tstring(L"MoveLocation"), this);
}
bool EnemyMoveLocationState::Init()
{
	setSprite(L"Monkey", L"Labeled");
	m_pSprite->setDivideTime(0.3f);
	return true;
}
bool EnemyMoveLocationState::Frame()
{
	m_pEnemy->setSpeed(120.0f);
	D2D1_RECT_F collisionRT = m_pEnemy->getCollisionRT();
	FLOAT fSpeed = m_pEnemy->getSpeed();
	INT iDir = m_pEnemy->getDir();
	m_pEnemy->MoveCenterPos({ iDir * g_fSecPerFrame * fSpeed ,0.0f });

	if (m_pEnemy->getDir() == 1 && collisionRT.left > m_rtArea->right)
	{
		m_pEnemy->setDir(-1);
		return true;
	}
	if (m_pEnemy->getDir() == -1 && collisionRT.right < m_rtArea->left)
	{
		m_pEnemy->setDir(-1);
		return true;
	}
	if (collisionRT.left > m_rtArea->left)
	{
		m_pSprite->setIndex(0);
		m_pEnemy->setSpeed(50.0f);
		m_pEnemy->setTransition(E_EVENT::ARRIVE);
		return true;
	}
	EnemyBasicState::Frame();
	return true;
}