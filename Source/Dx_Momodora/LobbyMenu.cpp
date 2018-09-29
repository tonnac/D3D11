#include "LobbyMenu.h"
#include "Bar.h"

Lobbymenu::Lobbymenu() : m_State(LOBBYSTATE::START), m_iBarIndex(0)
{}

bool Lobbymenu::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
	const std::string& VSFunc, const std::string& PSFunc)
{
	m_DrawArray[0] = { 0.0f,0.0f,960.0f,720.0f };
	m_DrawArray[1] = { 960.0f, 0.0f, 1920.0f, 720.0f };

	m_BarCenterPos[0] = { g_rtClient.right * 0.5f, 501.f };
	m_BarCenterPos[1] = { g_rtClient.right * 0.5f, 39.0f };
	m_BarCenterPos[2] = { g_rtClient.right * 0.5f, 123.0f };
	m_BarCenterPos[3] = { g_rtClient.right * 0.5f, 207.0f };
	m_BarCenterPos[4] = { g_rtClient.right * 0.5f, 291.0f };
	m_BarCenterPos[5] = { g_rtClient.right * 0.5f, 375.0f };
	m_BarCenterPos[6] = { 127.5f, 459.0f };
	m_BarCenterPos[7] = { 127.5f, 543.0f };

	m_BarDrawArray[0] = { 3.0f, 37.0f, 957.0f, 115.0f };
	m_BarDrawArray[1] = { 0.0f, 119.0f, 384.0f, 155.0f };
	m_BarDrawArray[2] = { 3.0f, 160.0f, 252.0f, 238.0f };
	m_BarDrawArray[3] = { 3.0f, 245.0f, 252.0f, 323.0f };

	m_Bar = std::make_shared<Bar>();
	m_Bar->SetCenterPos(m_BarCenterPos[0]);
	m_Bar->SetTexPos(m_BarDrawArray[1]);
	m_Bar->InitSet(pDevice, Name, Filepath::m_Pngpath[L"Misc"], ShaderFilepath);

	SetTexPos(m_DrawArray[0]);
	SetCenterPos({ g_rtClient.right * 0.5f, g_rtClient.bottom * 0.5f });
	return Menu::InitSet(pDevice, Name, TexFilepath, ShaderFilepath);
}

void Lobbymenu::SetPos(const FLOAT& rtLeft, const FLOAT& rtTop, const FLOAT& rtRight, const FLOAT& rtBottom)
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
bool Lobbymenu::Frame()
{
	m_LengthDiff.x = (m_VertexList[1].TexPos.x - m_VertexList[0].TexPos.x) * 0.5f;
	m_LengthDiff.y = (m_VertexList[2].TexPos.y - m_VertexList[0].TexPos.y) * 0.5f;
	StateFrame();
	return Menu::Frame();
}

bool Lobbymenu::Render(ID3D11DeviceContext* pContext)
{
	Menu::Render(pContext);
	if (m_State != LOBBYSTATE::DEFAULT)
	{
		m_Bar->Render(pContext);
	}
	return true;
}
bool Lobbymenu::Release()
{
	m_Bar->Release();
	m_Start->Release();
	return true;
}
bool Lobbymenu::StateFrame()
{
	if (m_State != LOBBYSTATE::DEFAULT)
	{
		m_Bar->Frame();
	}
	return true;
}