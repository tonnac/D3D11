#include "mSound.h"

bool mSound::Init()
{
	m_iEffectList = 0;
	m_iBGMList = 0;
	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(g_iMaxSound, FMOD_INIT_NORMAL, 0);
	return true;
}
bool mSound::Frame()
{
	m_pSystem->update();
	setBGMVolume();
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
		m_pEffectSound[iSoundNumber]->release();
	}
	for (int iSoundNumber = 0; iSoundNumber < 2; ++iSoundNumber)
	{
		m_pBGMSound[iSoundNumber]->release();
	}
	m_pSystem->close();
	m_pSystem->release();
	return true;
}

INT mSound::LoadEffect(const std::tstring& Filename, const bool& bPlay)
{
	FMOD_RESULT r;
	FMOD::Channel * pChannel = nullptr;
	char buffer[512] = { 0, };
	WideCharToMultiByte(CP_ACP, 0, Filename.c_str(), -1, buffer, 512, nullptr, nullptr);
	r = m_pSystem->createSound(buffer, FMOD_DEFAULT, 0, &m_pEffectSound[m_iEffectList++]);
	if (bPlay)
	{
		m_pSystem->playSound(m_pEffectSound[m_iEffectList -1], 0, false, &pChannel);
		m_pEffectCh[m_iEffectList - 1] = pChannel;
	}
	return m_iEffectList - 1;
}

INT	mSound::LoadBGM(const std::tstring& Filename, const bool& bPlay)
{
	FMOD_RESULT r;
	FMOD::Channel * pChannel = nullptr;
	char buffer[512] = { 0, };
	WideCharToMultiByte(CP_ACP, 0, Filename.c_str(), -1, buffer, 512, nullptr, nullptr);
	r = m_pSystem->createSound(buffer, FMOD_DEFAULT, 0, &m_pBGMSound[m_iBGMList++]);
	if (bPlay)
	{
		m_pSystem->playSound(m_pBGMSound[m_iBGMList - 1], 0, false, &pChannel);
		m_pEffectCh[m_iBGMList - 1] = pChannel;
	}
	return m_iBGMList - 1;
}
void mSound::PlayEffect(const INT& iIndex, const bool& bPlay, const bool& bLoop)
{
	FMOD::Channel * pChannel = nullptr;
	m_pSystem->playSound(m_pEffectSound[iIndex], 0, false, &pChannel);
	pChannel->setVolume(g_fEffectVolume);
	m_pEffectCh[iIndex] = pChannel;
}
void mSound::PlayBGM(const BGM& type)
{
	bool playing = false;
	FMOD::Channel * pChannel = nullptr;
	if (m_pBGMCh[type])
	{
		m_pBGMCh[type]->isPlaying(&playing);
	}
	if (playing == false)
	{
		m_pBGMSound[type]->setMode(FMOD_LOOP_NORMAL);
		m_pSystem->playSound(m_pBGMSound[type], 0, false, &pChannel);
		pChannel->setVolume(g_fBGMVolume);
		m_pBGMCh[type] = pChannel;
	}
}
void mSound::setBGMVolume()
{
	for (int i = 0; i < m_iBGMList; ++i)
	{
		if (SUBTRACT(g_fBGMVolume, 0.0f) < Epsilon)
		{
			g_fBGMVolume = 0.0f;
		}
		else if (SUBTRACT(g_fBGMVolume, 1.0f) < Epsilon)
		{
			g_fBGMVolume = 1.0f;
		}
		m_pBGMCh[i]->setVolume(g_fBGMVolume);
	}
}
void mSound::StopBGM(const INT& iIndex)
{
	m_pBGMCh[iIndex]->stop();
}
//void mSound::PauseSound(const INT& iIndex)
//{
//	bool paused;
//	m_pCh[iIndex]->getPaused(&paused);
//	m_pCh[iIndex]->setPaused(!paused);
//}

void mSound::LoadFile(const std::tstring& Filepath)
{
	std::tifstream fp;
	FileExceptErr(fp, Filepath);

	std::tstring buffer;
	
	int Total;

	fp >> Total;

	for (int k = 0; k < Total; ++k)
	{
		int Num;
		fp >> buffer >> Num;
		if (buffer == L"BGM")
		{
			for (int i = 0; i < Num; ++i)
			{
				fp >> buffer;
				LoadBGM(buffer);
			}
		}
		else
		{
			for (int i = 0; i < Num; ++i)
			{
				fp >> buffer;
				LoadEffect(buffer);
			}
		}
	}
}