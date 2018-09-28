#pragma once
#include "Scene.h"

class GameScene : public Scene
{
public:
	GameScene() = default;
	GameScene(const std::tstring& Scenename);
public:
	virtual bool		Frame		() override;
	virtual bool		Release		() override;
	virtual bool		inverseInit	() override;
	virtual void		InitArrow	(PlayerEffectPtr arrow) final;
protected:
};


class GameScene1 : public GameScene
{
public:
	GameScene1();
public:
	bool				Frame		() override;
};

class GameScene2 : public GameScene
{
public:
	GameScene2();
public:
	bool				Frame		() override;
};

class GameScene3 : public GameScene
{
public:
	GameScene3();
public:
	bool				Frame		() override;
	bool				Release		() override;
	bool				inverseInit	() override;
};
class GameScene4 : public GameScene
{
public:
	GameScene4();
public:
	bool				Frame		() override;
	bool				Release		() override;
	bool				inverseInit	() override;
};

class GameScene5 : public GameScene
{
public:
	GameScene5();
public:
	bool				Frame() override;
	bool				Release() override;
	bool				inverseInit() override;
public:
};