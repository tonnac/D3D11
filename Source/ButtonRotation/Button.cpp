#include "Button.h"

Button::Button()
{
}
void Button::SetPos(const D2D1_POINT_2F& CenterPos, const D2D1_POINT_2F& Size)
{
	fWidth = Size.x;
	fHeight = Size.y;
	m_Centerpos = CenterPos;

	for (int i = 0; i < 9; ++i)
	{
		pButton[i]->m_VertexList.resize(4);
	}

	pButton[0]->fWidth = Size.x * 0.1f;
	pButton[0]->fHeight = Size.y * 0.1f;
	pButton[0]->m_Centerpos.x = CenterPos.x - Size.x * 0.5f + pButton[0]->fWidth * 0.5f;
	pButton[0]->m_Centerpos.y = CenterPos.y - Size.y * 0.5f + pButton[0]->fHeight * 0.5f;

	pButton[1]->fWidth = Size.x * 0.8f;
	pButton[1]->fHeight = pButton[0]->fHeight;
	pButton[1]->m_Centerpos.x = pButton[0]->m_Centerpos.x + pButton[0]->fWidth * 0.5f + pButton[1]->fWidth * 0.5f;
	pButton[1]->m_Centerpos.y = pButton[0]->m_Centerpos.y;

	pButton[2]->fWidth = Size.x * 0.1f;
	pButton[2]->fHeight = pButton[0]->fHeight;
	pButton[2]->m_Centerpos.x = pButton[1]->m_Centerpos.x + pButton[1]->fWidth * 0.5f + pButton[2]->fWidth * 0.5f;
	pButton[2]->m_Centerpos.y = pButton[1]->m_Centerpos.y;
	/////////////////////////////////////////////////////
	pButton[3]->fWidth = pButton[0]->fWidth;
	pButton[3]->fHeight = Size.y * 0.8f;
	pButton[3]->m_Centerpos.x = pButton[0]->m_Centerpos.x;
	pButton[3]->m_Centerpos.y = pButton[0]->m_Centerpos.y + pButton[0]->fHeight * 0.5f + pButton[3]->fHeight * 0.5f;

	pButton[4]->fWidth = pButton[1]->fWidth;
	pButton[4]->fHeight = pButton[3]->fHeight;
	pButton[4]->m_Centerpos.x = pButton[1]->m_Centerpos.x;
	pButton[4]->m_Centerpos.y = pButton[3]->m_Centerpos.y;

	pButton[5]->fWidth = pButton[2]->fWidth;
	pButton[5]->fHeight = pButton[3]->fHeight;
	pButton[5]->m_Centerpos.x = pButton[2]->m_Centerpos.x;
	pButton[5]->m_Centerpos.y = pButton[4]->m_Centerpos.y;
	//////////////////////////////////////////////////////
	pButton[6]->fWidth = pButton[0]->fWidth;
	pButton[6]->fHeight = pButton[0]->fHeight;
	pButton[6]->m_Centerpos.x = pButton[0]->m_Centerpos.x;
	pButton[6]->m_Centerpos.y = pButton[3]->m_Centerpos.y + pButton[3]->fHeight * 0.5f + pButton[6]->fHeight * 0.5f;

	pButton[7]->fWidth = pButton[1]->fWidth;
	pButton[7]->fHeight = pButton[1]->fHeight;
	pButton[7]->m_Centerpos.x = pButton[4]->m_Centerpos.x;
	pButton[7]->m_Centerpos.y = pButton[6]->m_Centerpos.y;

	pButton[8]->fWidth = pButton[2]->fWidth;
	pButton[8]->fHeight = pButton[2]->fHeight;
	pButton[8]->m_Centerpos.x = pButton[5]->m_Centerpos.x;
	pButton[8]->m_Centerpos.y = pButton[7]->m_Centerpos.y;

	pButton[0]->m_VertexList[0].TexPos.x = 0.0f;
	pButton[0]->m_VertexList[0].TexPos.y = 0.0f;
	pButton[0]->m_VertexList[1].TexPos.x = 0.33f;
	pButton[0]->m_VertexList[1].TexPos.y = 0.0f;
	pButton[0]->m_VertexList[2].TexPos.x = 0.0f;
	pButton[0]->m_VertexList[2].TexPos.y = 0.33f;
	pButton[0]->m_VertexList[3].TexPos.x = 0.33f;
	pButton[0]->m_VertexList[3].TexPos.y = 0.33f;

	pButton[1]->m_VertexList[0].TexPos.x = 0.33f;
	pButton[1]->m_VertexList[0].TexPos.y = 0.0f;
	pButton[1]->m_VertexList[1].TexPos.x = 0.67f;
	pButton[1]->m_VertexList[1].TexPos.y = 0.0f;
	pButton[1]->m_VertexList[2].TexPos.x = 0.33f;
	pButton[1]->m_VertexList[2].TexPos.y = 0.33f;
	pButton[1]->m_VertexList[3].TexPos.x = 0.67f;
	pButton[1]->m_VertexList[3].TexPos.y = 0.33f;

	pButton[2]->m_VertexList[0].TexPos.x = 0.67f;
	pButton[2]->m_VertexList[0].TexPos.y = 0.0f;
	pButton[2]->m_VertexList[1].TexPos.x = 1.0f;
	pButton[2]->m_VertexList[1].TexPos.y = 0.0f;
	pButton[2]->m_VertexList[2].TexPos.x = 0.67f;
	pButton[2]->m_VertexList[2].TexPos.y = 0.33f;
	pButton[2]->m_VertexList[3].TexPos.x = 1.0f;
	pButton[2]->m_VertexList[3].TexPos.y = 0.33f;

	////////////////////////////////////////////

	pButton[3]->m_VertexList[0].TexPos = pButton[0]->m_VertexList[2].TexPos;
	pButton[3]->m_VertexList[1].TexPos = pButton[0]->m_VertexList[3].TexPos;
	pButton[3]->m_VertexList[2].TexPos.x = 0.0f;
	pButton[3]->m_VertexList[2].TexPos.y = 0.67f;
	pButton[3]->m_VertexList[3].TexPos.x = 0.33f;
	pButton[3]->m_VertexList[3].TexPos.y = 0.67f;

	pButton[4]->m_VertexList[0].TexPos = pButton[1]->m_VertexList[2].TexPos;
	pButton[4]->m_VertexList[1].TexPos = pButton[1]->m_VertexList[3].TexPos;
	pButton[4]->m_VertexList[2].TexPos.x = 0.33f;
	pButton[4]->m_VertexList[2].TexPos.y = 0.67f;
	pButton[4]->m_VertexList[3].TexPos.x = 0.67f;
	pButton[4]->m_VertexList[3].TexPos.y = 0.67f;

	pButton[5]->m_VertexList[0].TexPos = pButton[2]->m_VertexList[2].TexPos;
	pButton[5]->m_VertexList[1].TexPos = pButton[2]->m_VertexList[3].TexPos;
	pButton[5]->m_VertexList[2].TexPos.x = 0.67f;
	pButton[5]->m_VertexList[2].TexPos.y = 0.67f;
	pButton[5]->m_VertexList[3].TexPos.x = 1.0f;
	pButton[5]->m_VertexList[3].TexPos.y = 0.67f;

	//////////////////////////////////////////////

	pButton[6]->m_VertexList[0].TexPos = pButton[3]->m_VertexList[2].TexPos;
	pButton[6]->m_VertexList[1].TexPos = pButton[3]->m_VertexList[3].TexPos;
	pButton[6]->m_VertexList[2].TexPos.x = 0.0f;
	pButton[6]->m_VertexList[2].TexPos.y = 1.0f;
	pButton[6]->m_VertexList[3].TexPos.x = 0.33f;
	pButton[6]->m_VertexList[3].TexPos.y = 1.0f;

	pButton[7]->m_VertexList[0].TexPos = pButton[4]->m_VertexList[2].TexPos;
	pButton[7]->m_VertexList[1].TexPos = pButton[4]->m_VertexList[3].TexPos;
	pButton[7]->m_VertexList[2].TexPos.x = 0.33f;
	pButton[7]->m_VertexList[2].TexPos.y = 1.0f;
	pButton[7]->m_VertexList[3].TexPos.x = 0.67f;
	pButton[7]->m_VertexList[3].TexPos.y = 1.0f;

	pButton[8]->m_VertexList[0].TexPos = pButton[5]->m_VertexList[2].TexPos;
	pButton[8]->m_VertexList[1].TexPos = pButton[5]->m_VertexList[3].TexPos;
	pButton[8]->m_VertexList[2].TexPos.x = 0.67f;
	pButton[8]->m_VertexList[2].TexPos.y = 1.0f;
	pButton[8]->m_VertexList[3].TexPos.x = 1.0f;
	pButton[8]->m_VertexList[3].TexPos.y = 1.0f;

	CreateButtonVertex();
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			AddVertex(pButton[i]->m_VertexList[j]);
		}
	}
}
void Button::AddVertex(const P3_VERTEX& vertex)
{
	m_VertexList.push_back(vertex);
	++iIndex;
	if (iIndex % 4 == 0)
	{
		pushIndecies();
	}
}
void Button::pushIndecies()
{
	m_indiciesList.push_back(iIndex - 4);
	m_indiciesList.push_back(iIndex - 3);
	m_indiciesList.push_back(iIndex - 2);
	m_indiciesList.push_back(iIndex - 3);
	m_indiciesList.push_back(iIndex - 1);
	m_indiciesList.push_back(iIndex - 2);
}
bool Button::Init()
{
	for (int i = 0; i < 9; ++i)
	{
		pButton[i] = new Button;
	}
	return true;
}
bool Button::Release()
{
	for (int i = 0; i < 9; ++i)
	{
		delete pButton[i];
	}
	return true;
}
void Button::CreateButtonVertex()
{
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			pButton[i]->m_VertexList[j].Pos.z = 0.5f;
		}
		D2D1_POINT_2F pos[4];
		pos[0] = { pButton[i]->m_Centerpos.x - pButton[i]->fWidth * 0.5f, pButton[i]->m_Centerpos.y - pButton[i]->fHeight * 0.5f };
		pos[1] = { pButton[i]->m_Centerpos.x + pButton[i]->fWidth * 0.5f, pButton[i]->m_Centerpos.y - pButton[i]->fHeight * 0.5f };
		pos[2] = { pButton[i]->m_Centerpos.x - pButton[i]->fWidth * 0.5f, pButton[i]->m_Centerpos.y + pButton[i]->fHeight * 0.5f };
		pos[3] = { pButton[i]->m_Centerpos.x + pButton[i]->fWidth * 0.5f, pButton[i]->m_Centerpos.y + pButton[i]->fHeight * 0.5f };
		pos[0] = pButton[i]->ComputeCoord(pos[0]);
		pos[1] = pButton[i]->ComputeCoord(pos[1]);
		pos[2] = pButton[i]->ComputeCoord(pos[2]);
		pos[3] = pButton[i]->ComputeCoord(pos[3]);
		pButton[i]->m_VertexList[0].Pos.x = pos[0].x;
		pButton[i]->m_VertexList[0].Pos.y = pos[0].y;
		pButton[i]->m_VertexList[1].Pos.x = pos[1].x;
		pButton[i]->m_VertexList[1].Pos.y = pos[1].y;
		pButton[i]->m_VertexList[2].Pos.x = pos[2].x;
		pButton[i]->m_VertexList[2].Pos.y = pos[2].y;
		pButton[i]->m_VertexList[3].Pos.x = pos[3].x;
		pButton[i]->m_VertexList[3].Pos.y = pos[3].y;
	}
}