#include "Bar.h"
#include "Player.h"

bool Bar::Frame()
{
	m_LengthDiff.x = (m_VertexList[1].TexPos.x - m_VertexList[0].TexPos.x) * 0.5f;
	m_LengthDiff.y = (m_VertexList[2].TexPos.y - m_VertexList[0].TexPos.y) * 0.5f;
	return UI::Frame();
}


bool HPBar::Frame()
{
	m_LengthDiff.x = (m_VertexList[1].TexPos.x - m_VertexList[0].TexPos.x) * 0.5f;
	m_LengthDiff.y = (m_VertexList[2].TexPos.y - m_VertexList[0].TexPos.y) * 0.5f;
	UI::Frame();
	m_VertexList[1].Pos.x = m_VertexList[0].Pos.x + (m_LengthDiff.x * 2) * g_Player->getHP() / 100;
	m_VertexList[3].Pos.x = m_VertexList[1].Pos.x;
	return true;
}