#pragma once
#include "Scene.h"

class GameScene : public Scene
{
public:
	GameScene();
public:
	virtual bool		Init		() override = 0;
	virtual bool		Frame		() override = 0;
	virtual bool		Render		() override = 0;
	virtual bool		Release		() override = 0;
	virtual bool		inverseSet	() override = 0;
protected:
	ID3D11Device *			m_pDevice;
	ID3D11DeviceContext*	m_pContext;
};


class GameScene1 : public GameScene
{
public:
	GameScene1();
public:
	bool				SendDevice	(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	bool				Init		() override;
	bool				Frame		() override;
	bool				Render		() override;
	bool				Release		() override;
	bool				inverseSet	() override;
};

class GameScene2 : public GameScene
{
public:
	GameScene2();
public:
	bool				Init		() override;
	bool				Frame		() override;
	bool				Render		() override;
	bool				Release		() override;
	bool				inverseSet	() override;
};

class GameScene3 : public GameScene
{
public:
	GameScene3();
public:
	bool				Init		() override;
	bool				Frame		() override;
	bool				Render		() override;
	bool				Release		() override;
	bool				inverseSet	() override;
};
class GameScene4 : public GameScene
{
public:
	GameScene4();
public:
	bool				Init		() override;
	bool				Frame		() override;
	bool				Render		() override;
	bool				Release		() override;
	bool				inverseSet	() override;
};

class GameScene5 : public GameScene
{
public:
	GameScene5();
public:
	bool				Init() override;
	bool				Frame() override;
	bool				Render() override;
	bool				Release() override;
	bool				inverseSet() override;
public:
};