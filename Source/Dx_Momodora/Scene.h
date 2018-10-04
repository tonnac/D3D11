#pragma once
#include "Core.h"
#include "ObjectMgr.h"
#include "Fade.h"

class Lobbymenu;

class Scene
{
public:
	Scene() = default;
	Scene(const std::tstring& Scenename);
public:
	virtual bool			Init			();
	virtual bool			Frame			() = 0;
	virtual bool			Render			();
	virtual bool			Release			() = 0;
	virtual bool			inverseInit		();
	bool					getPrevScene	() const;
public:
	void					setDevice		(ID3D11Device * pDevice,ID3D11DeviceContext* pContext);
	void					setSetting		(const bool& set);
public:
	bool					getSetting		() const;
protected:
	virtual void			SceneSet		(const bool& isInverse = false) final;
protected:
	bool					m_bSetting;
	bool					m_bSceneChange;
	bool					m_bPrevScene;
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pContext;
	const std::tstring		m_SceneName;
};

class LobbyScene : public Scene
{
public:
	LobbyScene();
public:
	bool					Frame		() override;
	bool					Render		() override;
	bool					Release		() override;
public:
	void					setLobby	(std::shared_ptr<Lobbymenu> pLobby);
protected:
	std::shared_ptr<Lobbymenu> m_pLobby;
};


//class EndScene : public Scene
//{
//public:
//	bool			Init		() override;
//	bool			Frame		() override;
//	bool			Render		() override;
//	bool			Release		() override;
//private:
//	Background		m_Background;
//};
//
