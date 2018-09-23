#include "GameScene.h"

GameScene::GameScene(const std::tstring& Scenename) : Scene(Scenename)
{}


GameScene1::GameScene1() : GameScene(L"GameScene1")
{}
//bool GameScene1::Init()
//{
//	//Background * pBackGround = new Background;
//	//pBackGround->SetPos(220.0f, 0.0f, 896.0f, 240.0f);
//	//pBackGround->InitSet(m_pDevice, L"DD", L"../../momodora/data/map/Map.png", L"VertexShader.txt");
//	//S_Object.AddBackGround(pBackGround);
//	//Terrain * terrain = new Terrain;
//	//terrain->SetPos({ 0.0f, 192.0f, 545.0f, 240.0f });
//	//terrain->InitSet(m_pDevice, L"DD1", L"VertexShader.txt", "VS", "TerrainPS");
//	//S_Object.AddTerrain(terrain);
//	//terrain = new Terrain;
//	//terrain->SetPos({ 545.0f, 175.0f, 545.0f + 83.0f, 175.0 + 65.0f });
//	//terrain->InitSet(m_pDevice, L"DD1", L"VertexShader.txt", "VS", "TerrainPS");
//	//S_Object.AddTerrain(terrain);
//	return true;
//}
bool GameScene1::Frame()
{
	S_Object.Frame();
	return true;
}
bool GameScene1::Release()
{
	S_Object.Release();
	return true;
}
bool GameScene1::inverseSet()
{
	return true;
}

GameScene2::GameScene2()
{}
bool GameScene2::Frame()
{
	return true;
}
bool GameScene2::Release()
{
	return true;
}
bool GameScene2::inverseSet()
{
	return true;
}

GameScene3::GameScene3()
{}
bool GameScene3::Frame()
{
	return true;
}
bool GameScene3::Release()
{
	return true;
}
bool GameScene3::inverseSet()
{
	return true;
}

GameScene4::GameScene4()
{}
bool GameScene4::Frame()
{
	return true;
}
bool GameScene4::Release()
{
	return true;
}
bool GameScene4::inverseSet()
{
	return true;
}

GameScene5::GameScene5()
{}
bool GameScene5::Frame()
{
	return true;
}
bool GameScene5::Release()
{
	return true;
}
bool GameScene5::inverseSet()
{
	return true;
}