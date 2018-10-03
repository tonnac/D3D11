#include "Inventory.h"
#include "Player.h"


bool ItemSlot::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& ShaderFilepath,
						const std::string& VSFunc, const std::string& PSFunc)
{
	CreateVertexBuffer(pDevice);
	m_Object.CreateShader(pDevice, Name, ShaderFilepath, VSFunc, PSFunc);
	CreateConstantBuffer(pDevice);
	m_Object.CreateRasterizer(pDevice);
	CreateIndexBuffer(pDevice);
	m_Object.setBlendState(pDevice);
	return true;
}
bool ItemSlot::Frame()
{
	if (m_pItem != nullptr)
	{
		m_Object.setTexture(m_pItem->getIcon());
		m_ConstantData.Util.x = m_pItem->getIcon()->getImageSize().x;
		m_ConstantData.Util.y = m_pItem->getIcon()->getImageSize().y;
		m_VertexList[0].TexPos = { 0.0f,0.0f };
		m_VertexList[1].TexPos = { m_ConstantData.Util.x, 0.0f };
		m_VertexList[2].TexPos = { 0.0f,m_ConstantData.Util.y };
		m_VertexList[3].TexPos = { m_ConstantData.Util.x,m_ConstantData.Util.y };
		m_VertexList[0].Pos = { m_Centerpos.x - m_ConstantData.Util.x * 0.5f, m_Centerpos.y - m_ConstantData.Util.y * 0.5f, 0.5f };
		m_VertexList[1].Pos = { m_Centerpos.x + m_ConstantData.Util.x * 0.5f, m_Centerpos.y - m_ConstantData.Util.y * 0.5f, 0.5f };
		m_VertexList[2].Pos = { m_Centerpos.x - m_ConstantData.Util.x * 0.5f, m_Centerpos.y + m_ConstantData.Util.y * 0.5f, 0.5f };
		m_VertexList[3].Pos = { m_Centerpos.x + m_ConstantData.Util.x * 0.5f, m_Centerpos.y + m_ConstantData.Util.y * 0.5f, 0.5f };
	}
	return true;
}
bool ItemSlot::Render(ID3D11DeviceContext* pContext)
{
	if (m_pItem != nullptr)
	{
		PreRender(pContext);
		PostRender(pContext);
	}
	return true;
}

void ItemSlot::setItem(ItemPtr pItem)
{
	m_pItem = std::move(pItem);
}
Inventory::Inventory() : m_iConsumable(0), m_iEquip(0), m_iConsumableIndex(0), m_iEquipIndex(0)
{
	CompFunc = [](ItemPtr item, ItemPtr item2)
	{
		if (item->getID() < item2->getID())
		{
			return true;
		}
		return false;
	};
}

bool Inventory::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
	const std::string& VSFunc, const std::string& PSFunc)
{
	m_pDevice = pDevice;
	UI::InitSet(pDevice, Name, TexFilepath, ShaderFilepath, VSFunc, PSFunc);
	D3DXVECTOR2 TexPos = m_Object.getTexture()->getImageSize();
	SetTexPos({ 0.0f,0.0f,TexPos.x,TexPos.y });
	SetCenterPos({ g_rtClient.right * 0.5f, g_rtClient.bottom * 0.5f });

	m_VertexList[0].Color = { 0.0f, 1.0f, 0.0f, 1.0f };
	m_VertexList[1].Color = { 1.0f, 0.0f, 0.0f, 1.0f };
	m_VertexList[2].Color = { 0.0f, 0.0f, 1.0f, 1.0f };
	m_VertexList[3].Color = { 0.5f, 0.5f, 0.5f, 1.0f };

	m_itemslotCen[0] = { 87.0f, 156.0f };
	m_itemslotCen[1] = { 165.0f, 156.0f };
	m_itemslotCen[2] = { 243.0f, 156.0f };
	m_itemslotCen[3] = { 87.0f, 321.0f };
	m_itemslotCen[4] = { 165.0f, 321.0f };

	std::shared_ptr<FlowerItem> pFlower(new FlowerItem);
	pFlower->Initset(pDevice);

	std::shared_ptr<SeedItem> pSeed(new SeedItem);
	pSeed->Initset(pDevice);

	for(int i=0; i<m_SlotArray.size(); ++i)
	{
		m_SlotArray[i].InitSet(pDevice, L"Basic", Filepath::m_Txtpath[L"Shader"]);
		m_SlotArray[i].SetCenterPos(m_itemslotCen[i]);
	}
	m_Font.InitSet(pDevice, L"Basic", Filepath::m_Pngpath[L"None"], Filepath::m_Txtpath[L"Shader"]);
	m_Font.SetCenterPos({ 618.0f, 150.0f });

	m_SelectBar.InitSet(pDevice, L"SelectBar", Filepath::m_Txtpath[L"Shader"], "VS", "SelectPSS");
	m_SelectBar.SetCenterPos({ 685.5f, 150.0f });

	m_Button.InitSet(pDevice, L"Select", Filepath::m_Pngpath[L"InvenSel"], Filepath::m_Txtpath[L"Shader"], "VS", "SelectPS");
	m_Button.SetCenterPos(m_itemslotCen[m_iIndex]);

	m_Description.InitSet(pDevice, L"Basic", Filepath::m_Txtpath[L"Shader"]);
	m_Description.SetCenterPos({ g_rtClient.right * 0.5f, 522.5f });

	AddConsumable(pFlower);
	AddConsumable(pSeed);
	return true;
}

bool Inventory::Frame()
{
	if (S_Input.getKeyState(DIK_I) == Input::KEYSTATE::KEY_PUSH)
	{
		DelConsumable(0);
	}
	StateFrame();
	m_LengthDiff.x = (m_VertexList[1].TexPos.x - m_VertexList[0].TexPos.x) * 0.5f;
	m_LengthDiff.y = (m_VertexList[2].TexPos.y - m_VertexList[0].TexPos.y) * 0.5f;
	m_Button.Frame();
	for (int i = 0; i < m_SlotArray.size(); ++i)
	{
		m_SlotArray[i].setItem(m_QuickSlot.getItem(i));
		m_SlotArray[i].Frame();
	}
	if (m_State == INVENSTATE::CONSUMASELECT)
	{
		m_SelectBar.Frame();
	}
	
	m_Font.Frame();
	return UI::Frame();
}

bool Inventory::Render(ID3D11DeviceContext* pContext)
{
	Menu::Render(pContext);
	for (auto&it : m_SlotArray)
	{
		it.Render(pContext);
	}
	if (m_State == INVENSTATE::MAIN)
	{
		m_Button.Render(pContext);
	}
	if (m_State == INVENSTATE::CONSUMASELECT || m_State == INVENSTATE::EQUIPSELECT)
	{
		m_SelectBar.Render(pContext);
	}
	m_Description.Render(pContext);
	IndexRender(pContext);
	m_Font.Render(pContext);
	return true;
}
bool Inventory::Release()
{
	for (auto &it : m_SlotArray)
	{
		it.Release();
	}
	if (m_ConsumableName.empty() == false)
	{
		for (auto&it : m_ConsumableName)
		{
			it.second->Release();
			delete it.second;
		}
	}
	if (m_EquipName.empty() == false)
	{
		for (auto&it : m_EquipName)
		{
			it.second->Release();
			delete it.second;
		}
	}
	m_Button.Release();
	m_Font.Release();
	m_SelectBar.Release();
	return Menu::Release();
}
void Inventory::AddConsumable(ItemPtr item)
{
	INT Key = item->getID();
	if (m_ConsumableName[Key] == nullptr)
	{
		ItemFont* pFont = new ItemFont;
		pFont->InitSet(m_pDevice, L"Basic", Filepath::m_Txtpath[L"Shader"]);
		m_ConsumableName[Key] = pFont;
		m_ConsumableName[Key]->setTexture(item->getName());
	}
	++m_iConsumable;
	m_Consumable.push_back(std::move(item));
	std::sort(m_Consumable.begin(), m_Consumable.end(), CompFunc);
}
//void Inventory::AddEquip(ItemPtr item)
//{
//	INT Key = item->getID();
//	m_Consumable.insert(std::make_pair(Key, std::move(item)));
//}
void Inventory::DelConsumable(const INT& key)
{
	std::vector<ItemPtr>::iterator iter;
	auto foo = [key](ItemPtr pitem)
	{
		if (pitem->getID() == key)
		{
			return true;
		}
		return false;
	};
	iter = std::find_if(m_Consumable.begin(), m_Consumable.end(), foo);
	m_Consumable.erase(iter);
	ItemFont* pFont = m_ConsumableName[key];
	m_ConsumableName.erase(key);
	--m_iConsumable;
	delete pFont;
}
bool Inventory::IndexFrame()
{
	switch (m_iIndex)
	{
	case 0:
	{
		RightKey();
		if (LeftKey() == true)
		{
			m_iIndex = 4;
		}
		if (DownKey() == true)
		{
			m_iIndex = 3;
		}
		if (UPKey() == true)
		{
			m_iIndex = 3;
		}

	}break;
	case 1:
	{
		RightKey();
		LeftKey();
		if (DownKey() == true)
		{
			m_iIndex = 4;
		}
		if (UPKey() == true)
		{
			m_iIndex = 4;
		}
	}break;
	case 2:
	{
		RightKey();
		LeftKey();
		if (DownKey() == true)
		{
			m_iIndex = 4;
		}
		if (UPKey() == true)
		{
			m_iIndex = 4;
		}
	}break;
	case 3:
	{
		RightKey();
		LeftKey();
		if (DownKey() == true)
		{
			m_iIndex = 0;
		}
		if (UPKey() == true)
		{
			m_iIndex = 0;
		}
	}break;
	case 4:
	{
		if (RightKey() == true)
		{
			m_iIndex = 0;
		}
		LeftKey();
		if (DownKey() == true)
		{
			m_iIndex = 1;
		}
		if (UPKey() == true)
		{
			m_iIndex = 1;
		}
	}break;
	default:
		break;
	}
	if (m_iIndex >= 0 && m_iIndex < 3)
	{
		std::map<INT, ItemFont*>::iterator iter = m_ConsumableName.begin();
		for (int i = 0; i < m_Consumable.size(); ++i)
		{
			iter->second->SetCenterPos({ 618.0f, 150.0f + (i + 1)*50.0f });
			iter->second->Frame();
			++iter;
		}
	}
	if (m_iIndex < 3 && m_iIndex >= 0 && S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_PUSH)
	{
		m_State = INVENSTATE::CONSUMASELECT;
		return true;
	}
	else if (S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_PUSH)
	{
		m_State = INVENSTATE::EQUIPSELECT;
		return true;
	}
	ItemPtr item = m_QuickSlot.getItem(m_iIndex);
	if (item != nullptr)
	{
		m_Description.setTexture(item->getDesc());
		m_Description.Frame();
	}
	else
	{
		m_Description.setTexture(nullptr);
		m_Description.Frame();
	}
	m_Button.SetCenterPos(m_itemslotCen[m_iIndex]);
	return true;
}
bool Inventory::IndexRender(ID3D11DeviceContext* pContext)
{
	if (m_iIndex >= 0 && m_iIndex < 3)
	{
		std::map<INT, ItemFont*>::iterator iter = m_ConsumableName.begin();
		for (int i = 0; i < m_Consumable.size(); ++i)
		{
			iter->second->Render(pContext);
			++iter;
		}
	}
	return true;
}
bool Inventory::StateFrame()
{
	switch (m_State)
	{
	case INVENSTATE::MAIN:
	{
		IndexFrame();
	}break;
	case INVENSTATE::CONSUMASELECT:
	{
		if (S_Input.getKeyState(DIK_UP) == Input::KEYSTATE::KEY_PUSH)
		{
			--m_iConsumableIndex;
			if (m_iConsumableIndex < 0)
			{
				m_iConsumableIndex = 0;
			}
		}
		if (S_Input.getKeyState(DIK_DOWN) == Input::KEYSTATE::KEY_PUSH)
		{
			++m_iConsumableIndex;
			if (m_iConsumableIndex > m_iConsumable)
			{
				m_iConsumableIndex = m_iConsumable;
			}
		}
		if (m_iConsumableIndex == 0 && S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_PUSH)
		{
			ItemPtr item = m_QuickSlot.getItem(m_iIndex);
			if (item != nullptr)
			{
				m_QuickSlot.DelItem(m_iIndex);
				AddConsumable(item);
			}
			m_State = INVENSTATE::MAIN;
		}
		else if (S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_PUSH)
		{
			if (m_Consumable[m_iConsumableIndex - 1] != nullptr && m_QuickSlot.getItem(m_iIndex) != nullptr)
			{
				ItemPtr item = m_QuickSlot.getItem(m_iIndex);
				m_QuickSlot.DelItem(m_iIndex);
				m_QuickSlot.AddItem(m_Consumable[m_iConsumableIndex - 1], m_iIndex);
				DelConsumable(m_Consumable[m_iConsumableIndex - 1]->getID());
				AddConsumable(item);
			}
			else if (m_Consumable[m_iConsumableIndex - 1] != nullptr)
			{
				m_QuickSlot.AddItem(m_Consumable[m_iConsumableIndex - 1], m_iIndex);
				DelConsumable(m_Consumable[m_iConsumableIndex - 1]->getID());
			}
			m_iConsumableIndex = 0;
			m_State = INVENSTATE::MAIN;
		}
		if (S_Input.getKeyState(DIK_S) == Input::KEYSTATE::KEY_PUSH)
		{
			m_State = INVENSTATE::MAIN;
			m_iConsumableIndex = 0;
		}
		if (m_iConsumableIndex == 0)
		{
			m_Description.setTexture(nullptr);
			m_Description.Frame();
		}
		else if (m_iConsumableIndex > 0 && m_Consumable[m_iConsumableIndex - 1] != nullptr)
		{
			m_Description.setTexture(m_Consumable[m_iConsumableIndex - 1]->getDesc());
			m_Description.Frame();
		}
		else
		{
			m_Description.setTexture(nullptr);
			m_Description.Frame();
		}
		m_SelectBar.SetCenterPos({ 685.5f, 150.0f + (50.0f * m_iConsumableIndex) });
	}break;
	case INVENSTATE::EQUIPSELECT:
	{
		if (S_Input.getKeyState(DIK_S) == Input::KEYSTATE::KEY_PUSH)
		{
			m_State = INVENSTATE::MAIN;
			m_iEquip = 0;
			return true;
		}
	}break;
	default:
		break;
	}
	return true;
}
