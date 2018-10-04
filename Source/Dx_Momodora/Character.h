#pragma once
#include "Plane_Object.h"
#include "Sprite.h"

class State;
using StateList = std::map<std::tstring, State*>;

class Character : public Plane_Object
{
public:
	Character();
	virtual ~Character();
public:
	virtual bool		InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
						const std::string& VSFunc = "VS", const std::string& PSFunc = "PS") override;
	bool				Init() override;
	bool				Frame() override;
	bool				PostRender(ID3D11DeviceContext* pContext) override;
	bool				Release() override;
public:
	void				AddState(const std::tstring& statename, State* pState);
	virtual void		setState(const std::tstring& statename);
	void				reverseDir();
public:
	bool				isLanding() const;
	void				setLanding(const bool& landing);
public:
	std::tstring		getCurrentState();
	INT					getDir() const;
	INT					getHP() const;
	FLOAT				getSpeed() const;
	bool				getDead() const;
public:
	virtual void		setHP(const FLOAT& iVal);
	void				setSpeed(const FLOAT& fVal);
	virtual void		setDead();
private:
	void				reverseSet();
protected:
	bool				isDead;
	INT					m_HP;
	bool				m_bLanding;
	FLOAT				m_fSpeed;
	INT					m_iDir;
	ID3D11PixelShader * m_pShader;
	State*				m_pCurrentState;
	StateList			m_StateList;
};