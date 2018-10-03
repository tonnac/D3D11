#pragma once
#include "Menu.h"
#include "InGameBar.h"
#include "Bar.h"

enum class IGMSTATE : char
{
	DEFAULT,
	INVENTORY,
	MAINITEM,
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
private:
	bool StateFrame();
private:
	IGMSTATE				m_State;
	InGameBar				m_IngameBar;
	std::array<Bar, 3>		m_Bar;
};