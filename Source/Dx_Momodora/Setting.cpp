#include "Setting.h"
#include "DirectInput.h"

FLOAT g_fEffectVolume = 1.0f;
FLOAT g_fBGMVolume = 1.0f;

bool Setting::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
						const std::string& VSFunc, const std::string& PSFunc)
{
	m_DrawArray[0] = { 0.0f, 720.0f, 960.0f, 1440.0f };
	m_DrawArray[1] = { 960.0f, 720.0f, 1920.0f, 1440.0f };

	SetTexPos(m_DrawArray[0]);

	m_VertexList[0].Pos = { 0.0f, 0.0f, 0.5f };
	m_VertexList[1].Pos = { CASTING(FLOAT,g_rtClient.right), 0.0f, 0.5f };
	m_VertexList[2].Pos = { 0.0f, CASTING(FLOAT,g_rtClient.bottom), 0.5f };
	m_VertexList[3].Pos = { m_VertexList[1].Pos.x, m_VertexList[2].Pos.y, 0.5f };

	m_BarDrawArray[0] = { 0.0f, 330.0f, 960.0f, 372.0f };
	m_BarDrawArray[1] = { 0.0f, 374.0f, 960.0f, 416.0f };
	m_BarDrawArray[2] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_BarDrawArray[3] = { 0.0f, 418.0f, 960.0f, 460.0f };

	m_BarCenterPos[0] = { 480.0f, 114.0f };
	m_BarCenterPos[1] = { 480.0f, 162.0f };
	m_BarCenterPos[2] = { 0.0f, 0.0f };
	m_BarCenterPos[3] = { 480.0f, 257.0f };

	m_Bar.InitSet(pDevice, L"Basic", Filepath::m_Pngpath[L"Misc"], Filepath::m_Txtpath[L"Shader"]);
	m_Bar.SetCenterPos(m_BarCenterPos[0]);
	for (int i = 0; i < 2; ++i)
	{
		m_SoundBarArray[i].SetTexPos({ 390.0f, 119.0f, 480.0f, 143.0f });
		m_SoundBarArray[i].InitSet(pDevice, L"Basic", Filepath::m_Pngpath[L"Misc"], Filepath::m_Txtpath[L"Shader"]);
	}
	m_SoundBarArray[0].SetCenterPos({ 786.0f, 114.0f });
	m_SoundBarArray[0].setPercent(&g_fEffectVolume);
	m_SoundBarArray[1].SetCenterPos({ 786.0f, 162.0f });
	m_SoundBarArray[1].setPercent(&g_fBGMVolume);

	CreateIndexBuffer(pDevice);
	m_Object.setBlendState(pDevice);
	m_ConstantData.Util.z = 1.0f;
	return Plane_Object::InitSet(pDevice, Name, TexFilepath, ShaderFilepath, VSFunc, PSFunc);
}

bool Setting::Frame()
{
	switch (m_iIndex)
	{
	case 0:
	{
		DownKey();
		if (SUBTRACT(g_fEffectVolume, 0.0f) > Epsilon && S_Input.getKeyState(DIK_LEFT) == Input::KEYSTATE::KEY_PUSH)
		{
			VolumeSet(g_fEffectVolume, -0.1f);
		}
		if (SUBTRACT(g_fEffectVolume, 1.0f) > Epsilon && S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_PUSH)
		{
			VolumeSet(g_fEffectVolume, +0.1f);
		}
	}break;
	case 1:
	{
		UPKey();
		DownKey();
		if (SUBTRACT(g_fBGMVolume, 0.0f) > Epsilon && S_Input.getKeyState(DIK_LEFT) == Input::KEYSTATE::KEY_PUSH)
		{
			VolumeSet(g_fBGMVolume, -0.1f);
		}
		if (SUBTRACT(g_fBGMVolume, 1.0f) > Epsilon && S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_PUSH)
		{
			VolumeSet(g_fBGMVolume, +0.1f);
		}
	}break;
	case 2:
	{
		UPKey();
		DownKey();
	}break;
	case 3:
	{
		UPKey();
		if (S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_PUSH)
		{
			S_Sound.PlayEffect(Effect_Snd::MENUCANCLE);
			m_iIndex = 0;
			m_pMenu->setSetting(false);
			m_pMenu = nullptr;
			return true;
		}
	}break;
	}
	if (m_iIndex != 2)
	{
		SetTexPos(m_DrawArray[0]);
		for (auto & iter : m_SoundBarArray)
		{
			iter.Frame();
		}
	}
	else
	{
		SetTexPos(m_DrawArray[1]);
	}

	m_Bar.SetTexPos(m_BarDrawArray[m_iIndex]);
	m_Bar.SetCenterPos(m_BarCenterPos[m_iIndex]);
	m_Bar.Frame();
	return true;
}
bool Setting::Release()
{
	std::array<ProgressBar, 2>::iterator iter;
	for (iter = m_SoundBarArray.begin(); iter != m_SoundBarArray.end(); iter++)
	{
		iter->Release();
	}
	m_Bar.Release();
	return true;
}
bool Setting::Render(ID3D11DeviceContext* pContext)
{
	UI::Render(pContext);
	if (m_iIndex != 2)
	{
		m_Bar.Render(pContext);
		for (auto & it : m_SoundBarArray)
		{
			it.Render(pContext);
		}
	}
	return true;
}

void Setting::setMenu(Menu* pMenu)
{
	m_pMenu = pMenu;
}

bool Setting::DownKey()
{
	if (Menu::DownKey() == true)
	{
		S_Sound.PlayEffect(Effect_Snd::SETTINGMOVE);
	}
	return true;
}
bool Setting::UPKey()
{
	if (Menu::UPKey() == true)
	{
		S_Sound.PlayEffect(Effect_Snd::SETTINGMOVE);
	}
	return true;
}