#include "Scene.h"
#include "ObjectMgr.h"
//#include "mSound.h"

std::tifstream& operator >> (std::tifstream& fp, ObjectEnum& type)
{
	int k;
	fp >> k;
	type = CASTING(ObjectEnum, k);
	return fp;
}

Scene::Scene(const std::tstring& Scenename) : m_bNextSceneStart(false), m_SceneName(Scenename)
{}
bool Scene::Init()
{
	SceneSet();
	return true;
}
bool Scene::Render()
{
	S_Object.Render(m_pContext);
	return true;
}
bool Scene::inverseInit()
{
	return true;
}
bool Scene::getNextScene()
{
	return m_bNextSceneStart;
}
void Scene::setDevice(ID3D11Device * pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
}
void Scene::SceneSet(const bool& isInverse)
{
	std::tifstream fp;
	FileExceptErr(fp, Filepath::m_Txtpath[L"SceneScript"]);
	S_Object.Release();

	std::tstring Buffer;
	int ObjTypeNum;

	while (Buffer != m_SceneName && fp.eof() == false)
	{
		std::getline(fp, Buffer);
	}

	if (fp.eof() == true)
	{
		std::tstring ErrMsg = m_SceneName + L" Can't Find";
		MessageBox(nullptr, ErrMsg.c_str(), L"SceneBuilder Error", MB_OK);
		PostQuitMessage(0);
	}

	fp >> Buffer >> ObjTypeNum;
	if (isInverse == false)
	{
		for (int k = 0; k < ObjTypeNum; ++k)
		{
			ObjectEnum ObjType;
			int ObjNum;
			fp >> ObjType >> ObjNum >> Buffer;

			switch (ObjType)
			{
			case ObjectEnum::BACKGROUND:
			{
				for (int i = 0; i < ObjNum; ++i)
				{
					FLOAT BackPos[4];
					fp >> BackPos[0] >> BackPos[1] >> BackPos[2] >> BackPos[3];
					Background* pBackground = new Background;
					pBackground->SetPos(BackPos[0], BackPos[1], BackPos[2], BackPos[3]);
					pBackground->InitSet(m_pDevice, L"Basic", Filepath::m_Pngpath[L"Map"], Filepath::m_Txtpath[L"Shader"]);
					S_Object.AddBackGround(pBackground);
				}
			}break;
			case ObjectEnum::TERRAIN:
			{
				for (int i = 0; i < ObjNum; ++i)
				{
					D3DXVECTOR4 TerrainPos;
					fp >> TerrainPos.x >> TerrainPos.y >> TerrainPos.z >> TerrainPos.w;
					Terrain* pTerrain = new Terrain;
					pTerrain->SetPos(TerrainPos);
					pTerrain->InitSet(m_pDevice, L"Terrain", Filepath::m_Txtpath[L"Shader"], "VS", "TerrainPS");
					S_Object.AddTerrain(pTerrain);
				}
			}break;
			case ObjectEnum::PLAYER:
			{
				for (int i = 0; i < ObjNum; ++i)
				{
					D3DXVECTOR2 Center;
					fp >> Center.x >> Center.y;
					Player* pPlayer = new Player;
					pPlayer->SetCenterPos(Center);
					pPlayer->InitSet(m_pDevice, L"Basic", Filepath::m_Pngpath[L"Kaho"], Filepath::m_Txtpath[L"Shader"]);
					S_Object.AddPlayer(pPlayer);
				}
			}break;
			}
		}
	}
	else
	{
		FLOAT xWidth = 0.0f;

		for (int k = 0; k < ObjTypeNum; ++k)
		{
			ObjectEnum ObjType;
			int ObjNum;
			fp >> ObjType >> ObjNum >> Buffer;

			switch (ObjType)
			{
			case ObjectEnum::BACKGROUND:
			{
				for (int i = 0; i < ObjNum; ++i)
				{
					FLOAT BackPos[4];
					fp >> BackPos[0] >> BackPos[1] >> BackPos[2] >> BackPos[3];
					xWidth = BackPos[2] - g_fImageWidth - BackPos[0];
					Background* pBackground = new Background;
					pBackground->SetInversePos(BackPos[0], BackPos[1], BackPos[2], BackPos[3]);
					pBackground->InitSet(m_pDevice, L"Basic", Filepath::m_Pngpath[L"Map"], Filepath::m_Txtpath[L"Shader"]);
					S_Object.AddBackGround(pBackground);
				}
			}break;
			case ObjectEnum::TERRAIN:
			{
				for (int i = 0; i < ObjNum; ++i)
				{
					D3DXVECTOR4 TerrainPos;
					fp >> TerrainPos.x >> TerrainPos.y >> TerrainPos.z >> TerrainPos.w;
					Terrain* pTerrain = new Terrain;
					pTerrain->SetPos(TerrainPos);
					pTerrain->Scroll(xWidth * 3.0f);
					pTerrain->InitSet(m_pDevice, L"Terrain", Filepath::m_Txtpath[L"Shader"], "VS", "TerrainPS");
					S_Object.AddTerrain(pTerrain);
				}
			}break;
			case ObjectEnum::PLAYER:
			{
				for (int i = 0; i < ObjNum; ++i)
				{
					D3DXVECTOR2 Center;
					fp >> Center.x >> Center.y;
					Player* pPlayer = new Player;
					Center.x = g_rtClient.right - Center.x;
					pPlayer->SetCenterPos(Center);
					pPlayer->reverseDir();

					pPlayer->InitSet(m_pDevice, L"Basic", Filepath::m_Pngpath[L"Kaho"], Filepath::m_Txtpath[L"Shader"]);
					S_Object.AddPlayer(pPlayer);
				}
			}break;
			}
		}
	}
}
//LobbyScene::LobbyScene() : isPress(false), isSoundBar(false)
//{}										 
//bool LobbyScene::Init()
//{
//	PosSet();
//	m_pFadeObject = new FadeObject;
//	m_pFadeObject->Set(0, 0, 0, 0, g_rtClient.right, g_rtClient.bottom);
//	m_pFadeObject->Init();
//	m_pFadeObject->Decrease(30.0f);
//	for (int i = 0; i < 3; ++i)
//	{
//		m_bkmisc[i].LoadFile(L"TitleAnyKey", L"../../data/bmp/Misc.bmp");
//		m_bkmisc[i].Init();
//	}
//	m_BKObject.LoadFile(L"MainMenu", L"../../data/bmp/Title_0.bmp");
//	m_BKObject.Set(0, 0, 0, 0, g_rtClient.right, g_rtClient.bottom);
//	m_BKObject.Init();
//	m_BKObject.setFadeRender(0.0f, 30.0f);
//	m_state = LOBBYSTATE::DEFAULT;
//	g_UI = new UI;
//	g_UI->LoadFile(L"UI", L"../../data/bmp/UIColor.bmp", L"../../data/bmp/UIMask.bmp");
//	g_HPBar = new UI;
//	g_HPBar->LoadFile(L"HPBAR", L"../../data/bmp/HPBar.bmp");
//	g_UI->Set(30, 48, 0, 0, 276, 72);
//	g_HPBar->Set(108, 51, 0, 0, 186, 21);
//	g_UI->Init();
//	g_HPBar->Init();
//	return true;
//}
//bool LobbyScene::Frame()
//{
//	BKState();
//	if (m_pFadeObject)
//	{
//		if (m_pFadeObject->Frame() == false)
//		{
//			if (m_state == LOBBYSTATE::SELECT)
//			{
//				S_Sound.StopSound(BGM::TITLE);
//				m_bNextSceneStart = true;
//				return false;
//			}
//			m_state = LOBBYSTATE::START;
//			delete m_pFadeObject;
//			m_pFadeObject = nullptr;
//			m_miscIndex = 0;
//		}
//	}
//	m_BKObject.Frame();
//	for (int i = 0; i < 3; ++i)
//	{
//		m_bkmisc[i].Frame();
//	}
//	if (m_pFadeObject)
//	{
//		if (!m_pFadeObject->Frame())
//		{
//			if (m_state == LOBBYSTATE::SELECT)
//			{
//				m_bNextSceneStart = true;
//				delete m_pFadeObject;
//				m_pFadeObject = nullptr;
//				return false;
//			}
//		}
//	}
//	return true;
//}
//bool LobbyScene::Render()
//{
//	m_BKObject.Render();
//	for (int i = 0; i < 3; ++i)
//	{
//		m_bkmisc[i].Render();
//	}
//	if (m_pFadeObject)
//	{
//		m_pFadeObject->Render();
//	}
//	return true;
//}
//bool LobbyScene::Release()
//{
//	m_BKObject.Release();
//	for (int i = 0; i < 3; ++i)
//	{
//		m_bkmisc[i].Release();
//	}
//	return true;
//}
//bool LobbyScene::BKState()
//{
//	switch (m_state)
//	{
//	case LOBBYSTATE::START:
//		m_BKObject.Set(0, 0, 0, 0, 960, 720);
//		m_bkmisc[0].Set(m_miscpos[m_miscIndex],m_miscrt[m_miscIndex]);
//		if (S_Input.isPressAnyKey())
//		{
//			m_state = LOBBYSTATE::MAINMENU;
//			S_Sound.Play(Effect::PRESSANYKEY);
//			S_Sound.Play(BGM::TITLE, true, true);
//			m_miscIndex = 1;
//		}
//		break;
//	case LOBBYSTATE::MAINMENU:
//		if (m_miscIndex >= 2 && m_miscIndex <= 6)
//		{
//			if (S_Input.GetKey(VK_UP) == KEYSTATE::KEY_PUSH)
//			{
//				S_Sound.Play(Effect::MENUMOVE);
//				m_miscIndex--;
//			}
//			if (S_Input.GetKey(VK_DOWN) == KEYSTATE::KEY_PUSH)
//			{
//				S_Sound.Play(Effect::MENUMOVE);
//				m_miscIndex++;
//			}
//		}
//		else if (m_miscIndex == 1)
//		{
//			if (S_Input.GetKey(VK_DOWN) == KEYSTATE::KEY_PUSH)
//			{
//				S_Sound.Play(Effect::MENUMOVE);
//				m_miscIndex++;
//			}
//		}
//		else
//		{
//			if (S_Input.GetKey(VK_UP) == KEYSTATE::KEY_PUSH)
//			{
//				S_Sound.Play(Effect::MENUMOVE);
//				m_miscIndex--;
//			}
//		}
//		if (S_Input.GetKey('A') == KEYSTATE::KEY_PUSH)
//		{
//			S_Sound.Play(Effect::MENUSELECT);
//			AKeyReact(m_miscIndex);
//		}
//		m_BKObject.Set(0, 0, 960, 0, 960, 720);
//		m_bkmisc[0].Set(m_miscpos[m_miscIndex], m_miscrt[m_miscIndex]);
//		break;
//	case LOBBYSTATE::SETTING:
//		if (m_miscIndex == 9)
//		{
//			if (S_Input.GetKey(VK_UP) == KEYSTATE::KEY_PUSH)
//			{
//				S_Sound.Play(Effect::SETTINGMOVE);
//				m_miscIndex--;
//			}
//			if (S_Input.GetKey(VK_DOWN) == KEYSTATE::KEY_PUSH)
//			{
//				m_state = LOBBYSTATE::KEYSETTING;
//				S_Sound.Play(Effect::SETTINGMOVE);
//				isSoundBar = false;
//				m_miscIndex = 11;
//			}
//			if (S_Input.GetKey(VK_LEFT) == KEYSTATE::KEY_PUSH)
//			{
//				g_BGMVolume -= 0.1f;
//				if (g_BGMVolume <= 0.0f)
//				{
//					g_BGMVolume = 0.0f;
//				}
//			}
//			if (S_Input.GetKey(VK_RIGHT) == KEYSTATE::KEY_PUSH)
//			{
//				g_BGMVolume += 0.1f;
//				if (g_BGMVolume >= 1.0f)
//				{
//					g_BGMVolume = 1.0f;
//				}
//			}
//		}
//		else if (m_miscIndex == 8)
//		{
//			if (S_Input.GetKey(VK_DOWN) == KEYSTATE::KEY_PUSH)
//			{
//				S_Sound.Play(Effect::SETTINGMOVE);
//				m_miscIndex++;
//			}
//			if (S_Input.GetKey(VK_LEFT) == KEYSTATE::KEY_PUSH)
//			{
//				g_EffectVolume -= 0.1f;
//				if (g_EffectVolume <= 0.0f)
//				{
//					g_EffectVolume = 0.0f;
//				}
//			}
//			if (S_Input.GetKey(VK_RIGHT) == KEYSTATE::KEY_PUSH)
//			{
//				g_EffectVolume += 0.1f;
//				if (g_EffectVolume >= 1.0f)
//				{
//					g_EffectVolume = 1.0f;
//				}
//			
//			}
//		}
//		else
//		{
//			if (S_Input.GetKey(VK_UP) == KEYSTATE::KEY_PUSH)
//			{
//				S_Sound.Play(Effect::SETTINGMOVE);
//				m_state = LOBBYSTATE::KEYSETTING;
//				isSoundBar = false;
//				m_miscIndex = 11;
//			}
//			if (S_Input.GetKey('A') == KEYSTATE::KEY_PUSH)
//			{
//				S_Sound.Play(Effect::MENUCANCLE);
//				AKeyReact(m_miscIndex);
//			}
//		}
//		m_BKObject.Set(0, 0, 0, 720, 960, 720);
//		m_bkmisc[0].Set(m_miscpos[m_miscIndex], m_miscrt[m_miscIndex]);
//		if (!isSoundBar)
//		{
//			m_bkmisc[1].Set(741, 102, 390, 119, 0, 0);
//			m_bkmisc[2].Set(741, 150, 390, 119, 0, 0);
//		}
//		else
//		{
//			m_bkmisc[1].Set(741, 102, 390, 119, 90 * g_EffectVolume, 24);
//			m_bkmisc[2].Set(741, 150, 390, 119, 90 * g_BGMVolume, 24);
//		}
//		break;
//	case LOBBYSTATE::KEYSETTING:
//		m_BKObject.Set(0, 0, 960, 720, 960, 720);
//		if (S_Input.GetKey(VK_UP) == KEYSTATE::KEY_PUSH)
//		{
//			S_Sound.Play(Effect::SETTINGMOVE);
//			m_state = LOBBYSTATE::SETTING;
//			isSoundBar = true;
//			m_miscIndex = 9;
//		}
//		if (S_Input.GetKey(VK_DOWN) == KEYSTATE::KEY_PUSH)
//		{
//			S_Sound.Play(Effect::SETTINGMOVE);
//			m_state = LOBBYSTATE::SETTING;
//			isSoundBar = true;
//			m_miscIndex = 10;
//		}
//		break;
//	case LOBBYSTATE::SELECT:
//		if (S_Input.GetKey('A') == KEYSTATE::KEY_PUSH)
//		{
//			S_Sound.Play(Effect::MENUSELECT);
//			m_pFadeObject = new FadeObject;
//			m_pFadeObject->Set(0, 0, 0, 0, g_rtClient.right, g_rtClient.bottom);
//			m_pFadeObject->Init();
//			m_pFadeObject->SetAlpha(0.0f);
//			m_pFadeObject->Decrease(130.0f);
//		}
//		if (S_Input.GetKey('S') == KEYSTATE::KEY_PUSH)
//		{
//			S_Sound.Play(Effect::MENUCANCLE);
//			m_state = LOBBYSTATE::MAINMENU;
//			m_bkmisc[1].Set(741, 102, 390, 119, 0, 0);
//		}
//	}
//
//
//	return true;
//}
//void LobbyScene::PosSet()
//{
//	m_miscpos[0] = { 297,509 };
//	m_miscpos[1] = { 3,0 };
//	m_miscpos[2] = { 3,84 };
//	m_miscpos[3] = { 3,168 };
//	m_miscpos[4] = { 3,252 };
//	m_miscpos[5] = { 3,336 };
//	m_miscpos[6] = { 3,420 };
//	m_miscpos[7] = { 3,504 };
//	m_miscpos[8] = { 1,93 };
//	m_miscpos[9] = { 1,141 };
//	m_miscpos[10] = { 1,237 };
//
//	m_miscrt[0] = { 3,119,381,36 };
//	m_miscrt[1] = { 3,37,954,78 };
//	m_miscrt[2] = { 3,37,954,78 };
//	m_miscrt[3] = { 3,37,954,78 };
//	m_miscrt[4] = { 3,37,954,78 };
//	m_miscrt[5] = { 3,37,954,78 };
//	m_miscrt[6] = { 3,245,249,78 };
//	m_miscrt[7] = { 3,160,249,78 };
//	m_miscrt[8] = { 0,330,960,42 };
//	m_miscrt[9] = { 0,374,960,42 };
//	m_miscrt[10] = { 0,418,960,42 };
//	m_miscrt[11] = { 0,0,0,0 };
//
//	m_bkmisc[1].Set(741, 102, 390, 119, 0, 0);
//	m_bkmisc[2].Set(741, 150, 390, 119, 0, 0);
//}
//void LobbyScene::AKeyReact(const INT& index)
//{
//	switch (index)
//	{
//	case 1:
//		m_bkmisc[1].Set(755, 24, 491, 121, 107, 40);
//		m_state = LOBBYSTATE::SELECT;
//		break;
//	case 2:
//		m_bkmisc[1].Set(755, 108, 491, 121, 107, 40);
//		m_state = LOBBYSTATE::SELECT;
//		break;
//	case 3:
//		m_bkmisc[1].Set(755, 192, 491, 121, 107, 40);
//		m_state = LOBBYSTATE::SELECT;
//		break;
//	case 4:
//		m_bkmisc[1].Set(755, 276, 491, 121, 107, 40);
//		m_state = LOBBYSTATE::SELECT;
//		break;
//	case 5:
//		m_bkmisc[1].Set(755, 360, 491, 121, 107, 40);
//		m_state = LOBBYSTATE::SELECT;
//		break;
//	case 6:
//		m_state = LOBBYSTATE::SETTING;
//		m_miscIndex = 8;
//		isSoundBar = true;
//		break;
//	case 7:
//		DestroyWindow(g_hWnd);
//		break;
//	case 10:
//		m_state = LOBBYSTATE::MAINMENU;
//		isSoundBar = false;
//		m_miscIndex = 6;
//		break;
//	}
//}
//
//
//bool EndScene::Init()
//{
//	m_BKObject.LoadFile(L"End", L"../../data/bmp/EndScene.bmp");
//	m_BKObject.Set(0, 0, 0, 0, g_rtClient.right, g_rtClient.bottom);
//	m_BKObject.Init();
//	return true;
//}
//bool EndScene::Frame()
//{
//	if (S_Input.GetKey('A') == KEYSTATE::KEY_PUSH)
//	{
//		DestroyWindow(g_hWnd);
//	}
//	m_BKObject.Frame();
//	return true;
//}
//bool EndScene::Render()
//{
//	m_BKObject.Render();
//	return true;
//}
//bool EndScene::Release()
//{
//	m_BKObject.Release();
//	return true;
//}