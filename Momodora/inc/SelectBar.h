#pragma once
#include "Bar.h"

class SelectBar : public Bar
{
public:
	bool			InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& ShaderFilepath,
					const std::string& VSFunc = "VS", const std::string& PSFunc = "PS");
	bool			Frame() override;
private:
	FLOAT			m_fVal;
	INT				m_iMul;
};