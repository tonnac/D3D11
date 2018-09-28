#include "Background.h"
#include "DirectInput.h"
#include "DirectWrite.h"

FLOAT g_fMapWidth = 0.0f;

Background::Background()
{
}

void Background::SetPos(const FLOAT& rtLeft, const FLOAT& rtTop, const FLOAT& MaxWidth, const FLOAT& MaxHeight)
{
	BasicSet(rtLeft, MaxWidth);
	g_fMapWidth = (m_MaxSize.y - m_MaxSize.x) * g_rtClient.right / g_fImageWidth;
	Plane_Object::SetTexPos({ rtLeft, rtTop, rtLeft + g_fImageWidth, rtTop + g_fImageHeight });
}
void Background::SetInversePos(const FLOAT& rtLeft, const FLOAT& rtTop, const FLOAT& MaxWidth, const FLOAT& MaxHeight)
{
	BasicSet(rtLeft, MaxWidth);
	Plane_Object::SetTexPos({ MaxWidth - g_fImageWidth, rtTop, MaxWidth, rtTop + g_fImageHeight });
}
bool Background::Frame()
{
	m_ConstantData.Util.z = cos(g_fGameTime) * 0.5f + 0.5f;
	return true;
}
bool Background::Scroll(const FLOAT& pos)
{
	if (pos < 0.0f)
	{
		if (m_VertexList[0].TexPos.x > m_MaxSize.x)
		{
			for (int i = 0; i < m_VertexList.size(); ++i)
			{
				m_VertexList[i].TexPos.x += pos;
			}
			return true;
		}
	}
	else
	{
		if (m_VertexList[1].TexPos.x < m_MaxSize.y)
		{
			for (int i = 0; i < m_VertexList.size(); ++i)
			{
				m_VertexList[i].TexPos.x += pos;
			}
			return true;
		}
	}
	return false;
}
//COL Background::CanScroll()
//{
//	COL type = COL::NONE;
//	type += CanScrollLeft();
//	type += CanScrollRight();
//	return type;
//}
void Background::BasicSet(const FLOAT& rtLeft, const FLOAT& MaxWidth)
{
	m_MaxSize.x = rtLeft;
	m_MaxSize.y = MaxWidth;
	m_VertexList[0].Pos = { 0.0f, 0.0f, 0.5f };
	m_VertexList[1].Pos = { CASTING(FLOAT,g_rtClient.right), 0.0f, 0.5f };
	m_VertexList[2].Pos = { 0.0f, CASTING(FLOAT,g_rtClient.bottom), 0.5f };
	m_VertexList[3].Pos = { m_VertexList[1].Pos.x, m_VertexList[2].Pos.y, 0.5f };
	m_Centerpos = { g_rtClient.right * 0.5f, g_rtClient.bottom * 0.5f };
}
bool Background::CanScrollLeft()
{
	if (m_VertexList[0].TexPos.x > m_MaxSize.x)
	{
		return true;
	}
	return false;
}
bool Background::CanScrollRight()
{
	if (m_VertexList[1].TexPos.x < m_MaxSize.y)
	{
		return true;
	}
	return false;
}