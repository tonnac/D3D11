#pragma once
#include "Scene.h"

class GameScene : public Scene
{
public:
	GameScene() = default;
	GameScene(const std::tstring& Scenename);
public:
	virtual bool		Frame		() override = 0;
	virtual bool		Release		() override = 0;
	virtual bool		inverseSet	() override = 0;
protected:
};


class GameScene1 : public GameScene
{
public:
	GameScene1();
public:
	bool				Frame		() override;
	bool				Release		() override;
	bool				inverseSet	() override;
};

class GameScene2 : public GameScene
{
public:
	GameScene2();
public:
	bool				Frame		() override;
	bool				Release		() override;
	bool				inverseSet	() override;
};

class GameScene3 : public GameScene
{
public:
	GameScene3();
public:
	bool				Frame		() override;
	bool				Release		() override;
	bool				inverseSet	() override;
};
class GameScene4 : public GameScene
{
public:
	GameScene4();
public:
	bool				Frame		() override;
	bool				Release		() override;
	bool				inverseSet	() override;
};

class GameScene5 : public GameScene
{
public:
	GameScene5();
public:
	bool				Frame() override;
	bool				Release() override;
	bool				inverseSet() override;
public:
};