#include "ProgressBar.h"

ProgressBar::ProgressBar() : m_Percent(nullptr), m_MaxWidth(0.0f)
{}

void ProgressBar::SetTexPos(const D3DXVECTOR4& DrawVec)
{
	m_MaxWidth = DrawVec.z - DrawVec.x;
	Bar::SetTexPos(DrawVec);
}

bool ProgressBar::Frame()
{
	m_VertexList[0].Pos = { m_Centerpos.x - (m_MaxWidth * 0.5f), m_Centerpos.y - (m_VertexList[2].TexPos.y - m_VertexList[0].TexPos.y) * 0.5f , 0.5f};
	m_VertexList[1].Pos = { m_VertexList[0].Pos.x + (m_MaxWidth * (*m_Percent)), m_VertexList[0].Pos.y, 0.5f };
	m_VertexList[2].Pos = { m_VertexList[0].Pos.x, m_Centerpos.y + (m_VertexList[2].TexPos.y - m_VertexList[0].TexPos.y) * 0.5f, 0.5f };
	m_VertexList[3].Pos = { m_VertexList[1].Pos.x, m_VertexList[2].Pos.y, 0.5f };
	return true;
}

void ProgressBar::setPercent(FLOAT* per)
{
	m_Percent = per;
}