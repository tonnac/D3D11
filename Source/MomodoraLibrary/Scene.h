#pragma once
#include "Core.h"
#include "BKObject.h"
#include "SpriteMgr.h"
#include "Player.h"
#include "Scroll.h"
#include "Enemy.h"
#include "TerrainObject.h"
#include "FadeObject.h"
#include "UI.h"

class Scene
{
public:
	Scene();
public:
	virtual bool	Init		() = 0;
	virtual bool	Frame		() = 0;
	virtual bool	Render		() = 0;
	virtual bool	Release		() = 0;
	virtual bool	inverseSet	();
	bool			getNextScene();
public:
	void * operator new(size_t sz, const char* FileName, int iLine);
	void operator delete(void * p);
protected:
	BKObject		m_BKObject;
	FadeObject*		m_pFadeObject;
	bool			m_bNextSceneStart;
};

class LobbyScene : public Scene
{
public:
	LobbyScene();
public:
	bool			Init		() override;
	bool			Frame		() override;
	bool			Render		() override;
	bool			Release		() override;
	bool			BKState		();
	void			PosSet		();
	void			AKeyReact	(const INT&);
protected:
	INT				m_miscIndex;
	LOBBYSTATE		m_state;
	bool			isSoundBar;
	bool			isPress;
	BKObject		m_bkmisc[3];
	RECT			m_miscrt[12];
	FloatPoint		m_miscpos[11];
};

class EndScene : public Scene
{
public:
	bool			Init		() override;
	bool			Frame		() override;
	bool			Render		() override;
	bool			Release		() override;
};

