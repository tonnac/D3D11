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
	ATTACK1 = 0,
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
	HURT,
	ITEM
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
	INT					LoadEffect	(const std::tstring& Filename, const bool& = false);
	INT					LoadBGM		(const std::tstring& Filename, const bool& = false);
public:
	void				PlayEffect	(const INT&, const bool& = false, const bool& = false);
	void				PlayBGM		(const BGM& type);
public:
	void				setBGMVolume();
public:
	void				StopBGM		(const INT&);
//	void				PauseSound	(const INT&);
public:
	void				LoadFile	(const std::tstring& Filepath);
private:
	INT					m_iBGMList;
	INT					m_iEffectList;
	FMOD::System*		m_pSystem;
	FMOD::Channel*		m_pBGMCh[2];
	FMOD::Channel*		m_pEffectCh[g_iMaxSound];
	FMOD::Sound*		m_pBGMSound[2];
	FMOD::Sound*		m_pEffectSound[g_iMaxSound];
};

#define S_Sound mSound::getInst()