#pragma once
#include "Menu.h"
#include "Button.h"
#include "Font.h"
#include "Bar.h"

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
private:
	bool StateFrame();
private:
	std::array<Button, 5>	m_Button;
	std::array<Font, 5>		m_Font;
	Bar						m_Bar;
};