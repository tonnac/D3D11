#include "Button.h"
#include "DirectInput.h"

Button::Button()
{
}
bool Button::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
				const std::string& VSFunc, const std::string& PSFunc)
{
	Plane_Object::CreateConstantBuffer(pDevice);
	return Plane_Object::InitSet(pDevice, Name, TexFilepath, ShaderFilepath, VSFunc, PSFunc);
}
void Button::SetPos(const D3DXVECTOR2& CenterPos, const D2D1_POINT_2F& Size)
{
	DivideButton(CenterPos, Size);
	CreateButtonVertex();
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m_VertexList.push_back(pButton[i]->m_VertexList[j]);
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
	return;
	//m_indiciesList.push_back(iIndex - 4);
	//m_indiciesList.push_back(iIndex - 3);
	//m_indiciesList.push_back(iIndex - 2);
	//m_indiciesList.push_back(iIndex - 3);
	//m_indiciesList.push_back(iIndex - 1);
	//m_indiciesList.push_back(iIndex - 2);
}
void Button::CreateConstant(ID3D11Device* pDevice)
{
	m_ConstantData.Util.x = g_fGameTime;
	Plane_Object::CreateConstantBuffer(pDevice);
}
bool Button::Init()
{
	for (int i = 0; i < 9; ++i)
	{
		pButton[i] = new Button;
	}
	return true;
}
bool Button::Frame()
{
	m_ConstantData.Util.x = g_fGameTime * 50.0f;
	if (S_Input.getKeyState(DIK_D) == Input::KEYSTATE::KEY_HOLD)
	{
		for (int i = 0; i < m_VertexList.size(); ++i)
		{
			m_VertexList[i].Pos.x += g_fSecPerFrame;
		}
	}
	if (S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_HOLD)
	{
		for (int i = 0; i < m_VertexList.size(); ++i)
		{
			m_VertexList[i].Pos.x -= g_fSecPerFrame;
		}
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
void Button::DivideButton(const D3DXVECTOR2& CenterPos, const D2D1_POINT_2F& Size)
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
	pButton[0]->m_VertexList[0].TexPos.x = 0.0f;
	pButton[0]->m_VertexList[0].TexPos.y = 0.0f;
	pButton[0]->m_VertexList[1].TexPos.x = 0.33f;
	pButton[0]->m_VertexList[1].TexPos.y = pButton[0]->m_VertexList[0].TexPos.y;
	pButton[0]->m_VertexList[2].TexPos.x = pButton[0]->m_VertexList[0].TexPos.x;
	pButton[0]->m_VertexList[2].TexPos.y = 0.33f;
	pButton[0]->m_VertexList[3].TexPos.x = 0.33f;
	pButton[0]->m_VertexList[3].TexPos.y = 0.33f;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			int k = 3 * i + j;
			if (k == 0) continue;
			switch (k % 3)
			{
			case 0:
			{
				if (i == 1)
				{
					pButton[k]->fHeight = Size.y * 0.8f;
				}
				else
				{
					pButton[k]->fHeight = Size.y * 0.1f;
				}
				pButton[k]->fWidth = Size.x * 0.1f;
				pButton[k]->m_Centerpos.x = CenterPos.x - Size.x * 0.5f + pButton[k]->fWidth * 0.5f;
				pButton[k]->m_Centerpos.y = pButton[k - 3]->m_Centerpos.y + pButton[k - 3]->fHeight * 0.5f + pButton[k]->fHeight * 0.5f;

				pButton[k]->m_VertexList[0].TexPos = pButton[k - 3]->m_VertexList[2].TexPos;
				pButton[k]->m_VertexList[1].TexPos = pButton[k - 3]->m_VertexList[3].TexPos;
				pButton[k]->m_VertexList[2].TexPos = pButton[k]->m_VertexList[0].TexPos;
				pButton[k]->m_VertexList[2].TexPos.y += 1 / 3.0f;
				pButton[k]->m_VertexList[3].TexPos.x = pButton[k]->m_VertexList[1].TexPos.x;
				pButton[k]->m_VertexList[3].TexPos.y = pButton[k]->m_VertexList[2].TexPos.y;
				break;
			}
			default:
			{
				if (j == 1)
				{
					pButton[k]->fWidth = Size.x * 0.8f;
				}
				else
				{
					pButton[k]->fWidth = Size.x * 0.1f;
				}
				pButton[k]->fHeight = pButton[k - 1]->fHeight;
				pButton[k]->m_Centerpos.x = pButton[k - 1]->m_Centerpos.x + pButton[k - 1]->fWidth * 0.5f + pButton[k]->fWidth * 0.5f;
				pButton[k]->m_Centerpos.y = pButton[k - 1]->m_Centerpos.y;

				pButton[k]->m_VertexList[0].TexPos = pButton[k - 1]->m_VertexList[1].TexPos;
				pButton[k]->m_VertexList[2].TexPos = pButton[k - 1]->m_VertexList[3].TexPos;
				pButton[k]->m_VertexList[1].TexPos = pButton[k]->m_VertexList[0].TexPos;
				pButton[k]->m_VertexList[1].TexPos.x += 1 / 3.0f;
				pButton[k]->m_VertexList[3].TexPos.x = pButton[k]->m_VertexList[1].TexPos.x;
				pButton[k]->m_VertexList[3].TexPos.y = pButton[k]->m_VertexList[2].TexPos.y;
			}
			}
		}
	}
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