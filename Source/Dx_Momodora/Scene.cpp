#include "Scene.h"
#include "ObjectMgr.h"
#include "mSound.h"
#include "LobbyMenu.h"

std::tifstream& operator >> (std::tifstream& fp, ObjectEnum& type)
{
	int k;
	fp >> k;
	type = CASTING(ObjectEnum, k);
	return fp;
}

Scene::Scene(const std::tstring& Scenename) : m_bSceneChange(false), m_SceneName(Scenename), m_bSetting(false), m_bPrevScene(false)
{
	
}

bool Scene::Init()
{
	SceneSet();
	return true;
}
bool Scene::Render()
{
	S_Object.Render(m_pContext);
	if (g_Fade->getOn() == true)
	{
		g_Fade->Render(m_pContext);
	}
	return true;
}
bool Scene::inverseInit()
{
	return true;
}
bool Scene::getPrevScene() const
{
	return m_bPrevScene;
}
void Scene::setSetting(const bool& set)
{
	m_bSetting = set;
}
void Scene::setDevice(ID3D11Device * pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
}
bool Scene::getSetting() const
{
	return m_bSetting;
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
			case ObjectEnum::LOBBYMENU:
			{
				for (int i = 0; i < ObjNum; ++i)
				{
					std::shared_ptr<Lobbymenu> pData = std::make_shared<Lobbymenu>();
					pData->InitSet(m_pDevice, L"Basic", Filepath::m_Pngpath[L"Lobby"], Filepath::m_Txtpath[L"Shader"]);
					LobbyScene* se = dynamic_cast<LobbyScene*>(this);
					se->setLobby(pData);
					g_Fade->setDivideTime(10.0f);
					g_Fade->FadeIn();
				}
			}break;
			case ObjectEnum::BACKGROUND:
			{
				for (int i = 0; i < ObjNum; ++i)
				{
					FLOAT BackPos[4];
					fp >> BackPos[0] >> BackPos[1] >> BackPos[2] >> BackPos[3];
					BackgroundPTR pBackground = std::make_shared<Background>();
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
					TerrainPTR pTerrain = std::make_shared<Terrain>();
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
					g_Player->SetCenterPos(Center);
				}
			}break;
			case ObjectEnum::DOWN:
			{
				for (int i = 0; i < ObjNum; ++i)
				{
					D3DXVECTOR4 Pos;
					fp >> Pos.x >> Pos.y >> Pos.z >> Pos.w;
					std::shared_ptr<DownableObject> pData = std::make_shared<DownableObject>();
					pData->SetPos(Pos);
					pData->InitSet(m_pDevice, L"Terrain", Filepath::m_Txtpath[L"Shader"], "VS", "TerrainPS");
					S_Object.AddTerrain(pData);
				}
			}break;
			case ObjectEnum::LADDER:
			{
				for (int i = 0; i < ObjNum; ++i)
				{
					D3DXVECTOR4 Pos;
					fp >> Pos.x >> Pos.y >> Pos.z >> Pos.w;
					std::shared_ptr<Ladder> pData = std::make_shared<Ladder>();
					pData->SetPos(Pos);
					pData->InitSet(m_pDevice, L"Terrain", Filepath::m_Txtpath[L"Shader"], "VS", "TerrainPS");
					S_Object.AddTerrain(pData);
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
					BackgroundPTR pBackground = std::make_shared<Background>();
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
					TerrainPTR pTerrain = std::make_shared<Terrain>();
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
					Center.x = g_rtClient.right - Center.x;
					g_Player->SetCenterPos(Center);
				}
			}break;
			case ObjectEnum::DOWN:
			{
				for (int i = 0; i < ObjNum; ++i)
				{
					D3DXVECTOR4 Pos;
					fp >> Pos.x >> Pos.y >> Pos.z >> Pos.w;
					std::shared_ptr<DownableObject> pData = std::make_shared<DownableObject>();
					pData->SetPos(Pos);
					pData->Scroll(xWidth * 3.0f);
					pData->InitSet(m_pDevice, L"Terrain", Filepath::m_Txtpath[L"Shader"], "VS", "TerrainPS");
					S_Object.AddTerrain(pData);
				}
			}break;
			case ObjectEnum::LADDER:
			{
				for (int i = 0; i < ObjNum; ++i)
				{
					D3DXVECTOR4 Pos;
					fp >> Pos.x >> Pos.y >> Pos.z >> Pos.w;
					std::shared_ptr<Ladder> pData = std::make_shared<Ladder>();
					pData->SetPos(Pos);
					pData->Scroll(xWidth * 3.0f);
					pData->InitSet(m_pDevice, L"Terrain", Filepath::m_Txtpath[L"Shader"], "VS", "TerrainPS");
					S_Object.AddTerrain(pData);
				}
			}break;
			}
		}
	}
}

LobbyScene::LobbyScene() : Scene(L"LobbyScene")
{
	
}

bool LobbyScene::Frame()
{
	if (m_pLobby->Frame() == false)
	{
		m_bSceneChange = true;
		g_Fade->setDivideTime(1.0f);
		g_Fade->FadeOut();
	}
	if (g_Fade->getOn() == true)
	{
		g_Fade->Frame();
	}
	if (g_Fade->getOn() == false && m_bSceneChange == true)
	{
		return false;
	}
	return true;
}
bool LobbyScene::Render()
{
	m_pLobby->Render(m_pContext);
	if (g_Fade->getOn() == true)
	{
		g_Fade->Render(m_pContext);
	}
	return true;
}
bool LobbyScene::Release()
{
	m_pLobby->Release();
	return true;
}
void LobbyScene::setLobby(std::shared_ptr<Lobbymenu> pLobby)
{
	m_pLobby = pLobby;
}

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