#pragma once
#include "Menu.h"
#include "InGameBar.h"
#include "Bar.h"

enum class IGMSTATE : char
{
	DEFAULT,
	INVENTORY,
	MAINITEM,
	SETTING,
	MAINMENU
};

class InGameMenu : public Menu
{
public:
	InGameMenu();
public:
	bool InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
				const std::string& VSFunc = "VS", const std::string& PSFunc = "PS") override;
	bool Frame() override;
	bool Render(ID3D11DeviceContext* pContext) override;
	bool Release() override;
public:
	bool RightKey() override;
	bool LeftKey() override;
	IGMSTATE getState() const;
public:
	void	setSetting(const bool& set) override;
private:
	bool IndexFrame();
	bool StateFrame();
	bool StateRender(ID3D11DeviceContext* pContext);
private:
	std::array<D3DXVECTOR2, 2>	m_MaplePos;
	INT							m_iMapleIndex;
	IGMSTATE					m_State;
	InGameBar					m_IngameBar;
	std::array<Bar, 4>			m_Bar;
};