#include "MiscSet.h"
#include "Player.h"

bool MiscSet::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath)
{
	for (int i = 0; i < m_Misc.size(); ++i)
	{
		m_Misc[i].InitSet(pDevice, L"Misc", Filepath::m_Pngpath[L"Kaho"], Filepath::m_Txtpath[L"Shader"], "Misc", "PS");
		m_Misc[i].SetTexPos({ 3.0f, 3790.0f, 7.0f, 3794.0f });
		m_Misc[i].setColorX({ 0.5f * CASTING(FLOAT,D3DX_PI) * i });
	}
	return true;
}
bool MiscSet::Frame()
{
	for (int i = 0; i < m_Misc.size(); ++i)
	{
		m_Misc[i].SetCenterPos(g_Player->getCenterPos());
		m_Misc[i].MoveCenterPos({ 0.0f, -20.0f });
		m_Misc[i].Frame();
	}
	return true;
}
bool MiscSet::Render(ID3D11DeviceContext* pContext)
{
	for (int i = 0; i < m_Misc.size(); ++i)
	{
		m_Misc[i].Render(pContext);
	}
	return true;
}
bool MiscSet::Release()
{
	for (int i = 0; i < m_Misc.size(); ++i)
	{
		m_Misc[i].Release();
	}
	return true;
}