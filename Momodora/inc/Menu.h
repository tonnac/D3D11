#pragma once
#include "UI.h"
#include "DirectInput.h"
#include "Fade.h"
#include "mSound.h"

class Menu : public UI
{
public:
	Menu();
	void			setSetting(const bool& set);
	bool			getSetting() const;
public:
	virtual bool	DownKey();
	virtual bool	UPKey();
protected:
	INT				m_iIndex;
private:
	bool			m_bSetting;
};