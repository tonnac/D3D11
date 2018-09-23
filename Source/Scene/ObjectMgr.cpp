#include "ObjectMgr.h"
#include "DirectInput.h"

ObjectMgr::ObjectMgr() : isDebug(false)
{}

bool ObjectMgr::Frame()
{
	if (S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_HOLD)
	{
		m_pBackground->Scroll(g_fSecPerFrame * 200.0f);
		for (auto& it : m_Terrainlist)
		{
			it->Scroll(-g_fSecPerFrame * 600.0f);
		}
	}
	if (S_Input.getKeyState(DIK_DELETE) == Input::KEYSTATE::KEY_PUSH)
	{
		isDebug = !isDebug;
	}
	for (auto& it : m_Terrainlist)
	{
		it->Frame();
	}
	m_pBackground->Frame();
	return true;
}
bool ObjectMgr::Render(ID3D11DeviceContext* pContext)
{
	m_pBackground->Render(pContext);
	if (isDebug == true)
	{
		for (auto& it : m_Terrainlist)
		{
			it->Render(pContext);
		}
	}
	return true;
}
bool ObjectMgr::Release()
{
	for (auto& it : m_Terrainlist)
	{
		RELEASE(it);
	}
	RELEASE(m_pBackground);
	return true;
}
void ObjectMgr::AddBackGround(Background* pBackGround)
{
	m_pBackground = pBackGround;
}
void ObjectMgr::AddTerrain(Terrain* pTerrain)
{
	m_Terrainlist.push_back(pTerrain);
}