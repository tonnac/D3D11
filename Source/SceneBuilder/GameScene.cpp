#include "GameScene.h"

GameScene::GameScene(const std::tstring& Scenename) : Scene(Scenename)
{}


GameScene1::GameScene1() : GameScene(L"GameScene1")
{}
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
bool GameScene1::inverseInit()
{
	SceneSet(true);
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
bool GameScene2::inverseInit()
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
bool GameScene3::inverseInit()
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
bool GameScene4::inverseInit()
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
bool GameScene5::inverseInit()
{
	return true;
}