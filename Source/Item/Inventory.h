#pragma once
#include "Menu.h"
#include "Button.h"
#include "SelectBar.h"
#include "ConsumableItem.h"
#include "ItemFont.h"
#include "QuickSlot.h"

enum class INVENSTATE : char
{
	MAIN,
	CONSUMASELECT,
	EQUIPSELECT
};

class ItemSlot : public UI
{
public:
	ItemSlot() : m_bEmpty(false), m_pItem(nullptr)
	{}
public:
	bool InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& ShaderFilepath,
				const std::string& VSFunc = "VS", const std::string& PSFunc = "PS");
	bool Frame();
	bool Render(ID3D11DeviceContext* pContext);
public:
	void setItem(ItemPtr pItem);
private:
	bool	m_bEmpty;
	ItemPtr m_pItem;
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
public:
	void AddConsumable(ItemPtr item);
//	void AddEquip(ItemPtr item);
public:
	void DelConsumable(const INT& key);
private:
	bool IndexRender(ID3D11DeviceContext* pContext);
private:
	bool StateFrame();
	bool IndexFrame();
private:
	std::function<bool(ItemPtr, ItemPtr)>	CompFunc;
private:
	INT										m_iConsumableIndex;
	INT										m_iEquipIndex;
	INT										m_iConsumable;
	INT										m_iEquip;
	INVENSTATE								m_State;
	std::array<D3DXVECTOR2, 5>				m_itemslotCen;
	std::array<ItemSlot, 5>					m_SlotArray;
	std::vector<ItemPtr>					m_Consumable;
	std::vector<ItemPtr>					m_Equip;
	Button									m_Button;
	SelectBar								m_SelectBar;
	Font									m_Font;
	ItemFont								m_Description;
	std::map<INT, ItemFont*>				m_ConsumableName;
	std::map<INT, ItemFont*>				m_EquipName;
	QuickSlot								m_QuickSlot;
private:
	ID3D11Device *							m_pDevice;
};