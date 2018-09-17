#include "mSound.h"

bool mSound::Init()
{
	m_iSoundList = 0;
	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(g_iMaxSound, FMOD_INIT_NORMAL, 0);
	return true;
}
bool mSound::Frame()
{
	m_pSystem->update();
	return true;
}
bool mSound::Render()
{
	return true;
}
bool mSound::Release()
{
	for (int iSoundNumber = 0; iSoundNumber < g_iMaxSound; ++iSoundNumber)
	{
		m_pSound[iSoundNumber]->release();
	}
	m_pSystem->close();
	m_pSystem->release();
	return true;
}

INT mSound::LoadSound(const char* pLoadName, const bool& bPlay)
{
	FMOD::Channel * pChannel = nullptr;
	m_pSystem->createSound(pLoadName, FMOD_DEFAULT, 0, &m_pSound[m_iSoundList++]);
	if (bPlay)
	{
		m_pSystem->playSound(m_pSound[m_iSoundList -1], 0, false, &pChannel);
		m_pCh[m_iSoundList - 1] = pChannel;
	}
	return m_iSoundList - 1;
}
void mSound::Play(const INT& iIndex, const bool& bPlay, const bool& bLoop)
{
	bool playing = false;
	FMOD::Channel * pChannel = nullptr;
	if (m_pCh[iIndex])
	{
		m_pCh[iIndex]->isPlaying(&playing);
	}
	if (!playing)
	{
		if (bLoop)
		{
			m_pSound[iIndex]->setMode(FMOD_LOOP_NORMAL);
		}
		else
		{
			m_pSound[iIndex]->setMode(FMOD_LOOP_OFF);
		}
		m_pSystem->playSound(m_pSound[iIndex], 0, false, &pChannel);
		m_pCh[iIndex] = pChannel;
	}
}
void mSound::StopSound(const INT& iIndex)
{
	m_pCh[iIndex]->stop();
}
void mSound::PauseSound(const INT& iIndex)
{
	bool paused;
	m_pCh[iIndex]->getPaused(&paused);
	m_pCh[iIndex]->setPaused(!paused);
}

void mSound::LoadFile(const char* szFileName)
{
	std::ifstream fp(szFileName);
	if (!fp.is_open())
	{
		MessageBox(nullptr, L"FILE OPEN", L"SOUND FILE OPEN ERROR", MB_OK);
	}

	std::string buffer;
	
	int Total;

	fp >> Total;
	for (int i = 0; i < Total; ++i)
	{
		fp >> buffer;
		LoadSound(buffer.c_str());
	}
}