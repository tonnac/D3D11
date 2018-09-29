#pragma once
#include "ProgressBar.h"
#include "LobbyMenu.h"

class Setting : public UI
{
public:
	bool						InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
								const std::string& VSFunc = "VS", const std::string& PSFunc = "PS") override;
	bool						Frame() override;
	bool						Release() override;
	bool						Render(ID3D11DeviceContext* pContext) override;
private:
	INT							m_iBarIndex;
	Bar							m_Bar;
	std::array<ProgressBar, 2>	m_SoundBarArray;
	std::array<D3DXVECTOR2, 4>	m_BarCenterPos;
	std::array<D3DXVECTOR4, 2>	m_DrawArray;
	std::array<D3DXVECTOR4, 4>	m_BarDrawArray;
	Menu*						m_pMenu;
};