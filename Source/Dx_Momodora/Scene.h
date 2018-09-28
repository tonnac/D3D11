#pragma once
#include "Core.h"
#include "ObjectMgr.h"


enum ObjectEnum : unsigned char
{
	BACKGROUND = 0,
	TERRAIN,
	PLAYER,
	DOWN,
	LADDER
};


class Scene
{
public:
	Scene() = default;
	Scene(const std::tstring& Scenename);
public:
	virtual bool							Init			() final;
	virtual bool							Frame			() = 0;
	virtual bool							Render			() final;
	virtual bool							Release			() = 0;
	virtual bool							inverseInit		();
	bool									getSceneChange	();
public:
	void									setDevice		(ID3D11Device * pDevice,ID3D11DeviceContext* pContext);
protected:
	virtual void							SceneSet		(const bool& isInverse = false) final;
protected:
	bool									m_bSceneChange;
	ID3D11Device*							m_pDevice;
	ID3D11DeviceContext*					m_pContext;
	const std::tstring						m_SceneName;
};

class LobbyScene : public Scene
{
public:
	LobbyScene();
public:
	bool			inverseInit	() override;
	bool			Frame		() override;
	bool			Release		() override;
protected:
	INT				m_miscIndex;
	bool			isSoundBar;
	bool			isPress;
	Background*		m_pBackground;
};
//
//class EndScene : public Scene
//{
//public:
//	bool			Init		() override;
//	bool			Frame		() override;
//	bool			Render		() override;
//	bool			Release		() override;
//};

