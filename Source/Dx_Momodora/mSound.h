#pragma once
#include "d3dUtil.h"
#include "fmod.hpp"
#include "fmod.h"
#include "fmod_errors.h"

const INT g_iMaxSound = 32;

enum BGM
{
	TITLE = 0,
	STAGE
};

enum Effect_Snd
{
	ATTACK1 = 2,
	ATTACK2,
	ATTACK3,
	AIRATTACK,
	ARROW,
	JUMP,
	ROLL,
	MENUCANCLE,
	MENUMOVE,
	MENUSELECT,
	PRESSANYKEY,
	SETTINGMOVE,
	HURT
};

class mSound : public Singleton<mSound>
{
	friend class Singleton<mSound>;
private:
	mSound() {};
public:
	bool				Init		();
	bool				Frame		();
	bool				Render		();
	bool				Release		();
public:
	INT					LoadSound	(const std::tstring& Filename, const bool& = false);
	void				Play		(const INT&, const bool& = true, const bool& = false);
public:
	void				StopSound	(const INT&);
	void				PauseSound	(const INT&);
public:
	void				LoadFile	(const std::tstring& Filepath);
private:
	INT					m_iSoundList;
	FMOD::System*		m_pSystem;
	FMOD::Channel*		m_pCh[g_iMaxSound];
	FMOD::Sound*		m_pSound[g_iMaxSound];
};


#define S_Sound mSound::getInst()