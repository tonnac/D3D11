#include "Scene.h"
#include "ObjectMgr.h"
#include "mSound.h"
#include "LobbyMenu.h"
#include "Enemy.h"
#include "FSMMgr.h"

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
	std::tstring ObjType[] = {
		L"BackGround",
		L"Terrain",
		L"Player",
		L"Down",
		L"Ladder",
		L"LobbyScene",
		L"Enemy"
	};

	auto foo = [&ObjType](const std::tstring& buffer)
	{
		for (int i = 0; i < sizeof(ObjType) / sizeof(ObjType[0]); ++i)
		{
			if (buffer == ObjType[i])
			{
				return true;
			}
		}
		return false;
	};

	std::tifstream fp;
	FileExceptErr(fp, Filepath::m_Txtpath[L"SceneScript"]);
	S_Object.Release();

	std::tstring Buffer;

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

	std::tstring type;

	if (isInverse == false)
	{
		while (Buffer.empty() == false && fp.eof() == false)
		{
			std::getline(fp, Buffer);
			if (foo(Buffer) == true)
			{
				type = Buffer;
				continue;
			}
			std::tistringstream is(Buffer);

			if (type == ObjType[0])
			{
				FLOAT pos[4];
				is >> pos[0] >> pos[1] >> pos[2] >> pos[3];
				BackgroundPTR pBackground = std::make_shared<Background>();
				pBackground->SetPos(pos[0], pos[1], pos[2], pos[3]);
				pBackground->InitSet(m_pDevice, L"Basic", Filepath::m_Pngpath[L"Map"], Filepath::m_Txtpath[L"Shader"]);
				S_Object.AddBackGround(pBackground);
			}
			else if (type == ObjType[1])
			{
				D3DXVECTOR4 TerrainPos;
				is >> TerrainPos.x >> TerrainPos.y >> TerrainPos.z >> TerrainPos.w;
				TerrainPTR pTerrain = std::make_shared<Terrain>();
				pTerrain->SetPos(TerrainPos);
				pTerrain->InitSet(m_pDevice, L"Terrain", Filepath::m_Txtpath[L"Shader"], "VS", "TerrainPS");
				S_Object.AddTerrain(pTerrain);
			}
			else if (type == ObjType[2])
			{
				D3DXVECTOR2 Center;
				is >> Center.x >> Center.y;
				g_Player->SetCenterPos(Center);
			}
			else if (type == ObjType[3])
			{
				D3DXVECTOR4 Pos;
				is >> Pos.x >> Pos.y >> Pos.z >> Pos.w;
				std::shared_ptr<DownableObject> pData = std::make_shared<DownableObject>();
				pData->SetPos(Pos);
				pData->InitSet(m_pDevice, L"Terrain", Filepath::m_Txtpath[L"Shader"], "VS", "TerrainPS");
				S_Object.AddTerrain(pData);
			}
			else if (type == ObjType[4])
			{
				D3DXVECTOR4 Pos;
				is >> Pos.x >> Pos.y >> Pos.z >> Pos.w;
				std::shared_ptr<Ladder> pData = std::make_shared<Ladder>();
				pData->SetPos(Pos);
				pData->InitSet(m_pDevice, L"Terrain", Filepath::m_Txtpath[L"Shader"], "VS", "TerrainPS");
				S_Object.AddTerrain(pData);
			}
			else if (type == ObjType[5])
			{
				std::shared_ptr<Lobbymenu> pData = std::make_shared<Lobbymenu>();
				pData->InitSet(m_pDevice, L"Basic", Filepath::m_Pngpath[L"Lobby"], Filepath::m_Txtpath[L"Shader"]);
				LobbyScene* se = dynamic_cast<LobbyScene*>(this);
				se->setLobby(pData);
				g_Fade->setDivideTime(10.0f);
				g_Fade->FadeIn();
			}
			else if (type == ObjType[6])
			{
				std::tstring FSM;
				D3DXVECTOR2 Pos;
				D3DXVECTOR4 RTPos;
				std::shared_ptr<Enemy> pData = std::make_shared<Enemy>();
				is >> Pos.x >> Pos.y >> RTPos.x >> RTPos.y >> RTPos.z >> RTPos.w >> FSM;
				pData->SetCenterPos(Pos);
				pData->setArea({ RTPos.x, RTPos.y, RTPos.z, RTPos.w });
				pData->InitSet(m_pDevice, L"PLAYER", Filepath::m_Pngpath[L"Monkey"], Filepath::m_Txtpath[L"Shader"],"VS","PlayerPS");
				pData->setFSM(S_FSMMgr.LoadFSM(FSM));
				S_Object.AddEnemy(pData);
			}
		};
	}
	else
	{
		FLOAT xWidth = 0.0f;
		while (Buffer.empty() == false)
		{
			std::getline(fp, Buffer);
			if (foo(Buffer) == true)
			{
				type = Buffer;
				continue;
			}
			std::tistringstream is(Buffer);

			if (type == ObjType[0])
			{
				FLOAT BackPos[4];
				is >> BackPos[0] >> BackPos[1] >> BackPos[2] >> BackPos[3];
				xWidth = BackPos[2] - g_fImageWidth - BackPos[0];
				BackgroundPTR pBackground = std::make_shared<Background>();
				pBackground->SetInversePos(BackPos[0], BackPos[1], BackPos[2], BackPos[3]);
				pBackground->InitSet(m_pDevice, L"Basic", Filepath::m_Pngpath[L"Map"], Filepath::m_Txtpath[L"Shader"]);
				S_Object.AddBackGround(pBackground);
			}
			else if (type == ObjType[1])
			{
				D3DXVECTOR4 TerrainPos;
				is >> TerrainPos.x >> TerrainPos.y >> TerrainPos.z >> TerrainPos.w;
				TerrainPTR pTerrain = std::make_shared<Terrain>();
				pTerrain->SetPos(TerrainPos);
				pTerrain->Scroll(xWidth * 3.0f);
				pTerrain->InitSet(m_pDevice, L"Terrain", Filepath::m_Txtpath[L"Shader"], "VS", "TerrainPS");
				S_Object.AddTerrain(pTerrain);
			}
			else if (type == ObjType[2])
			{
				D3DXVECTOR2 Center;
				is >> Center.x >> Center.y;
				Center.y = g_Player->getCenterPos().y;
				Center.x = g_rtClient.right - 15.0f;
				g_Player->SetCenterPos(Center);
			}
			else if (type == ObjType[3])
			{
				D3DXVECTOR4 Pos;
				is >> Pos.x >> Pos.y >> Pos.z >> Pos.w;
				std::shared_ptr<DownableObject> pData = std::make_shared<DownableObject>();
				pData->SetPos(Pos);
				pData->Scroll(xWidth * 3.0f);
				pData->InitSet(m_pDevice, L"Terrain", Filepath::m_Txtpath[L"Shader"], "VS", "TerrainPS");
				S_Object.AddTerrain(pData);
			}
			else if (type == ObjType[4])
			{
				D3DXVECTOR4 Pos;
				is >> Pos.x >> Pos.y >> Pos.z >> Pos.w;
				std::shared_ptr<Ladder> pData = std::make_shared<Ladder>();
				pData->SetPos(Pos);
				pData->Scroll(xWidth * 3.0f);
				pData->InitSet(m_pDevice, L"Terrain", Filepath::m_Txtpath[L"Shader"], "VS", "TerrainPS");
				S_Object.AddTerrain(pData);
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