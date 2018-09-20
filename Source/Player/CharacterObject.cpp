#include "Player.h"
#include "PlayerAttack.h"
#include "PlayerBasicState.h"
#include "Rendering.h"

CharacterObject::CharacterObject() : m_pCurrentState(nullptr), m_iCurrentDir(1), m_bDownable(false),
									m_iJumpNumber(0), m_fJumpSpeed(200.0f), m_fJumpSpeed2(250.0f), m_bLadder(false),
									m_bDead(false), m_bInvincible(false), m_fInvincibleTime(0.0f), m_fZoom(0.0f),
									m_finvencible(5.0f),m_fms(nullptr)
{
	m_fSpeed = 250.0f;
	m_pEffectVector.clear();
	m_EffectIter = m_pEffectVector.begin();
}

bool CharacterObject::Init()
{
	for (auto it : m_pStateList)
	{
		it.second->Init();
	}
	m_pCurrentState = m_pStateList["Fall"];
	Object::Init();
	return true;
}
bool CharacterObject::Frame()
{
	if (m_bInvincible)
	{
		m_fInvincibleTime += g_fSecPerFrame;
		if (m_fInvincibleTime >= m_finvencible)
		{
			m_bInvincible = false;
			m_fInvincibleTime = 0.0f;
		}
	}
	m_pCurrentState->Frame();
	if (!m_pEffectVector.empty())
	{
		for (auto it : m_pEffectVector)
		{
			it->Frame();
		}
	}
	if (m_fZoom > 0.0f)
	{
		m_DrawPos.x = m_CenterPos.x - (m_rtDraw.right * m_fZoom / 2);
		m_DrawPos.y = m_CenterPos.y - (m_rtDraw.bottom * m_fZoom / 2);

		FLOAT d = m_rtDraw.right * m_fZoom / 2;
		FLOAT e = m_rtDraw.bottom * m_fZoom / 2;

		m_rtCollision.left = static_cast<LONG>(m_CenterPos.x - d);
		m_rtCollision.top = static_cast<LONG>(m_CenterPos.y - e);
		m_rtCollision.right = static_cast<LONG>(m_CenterPos.x + d);
		m_rtCollision.bottom = static_cast<LONG>(m_CenterPos.y + e);
	}
	else
	{
		m_DrawPos.x = m_CenterPos.x + (m_rtDraw.right / 2);
		m_DrawPos.y = m_CenterPos.y + (m_rtDraw.bottom / 2);

		m_rtCollision.left = static_cast<LONG>(m_DrawPos.x);
		m_rtCollision.top = static_cast<LONG>(m_DrawPos.y);
		m_rtCollision.right = static_cast<LONG>(m_DrawPos.x + m_rtDraw.right);
		m_rtCollision.bottom = static_cast<LONG>(m_DrawPos.y + m_rtDraw.bottom);
	}
	m_pRendering->Frame();
	return true;
}
bool CharacterObject::Render()
{
	if (m_bInvincible)
	{
		TCHAR csbuffer[256] = { 0, };
		_stprintf_s(csbuffer, _T("%.3f"), m_finvencible - m_fInvincibleTime);
		SetTextColor(g_hOffScreenDC, RGB(255, 255, 255));
		SetBkColor(g_hOffScreenDC, RGB(0, 0, 0));
		TextOut(g_hOffScreenDC, m_CenterPos.x - 10, m_rtCollision.top - 30, csbuffer, wcslen(csbuffer));
	}
	Object::Render();
	if (!m_pEffectVector.empty())
	{
		for (auto it : m_pEffectVector)
		{
			it->Render();
		}
	}
	m_pCurrentState->Render();
	return true;
};
bool CharacterObject::Release()
{
	for (auto it : m_pStateList)
	{
		it.second->Release();
		delete it.second;
	}
	m_pStateList.clear();
	Object::Release();
	return true;
}

void CharacterObject::setState(T_STR szStateName)
{
	std::string cstate(szStateName.begin(), szStateName.end());
	m_pCurrentState = m_pStateList[cstate];
}
void CharacterObject::setDir(const INT& dir)
{
	m_iCurrentDir *= dir;
}
void CharacterObject::setLadder(const bool& bflag)
{
	m_bLadder = bflag;
}
void CharacterObject::setJumpSpeed(const FLOAT& jSpeed, const FLOAT& jSpeed_2)
{
	m_fJumpSpeed = jSpeed;
	m_fJumpSpeed2 = jSpeed_2;
}
void CharacterObject::setJumpNum(const INT& Num)
{
	m_iJumpNumber = Num;
}
bool CharacterObject::isInvincible()
{
	return m_bInvincible;
}
bool CharacterObject::isDead()
{
	return m_bDead;
}
INT	 CharacterObject::getDamage()
{
	return m_Damage;
}
RECT CharacterObject::getEffectObj()
{
	return m_pCurrentState->getEffectRT();
}
std::string	CharacterObject::getCurrentState()
{
	for (auto it : m_pStateList)
	{
		if (it.second == m_pCurrentState)
		{
			return it.first;
		}
	}
	return std::string();
}
void CharacterObject::setDownable(const bool& bdown)
{
	m_bDownable = bdown;
}
void CharacterObject::setHP(const INT& hp)
{
	m_HP -= hp;
	if (m_HP <= 0)
	{
		m_bDead = true;
	}
}
void CharacterObject::setInvincible(const bool& bflag)
{
	m_bInvincible = bflag;
}
void CharacterObject::setDead(const bool& bflag)
{
	m_bDead = bflag;
}
void CharacterObject::setFSM(FinateStateMachine* fsm)
{
	m_fms = fsm;
}
void CharacterObject::setDamage(const INT& damage)
{
	m_Damage = damage;
}
bool CharacterObject::getDownable()
{
	return m_bDownable;
}
INT	CharacterObject::getDir()
{
	return m_iCurrentDir;
}
FLOAT& CharacterObject::getJumpSpeed(const INT& num)
{
	if (num != 0)
	{
		return m_fJumpSpeed2;
	}
	return m_fJumpSpeed;
}
bool CharacterObject::getLadder()
{
	return m_bLadder;
}
bool CharacterObject::isFallState()
{
	auto it = m_pStateList.find("Fall");
	if (it->second == m_pCurrentState)
	{
		return true;
	}
	return false;
}
INT CharacterObject::getJumpNum()
{
	return m_iJumpNumber;
}
FLOAT CharacterObject::getZoom()
{
	return m_fZoom;
}
INT	CharacterObject::getHP()
{
	return m_HP;
}
void CharacterObject::addState(std::string Name, State* state)
{
	m_pStateList.insert(std::make_pair(Name, state));
}
void CharacterObject::addEffect(EffectObj * eobj)
{
	m_pEffectVector.push_back(eobj);
	m_EffectIter = m_pEffectVector.begin();
}
bool CharacterObject::hasNext()
{
	if (m_EffectIter == m_pEffectVector.end())
	{
		m_EffectIter = m_pEffectVector.begin();
		return false;
	}
	return true;
}
void CharacterObject::deleteEffect(EffectIter& iter)
{
	(*iter)->Release();
	delete *iter;
	m_EffectIter = m_pEffectVector.erase(iter);
}
EffectIter CharacterObject::getEffectIter()
{
	return m_EffectIter++;
}
std::string	CharacterObject::setTransition(E_EVENT Event)
{
	m_sCurrestState = m_fms->StateTransition(m_sCurrestState, Event);
	m_pCurrentState = m_pStateList[m_sCurrestState];
	return m_sCurrestState;
}