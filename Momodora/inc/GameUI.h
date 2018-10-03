#pragma once
#include "Inventory.h"

class GameUI : public UI
{
public:
	GameUI() = default;
	~GameUI();
public:
	bool		InitSet(ID3D11Device* pDevice);

	bool		Frame() override;
	bool		Render(ID3D11DeviceContext* pContext) override;
	bool		Release();
private:
	HPBar		m_HPBar;
	Bar			m_HPBk;
	Bar			m_Moo;
	Bar			m_QuickBk;
	ItemSlot	m_QuickNum;
	ItemFont	m_ItemNum;
};