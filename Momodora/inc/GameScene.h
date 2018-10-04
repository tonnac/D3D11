#pragma once
#include "Scene.h"
#include "MessageBox.h"

class GameScene : public Scene
{
public:
	GameScene() = default;
	GameScene(const std::tstring& Scenename);
public:
	virtual bool		Frame		() override;
	virtual bool		Render		() override;
	virtual bool		Release		() override;
	virtual bool		inverseInit	() override;
	virtual void		InitArrow	(PlayerEffectPtr arrow) final;
public:
	bool				getGameMenu() const;
	void				setGameMenu(const bool& flag);
protected:
	virtual bool		SceneChange	();
protected:
	bool				m_bGameMenu;
};


class GameScene1 : public GameScene
{
public:
	GameScene1();
protected:
	bool				SceneChange	() override;
};

class GameScene2 : public GameScene
{
public:
	GameScene2();
};

class GameScene3 : public GameScene
{
public:
	GameScene3();
public:
	bool	Init		() override;
	bool	Frame		() override;
private:
	MsgPTR	m_Box[2];
	bool	isItemGet;
	bool	isProcess;
};
class GameScene4 : public GameScene
{
public:
	GameScene4();
public:
//	bool				Frame		() override;
};

class GameScene5 : public GameScene
{
public:
	GameScene5();
public:
	bool				Frame() override;
public:
};