#pragma once
#include "Menu.h"
#include "Button.h"
#include "SelectBar.h"
#include "Font.h"

enum class INVENSTATE : char
{
	MAIN,
	CONSUMASELECT,
	EQUIPSELECT
};

class Inventory : public Menu
{
public:
	Inventory();
public:
	bool InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
		const std::string& VSFunc = "VS", const std::string& PSFunc = "PS") override;
	bool Frame() override;
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();
private:
	bool StateFrame();
	bool IndexFrame();
private:
	INT							m_iConsumable;
	INT							m_iEquip;
	INVENSTATE					m_State;
	std::array<D3DXVECTOR2, 5>	m_itemslotCen;
	Button						m_Button;
	SelectBar					m_SelectBar;
	Font						m_Font;
};