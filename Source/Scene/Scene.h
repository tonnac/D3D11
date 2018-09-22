#pragma once
#include "Core.h"
#include "ObjectMgr.h"
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
protected:
	bool			m_bNextSceneStart;
};

//class LobbyScene : public Scene
//{
//public:
//	LobbyScene();
//public:
//	bool			Init		() override;
//	bool			Frame		() override;
//	bool			Render		() override;
//	bool			Release		() override;
//	bool			BKState		();
//	void			PosSet		();
//	void			AKeyReact	(const INT&);
//protected:
//	INT				m_miscIndex;
//	LOBBYSTATE		m_state;
//	bool			isSoundBar;
//	bool			isPress;
//	BKObject		m_bkmisc[3];
//	RECT			m_miscrt[12];
//	FloatPoint		m_miscpos[11];
//};
//
//class EndScene : public Scene
//{
//public:
//	bool			Init		() override;
//	bool			Frame		() override;
//	bool			Render		() override;
//	bool			Release		() override;
//};

