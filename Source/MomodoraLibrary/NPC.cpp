#include "NPC.h"

NPC::NPC()
{
	m_iDirection[0] = (rand() % 2 > 0 ? 1 : -1);
	m_iDirection[1] = (rand() % 2 > 0 ? 1 : -1);
	m_fSpeed = 200.0f;
}
NPC::~NPC()
{

}

bool NPC::Frame()
{
	if (m_CenterPos.x + m_rtDraw.right / 2 >= g_rtClient.right)
	{
		m_iDirection[0] *= (-1);
		m_CenterPos.x = static_cast<float>(g_rtClient.right - (m_rtDraw.right / 2));
	}
	if (m_CenterPos.x - m_rtDraw.right / 2 <= g_rtClient.left)
	{
		m_iDirection[0] *= (-1);
		m_CenterPos.x = static_cast<float>(g_rtClient.left + (m_rtDraw.right / 2));
	}
	if (m_CenterPos.y + m_rtDraw.bottom / 2 >= g_rtClient.bottom)
	{
		m_iDirection[1] *= (-1);
		m_CenterPos.y = static_cast<float>(g_rtClient.bottom - (m_rtDraw.bottom / 2));
	}
	if (m_CenterPos.y - m_rtDraw.bottom / 2 <= g_rtClient.top)
	{
		m_iDirection[1] *= (-1);
		m_CenterPos.y = static_cast<float>(g_rtClient.top + (m_rtDraw.bottom / 2));
	}

	m_CenterPos.x += m_iDirection[0] * g_fPerSecFrame * m_fSpeed;
	m_CenterPos.y += m_iDirection[1] * g_fPerSecFrame * m_fSpeed;


	m_DrawPos.x = m_CenterPos.x - (m_rtDraw.right / 2);
	m_DrawPos.y = m_CenterPos.y - (m_rtDraw.bottom / 2);

	m_rtCollision.left = static_cast<LONG>(m_DrawPos.x);
	m_rtCollision.top = static_cast<LONG>(m_DrawPos.y);
	m_rtCollision.right = static_cast<LONG>(m_DrawPos.x + m_rtDraw.right);
	m_rtCollision.bottom = static_cast<LONG>(m_DrawPos.y + m_rtDraw.bottom);
	return true;
}