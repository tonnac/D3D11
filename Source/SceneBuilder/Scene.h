#pragma once
#include "Core.h"
#include "ObjectMgr.h"


enum ObjectEnum : unsigned char
{
	BACKGROUND = 0,
	TERRAIN
};


class Scene
{
public:
	Scene() = default;
	Scene(const std::tstring& Scenename);
public:
	virtual bool			Init		() final;
	virtual bool			Frame		() = 0;
	virtual bool			Render		() final;
	virtual bool			Release		() = 0;
	virtual bool			inverseSet	();
	bool					getNextScene();
public:
	void					setDevice(ID3D11Device * pDevice,ID3D11DeviceContext* pContext);
protected:
	virtual void			SceneSet(const bool& isInverse = false) final;
protected:
	bool					m_bNextSceneStart;
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pContext;
	const std::tstring		m_SceneName;
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

