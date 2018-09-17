#include "EnemyBasicState.h"
#include "Enemy.h"
#include "Player.h"
#include "mSound.h"

EnemyBasicState::EnemyBasicState(Enemy* pEnemy) : State(pEnemy), m_pEnemy(pEnemy)
{
	m_CenterPos = m_pEnemy->getCenterPos();
	m_rtDraw = m_pEnemy->getrtDraw();
	m_rtArea = m_pEnemy->getArea();
	m_rtSight = m_pEnemy->getSight();
	m_rtAttackRange = m_pEnemy->getAttackRange();
	pEnemy->setSpeed(55.0f);
}
bool EnemyBasicState::Render()
{
	return true;
}
bool EnemyBasicState::Release()
{
	return true;
}

EnemyMoveState::EnemyMoveState(Enemy* pEnemy) : EnemyBasicState(pEnemy)
{
	pEnemy->addState(std::string("Move"), this);
}
bool EnemyMoveState::Init()
{
	setSprite(L"Monkey", L"Labeled");
	m_pSprite->setDivideTime(1.0f);
	return true;
}
bool EnemyMoveState::Process(Player * pPlayer)
{
	const RECT playerEffectRT = pPlayer->getEffectObj();
	const RECT collisionRT = *m_pEnemy->getCollisionRt();
	const RECT playerRT = *pPlayer->getCollisionRt();
	FLOAT fSpeed = m_pEnemy->getSpeed();
	INT iDir = m_pEnemy->getDir();
	m_CenterPos->x += iDir * g_fSecPerFrame * fSpeed;
	m_CenterPos->y += g_fSecPerFrame * 5.0f;

	if (iDir == -1)
	{
		m_rtSight->left = collisionRT.left - 100.0f;
		m_rtSight->top = collisionRT.top - 150.0f;
		m_rtSight->right = m_CenterPos->x;
		m_rtSight->bottom = collisionRT.bottom;

		m_rtAttackRange->left = collisionRT.left - 10.0f;
		m_rtAttackRange->top = collisionRT.top;
		m_rtAttackRange->right = collisionRT.left;
		m_rtAttackRange->bottom = collisionRT.bottom;
	}
	else
	{
		m_rtSight->left = m_CenterPos->x;
		m_rtSight->top = collisionRT.top - 150.0f;
		m_rtSight->right = collisionRT.right + 100.0f;
		m_rtSight->bottom = collisionRT.bottom;

		m_rtAttackRange->left = collisionRT.right + 10.0f;
		m_rtAttackRange->top = collisionRT.top;
		m_rtAttackRange->right = collisionRT.right;
		m_rtAttackRange->bottom = collisionRT.bottom;
	}
	if (CollisionClass::RectInRect(playerEffectRT, collisionRT))
	{
		m_pSprite->setIndex(0);
		m_pEnemy->setHP(pPlayer->getDamage());
		if (m_pEnemy->isDead())
		{
			m_pEnemy->Release();
			return true;
		}
		m_pEnemy->setTransition(E_EVENT::BEATTACKED);
		return true;
	}
	if (CollisionClass::RectInRect(playerRT, collisionRT) && pPlayer->getCurrentState() != "Roll" &&
		!pPlayer->isInvincible())
	{
		RECT CollisionArea;
		CollisionArea.left = (playerRT.left < collisionRT.left) ? collisionRT.left : playerRT.left;
		CollisionArea.right = (playerRT.right > collisionRT.right) ? collisionRT.right : playerRT.right;
		CollisionArea.top = (playerRT.top < collisionRT.top) ? collisionRT.top : playerRT.top;
		CollisionArea.bottom = (playerRT.bottom > collisionRT.bottom) ? collisionRT.bottom : playerRT.bottom;

		FloatPoint vDir;
		vDir.x = CollisionArea.right - CollisionArea.left;
		vDir.y = CollisionArea.bottom - CollisionArea.top;

		FLOAT fLength = sqrt(vDir.x * vDir.x + vDir.y * vDir.y);

		vDir.x /= fLength;
		vDir.y /= fLength;

		fLength = sqrt(vDir.x * vDir.x + vDir.y * vDir.y);

		FloatPoint vPos = *pPlayer->getCenterPos();
		vPos.x += vDir.x * g_fSecPerFrame * g_fSpeed;
		vPos.y -= vDir.y * g_fSecPerFrame * g_fSpeed * 50.0f;

		pPlayer->setCenterPos_x(vPos.x);
		pPlayer->setCenterPos_y(vPos.y);
		pPlayer->setHP(m_pEnemy->getDamage());
		
		S_Sound.Play(Effect::HURT);
		pPlayer->setState(L"Hurt");
		return true;
	}
	if (!m_pSprite->Frame())
	{
		m_pSprite->setIndex(0);
	}
	if (CollisionClass::RectInRect(playerRT, *m_rtSight))
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
			m_CenterPos->x += -30;
			m_pEnemy->setRendering(2.8f, INVERSE::LR_ROTATION);
		}
		else
		{
			m_CenterPos->x += 30;
			m_pEnemy->setRendering(2.8f, INVERSE::DEFAULT);
		}
		m_pEnemy->setDir(-1);
	}
	while (pPlayer->hasNext())
	{
		EffectIter it = pPlayer->getEffectIter();
		RECT itrt = *(*it)->getCollisionRt();
		if (CollisionClass::RectInRect(itrt,collisionRT))
		{
			pPlayer->deleteEffect(it);
			m_pEnemy->setHP(1);
			if (m_pEnemy->isDead())
			{
				m_pEnemy->Release();
				return true;
			}
			m_pSprite->setIndex(0);
			m_pEnemy->setTransition(E_EVENT::BEARROWATTACKED);
			return true;
		}
	}
	*m_rtDraw = m_pSprite->getSpriteRt();
	return true;
}

EnemyChaseState::EnemyChaseState(Enemy* pEnemy) : EnemyBasicState(pEnemy)
{
	pEnemy->addState(std::string("Chase"), this);
}
bool EnemyChaseState::Init()
{
	setSprite(L"Monkey", L"Labeled");
	m_pSprite->setDivideTime(1.0f);
	return true;
}
bool EnemyChaseState::Process(Player * pPlayer)
{
	const RECT playerEffectRT = pPlayer->getEffectObj();
	const RECT collisionRT = *m_pEnemy->getCollisionRt();
	const RECT playerRT = *pPlayer->getCollisionRt();
	FLOAT fSpeed = m_pEnemy->getSpeed();
	INT iDir = m_pEnemy->getDir();
	m_CenterPos->x += iDir * g_fSecPerFrame * fSpeed;
	m_CenterPos->y += g_fSecPerFrame * 10.0f;

	if (iDir == -1)
	{
		m_rtSight->left = collisionRT.left - 100.0f;
		m_rtSight->top = collisionRT.top - 150.0f;
		m_rtSight->right = m_CenterPos->x;
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
		m_rtSight->right = m_CenterPos->x;
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
	if (CollisionClass::RectInRect(playerRT, collisionRT) && pPlayer->getCurrentState() != "Roll" &&
		!pPlayer->isInvincible())
	{
		RECT CollisionArea;
		CollisionArea.left = (playerRT.left < collisionRT.left) ? collisionRT.left : playerRT.left;
		CollisionArea.right = (playerRT.right > collisionRT.right) ? collisionRT.right : playerRT.right;
		CollisionArea.top = (playerRT.top < collisionRT.top) ? collisionRT.top : playerRT.top;
		CollisionArea.bottom = (playerRT.bottom > collisionRT.bottom) ? collisionRT.bottom : playerRT.bottom;

		FloatPoint vDir;
		vDir.x = CollisionArea.right - CollisionArea.left;
		vDir.y = CollisionArea.bottom - CollisionArea.top;

		FLOAT fLength = sqrt(vDir.x * vDir.x + vDir.y * vDir.y);

		vDir.x /= fLength;
		vDir.y /= fLength;

		fLength = sqrt(vDir.x * vDir.x + vDir.y * vDir.y);

		FloatPoint vPos = *pPlayer->getCenterPos();
		vPos.x += vDir.x * g_fSecPerFrame * g_fSpeed;
		vPos.y -= vDir.y * g_fSecPerFrame * g_fSpeed * 50.0f;

		pPlayer->setCenterPos_x(vPos.x);
		pPlayer->setCenterPos_y(vPos.y);
		pPlayer->setHP(m_pEnemy->getDamage());
		S_Sound.Play(Effect::HURT);
		pPlayer->setState(L"Hurt");
		return true;
	}
	if (CollisionClass::RectInRect(playerRT, *m_rtAttackRange))
	{
		m_pSprite->setIndex(0);
		m_pEnemy->setTransition(E_EVENT::INATTACKRANGE);
		return true;
	}
	if (CollisionClass::RectInRect(playerEffectRT, collisionRT))
	{
		m_pSprite->setIndex(0);
		m_pEnemy->setHP(pPlayer->getDamage());
		if (m_pEnemy->isDead())
		{
			m_pEnemy->Release();
			return true;
		}
		m_pEnemy->setTransition(E_EVENT::BEATTACKED);
		return true;
	}
	if (CollisionClass::RectInRect(playerRT, *m_rtArea))
	{
		if (m_pEnemy->getDir() == 1 && collisionRT.left > playerRT.right)
		{
			m_pEnemy->setDir(-1);
			m_pEnemy->setRendering(2.8f, INVERSE::LR_ROTATION);
			return true;
		}
		if (m_pEnemy->getDir() == -1 && collisionRT.right < playerRT.left)
		{
			m_pEnemy->setDir(-1);
			m_pEnemy->setRendering(2.8f, INVERSE::DEFAULT);
			return true;
		}
	}
	while (pPlayer->hasNext())
	{
		EffectIter it = pPlayer->getEffectIter();
		RECT itrt = *(*it)->getCollisionRt();
		if (CollisionClass::RectInRect(itrt, collisionRT))
		{
			pPlayer->deleteEffect(it);
			m_pEnemy->setHP(1);
			if (m_pEnemy->isDead())
			{
				m_pEnemy->Release();
				return true;
			}
			m_pSprite->setIndex(0);
			m_pEnemy->setTransition(E_EVENT::BEATTACKED);
			return true;
		}
	}
	if (!CollisionClass::RectInRect(playerRT, *m_rtArea))
	{
		m_pSprite->setIndex(0);
		m_pEnemy->setSpeed(80.0f);
		m_pEnemy->setTransition(E_EVENT::LOSTTARGET);
		return true;
	}
	*m_rtDraw = m_pSprite->getSpriteRt();
	return true;
}

EnemyAttackState::EnemyAttackState(Enemy* pEnemy) : EnemyBasicState(pEnemy)
{
	pEnemy->addState(std::string("Attack"), this);
}
bool EnemyAttackState::Init()
{
	setSprite(L"Monkey", L"Attack");
	m_pSprite->setDivideTime(0.8f);
	return true;
}
bool EnemyAttackState::Process(Player * pPlayer)
{
	const RECT collisionRT = *m_pEnemy->getCollisionRt();
	const RECT playerEffectRT = pPlayer->getEffectObj();
	const RECT playerRT = *pPlayer->getCollisionRt();
	m_CenterPos->y += g_fSecPerFrame * 10.0f;
	if (CollisionClass::RectInRect(playerEffectRT, collisionRT))
	{
		m_pSprite->setIndex(0);
		m_pEnemy->setHP(pPlayer->getDamage());
		if (m_pEnemy->isDead())
		{
			m_pEnemy->Release();
			return true;
		}
		m_pEnemy->setTransition(E_EVENT::BEATTACKED);
		return true;
	}
	if (CollisionClass::RectInRect(playerRT, collisionRT) && pPlayer->getCurrentState() != "Roll" &&
		!pPlayer->isInvincible())
	{
		FloatPoint pPos = *pPlayer->getCenterPos();
		FloatPoint vDir;
		vDir.x = pPos.x - m_CenterPos->x;
		vDir.y = pPos.y - m_CenterPos->y;

		FLOAT fLength = sqrt(vDir.x * vDir.x + vDir.y * vDir.y);

		vDir.x /= fLength;
		vDir.y /= fLength;

		fLength = sqrt(vDir.x * vDir.x + vDir.y * vDir.y);

		FloatPoint vPos = *pPlayer->getCenterPos();
		vPos.x += vDir.x * g_fSecPerFrame * g_fSpeed;
		vPos.y -= vDir.y * g_fSecPerFrame * g_fSpeed * 50.0f;

		pPlayer->setCenterPos_x(vPos.x);
		pPlayer->setCenterPos_y(vPos.y);
		pPlayer->setHP(m_pEnemy->getDamage());
		S_Sound.Play(Effect::HURT);
		pPlayer->setState(L"Hurt");
		return true;
	}
	if (!m_pSprite->Frame())
	{
		m_pSprite->setIndex(0);
		if (!CollisionClass::RectInRect(*pPlayer->getCollisionRt(), *m_rtAttackRange))
		{
			m_pEnemy->setTransition(E_EVENT::OUTATTACKRANGE);
			return true;
		}
	}
	while (pPlayer->hasNext())
	{
		EffectIter it = pPlayer->getEffectIter();
		RECT itrt = *(*it)->getCollisionRt();
		if (CollisionClass::RectInRect(itrt, collisionRT))
		{
			pPlayer->deleteEffect(it);
			m_pEnemy->setHP(1);
			if (m_pEnemy->isDead())
			{
				m_pEnemy->Release();
				return true;
			}
			m_pSprite->setIndex(0);
			m_pEnemy->setTransition(E_EVENT::BEATTACKED);
			return true;
		}
	}
	*m_rtDraw = m_pSprite->getSpriteRt();
	return true;
}

EnemyHitState::EnemyHitState(Enemy* pEnemy) : EnemyBasicState(pEnemy)
{
	pEnemy->addState(std::string("Hit"), this);
}
bool EnemyHitState::Init()
{
	setSprite(L"Monkey", L"Hit");
	m_pSprite->setDivideTime(0.3f);
	return true;
}
bool EnemyHitState::Process(Player * pPlayer)
{
	const RECT playerRT = *pPlayer->getCollisionRt();
	if (!m_pSprite->Frame())
	{
		if (!CollisionClass::RectInRect(playerRT, *m_rtArea))
		{
			m_pSprite->setIndex(0);
			m_pEnemy->setTransition(E_EVENT::LOSTTARGET);
			return true;
		}
		m_pSprite->setIndex(0);
		m_pEnemy->setTransition(E_EVENT::NOHIT);
		return true;
	}
	*m_rtDraw = m_pSprite->getSpriteRt();
	return true;
}

EnemyAngryState::EnemyAngryState(Enemy* pEnemy) : EnemyBasicState(pEnemy)
{
	pEnemy->addState(std::string("Angry"), this);
}
bool EnemyAngryState::Init()
{
	setSprite(L"Monkey", L"Labeled");
	m_pSprite->setDivideTime(0.3f);
	return true;
}
bool EnemyAngryState::Process(Player * pPlayer)
{
	m_pEnemy->setSpeed(120.0f);
	m_fTimer += g_fSecPerFrame;
	const RECT playerEffectRT = pPlayer->getEffectObj();
	const RECT collisionRT = *m_pEnemy->getCollisionRt();
	const RECT playerRT = *pPlayer->getCollisionRt();
	FLOAT fSpeed = m_pEnemy->getSpeed();
	INT iDir = m_pEnemy->getDir();
	m_CenterPos->x += iDir * g_fSecPerFrame * fSpeed;

	if (m_fTimer >= 7.0f)
	{
		m_pSprite->setIndex(0);
		m_fTimer = 0.0f;
		m_pEnemy->setTransition(E_EVENT::LOSTTARGET);
	}

	if (m_pEnemy->getJumpSpeed() < 0.0f)
	{
		m_CenterPos->y += g_fSecPerFrame * 300.0f;
	}
	else
	{
		m_pEnemy->setJumpSpeed(m_pEnemy->getJumpSpeed() - 5);
		m_CenterPos->y -= g_fSecPerFrame * m_pEnemy->getJumpSpeed();
	}
	if (iDir == -1)
	{
		m_rtSight->left = collisionRT.left - 100.0f;
		m_rtSight->top = collisionRT.top - 150.0f;
		m_rtSight->right = m_CenterPos->x;
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
		m_rtSight->right = m_CenterPos->x;
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
	if (CollisionClass::RectInRect(playerRT, collisionRT) && pPlayer->getCurrentState() != "Roll" &&
		!pPlayer->isInvincible())
	{
		RECT CollisionArea;
		CollisionArea.left = (playerRT.left < collisionRT.left) ? collisionRT.left : playerRT.left;
		CollisionArea.right = (playerRT.right > collisionRT.right) ? collisionRT.right : playerRT.right;
		CollisionArea.top = (playerRT.top < collisionRT.top) ? collisionRT.top : playerRT.top;
		CollisionArea.bottom = (playerRT.bottom > collisionRT.bottom) ? collisionRT.bottom : playerRT.bottom;

		FloatPoint vDir;
		vDir.x = CollisionArea.right - CollisionArea.left;
		vDir.y = CollisionArea.bottom - CollisionArea.top;

		FLOAT fLength = sqrt(vDir.x * vDir.x + vDir.y * vDir.y);

		vDir.x /= fLength;
		vDir.y /= fLength;

		fLength = sqrt(vDir.x * vDir.x + vDir.y * vDir.y);

		FloatPoint vPos = *pPlayer->getCenterPos();
		vPos.x += vDir.x * g_fSecPerFrame * g_fSpeed;
		vPos.y -= vDir.y * g_fSecPerFrame * g_fSpeed * 50.0f;

		pPlayer->setCenterPos_x(vPos.x);
		pPlayer->setCenterPos_y(vPos.y);
		pPlayer->setHP(m_pEnemy->getDamage());
		S_Sound.Play(Effect::HURT);
		pPlayer->setState(L"Hurt");
		return true;
	}
	if (CollisionClass::RectInRect(playerRT, *m_rtAttackRange))
	{
		m_pSprite->setIndex(0);
		m_pEnemy->setTransition(E_EVENT::INATTACKRANGE);
		return true;
	}
	if (CollisionClass::RectInRect(playerEffectRT, collisionRT))
	{
		m_pSprite->setIndex(0);
		m_pEnemy->setHP(pPlayer->getDamage());
		if (m_pEnemy->isDead())
		{
			m_pEnemy->Release();
			return true;
		}
		m_pEnemy->setTransition(E_EVENT::BEATTACKED);
		return true;
	}
	while (pPlayer->hasNext())
	{
		EffectIter it = pPlayer->getEffectIter();
		RECT itrt = *(*it)->getCollisionRt();
		if (CollisionClass::RectInRect(itrt, collisionRT))
		{
			pPlayer->deleteEffect(it);
			m_pEnemy->setHP(1);
			if (m_pEnemy->isDead())
			{
				m_pEnemy->Release();
				return true;
			}
			m_pSprite->setIndex(0);
			m_pEnemy->setTransition(E_EVENT::BEARROWATTACKED);
			return true;
		}
	}
	if (m_pEnemy->getDir() == 1 && collisionRT.left > playerRT.right)
	{
		m_pEnemy->setDir(-1);
		m_pEnemy->setRendering(2.8f, INVERSE::LR_ROTATION);
		return true;
	}
	if (m_pEnemy->getDir() == -1 && collisionRT.right < playerRT.left)
	{
		m_pEnemy->setDir(-1);
		m_pEnemy->setRendering(2.8f, INVERSE::DEFAULT);
		return true;
	}
	*m_rtDraw = m_pSprite->getSpriteRt();
	return true;
}

EnemyMoveLocationState::EnemyMoveLocationState(Enemy* pEnemy) : EnemyBasicState(pEnemy)
{
	pEnemy->addState(std::string("MoveLocation"), this);
}
bool EnemyMoveLocationState::Init()
{
	setSprite(L"Monkey", L"Labeled");
	m_pSprite->setDivideTime(0.3f);
	return true;
}
bool EnemyMoveLocationState::Process(Player * pPlayer)
{
	m_pEnemy->setSpeed(120.0f);
	const RECT collisionRT = *m_pEnemy->getCollisionRt();
	FLOAT fSpeed = m_pEnemy->getSpeed();
	INT iDir = m_pEnemy->getDir();
	m_CenterPos->x += iDir * g_fSecPerFrame * fSpeed;

	if (!m_pSprite->Frame())
	{
		m_pSprite->setIndex(0);
	}
	if (m_pEnemy->getJumpSpeed() < 0.0f)
	{
		m_CenterPos->y += g_fSecPerFrame * 300.0f;
	}
	else
	{
		m_pEnemy->setJumpSpeed(m_pEnemy->getJumpSpeed() - 5);
		m_CenterPos->y -= g_fSecPerFrame * m_pEnemy->getJumpSpeed();
	}
	if (m_pEnemy->getDir() == 1 && collisionRT.left > m_rtArea->right)
	{
		m_pEnemy->setDir(-1);
		m_pEnemy->setRendering(2.8f, INVERSE::LR_ROTATION);
		return true;
	}
	if (m_pEnemy->getDir() == -1 && collisionRT.right < m_rtArea->left)
	{
		m_pEnemy->setDir(-1);
		m_pEnemy->setRendering(2.8f, INVERSE::DEFAULT);
		return true;
	}
	if (collisionRT.left > m_rtArea->left)
	{
		m_pSprite->setIndex(0);
		m_pEnemy->setSpeed(50.0f);
		m_pEnemy->setTransition(E_EVENT::ARRIVE);
	}
	*m_rtDraw = m_pSprite->getSpriteRt();
	return true;
}