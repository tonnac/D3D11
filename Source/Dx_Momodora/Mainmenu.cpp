#include "Mainmenu.h"

void Mainmenu::SetPos(const FLOAT& rtLeft, const FLOAT& rtTop, const FLOAT& rtRight, const FLOAT& rtBottom)
{
	m_VertexList[0].TexPos = { rtLeft, rtTop };
	m_VertexList[1].TexPos = { rtRight, rtTop };
	m_VertexList[2].TexPos = { rtLeft, rtBottom };
	m_VertexList[3].TexPos = { rtRight, rtBottom };
	m_VertexList[0].Pos = { 0.0f, 0.0f, 0.5f };
	m_VertexList[1].Pos = { CASTING(FLOAT,g_rtClient.right), 0.0f, 0.5f };
	m_VertexList[2].Pos = { 0.0f, CASTING(FLOAT,g_rtClient.bottom), 0.5f };
	m_VertexList[3].Pos = { m_VertexList[1].Pos.x, m_VertexList[2].Pos.y, 0.5f };
}
bool Mainmenu::Frame()
{
	return true;
}