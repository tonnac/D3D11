#pragma once
#include "Misc.h"

class MiscSet
{
public:
	bool		InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath);
	bool		Frame();
	bool		Render(ID3D11DeviceContext* pContext);
	bool		Release();
private:
	std::array<Misc, 4> m_Misc;
};