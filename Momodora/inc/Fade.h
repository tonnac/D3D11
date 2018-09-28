#pragma once
#include "Plane_Object.h"

class Fade : public Plane_Object
{
public:
	Fade();
public:
	bool		InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& ShaderFilepath,
				const std::string& VSFunc = "VS", const std::string& PSFunc = "PS");
	bool		Frame() override;
	void		setOn(const bool& Switch);
	void		FadeIn();
	void		FadeOut();
	void		setDivideTime(const FLOAT& time);
private:
	bool		isOn;
	FLOAT		m_fDivideTime;
	FLOAT		m_fAngle;
	FLOAT		m_fInitVal;
	const FLOAT	m_fMaxAngle;
};