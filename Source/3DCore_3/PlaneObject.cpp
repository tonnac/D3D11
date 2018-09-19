#include "PlaneObject.h"
#include "DirectInput.h"


void PlaneObject::CreateVertexBuffer(ID3D11Device* pDevice)
{
	m_VertexList.push_back({ DirectX::XMFLOAT3(-0.15f, 0.15f, 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f) });
	m_VertexList.push_back({ DirectX::XMFLOAT3(0.15f,0.15f,0.5f), DirectX::XMFLOAT2(0.0f, 0.0f) });
	m_VertexList.push_back({ DirectX::XMFLOAT3(-0.15f,-0.15f,0.5f), DirectX::XMFLOAT2(0.0f, 0.0f) });
	m_VertexList.push_back({ DirectX::XMFLOAT3(0.15f,-0.15f,0.5f), DirectX::XMFLOAT2(0.0f, 0.0f) });
	Object::CreateVertexBuffer(pDevice);
}
void PlaneObject::CreateIndexBuffer(ID3D11Device* pDevice)
{
	m_indiciesList.push_back(0);
	m_indiciesList.push_back(1);
	m_indiciesList.push_back(2);
	m_indiciesList.push_back(1);
	m_indiciesList.push_back(3);
	m_indiciesList.push_back(2);
	
	Object::CreateIndexBuffer(pDevice);
}
bool PlaneObject::Init()
{
	Sprite.push_back({ 10,87,25,36 });
	Sprite.push_back({ 58,87,26,36 });
	Sprite.push_back({ 109,87,26,36 });
	Sprite.push_back({ 163,87,27,36 });
	Sprite.push_back({ 212,87,27,36 });
	Sprite.push_back({ 261,87,27,36 });
	Sprite.push_back({ 310,87,27,36 });
	return Object::Init();
}
bool PlaneObject::Frame()
{
	static int iNum = 0;
	static float fTimer = 0.0f;
	static int iDir = 1;
	if (iNum == Sprite.size() - 1) iNum = 0;
	if (S_Input.getKeyState(DIK_9) == Input::KEYSTATE::KEY_HOLD)
	{
		for (int i = 0; i < m_VertexList.size(); ++i)
		{
			m_Centerpos.x += iDir*g_fSecPerFrame * 50.0f;
		}
	}
//	D2D1_RECT_F rt = { 10 / 980.0f, 87 / 3869.0f, 10 / 980.0f + 25 / 980.0f, 87 / 3869.0f + 36 / 3869.0f };
	DirectX::XMFLOAT2 x1 = { Sprite[iNum].left / 980.f, Sprite[iNum].top / 3869.0f };
	DirectX::XMFLOAT2 x2 = { (Sprite[iNum].left + Sprite[iNum].right) / 980.f, Sprite[iNum].top / 3869.0f };
	DirectX::XMFLOAT2 x3 = { Sprite[iNum].left / 980.f, (Sprite[iNum].top + Sprite[iNum].bottom) / 3869.0f };
	DirectX::XMFLOAT2 x4 = { (Sprite[iNum].left + Sprite[iNum].right) / 980.f, (Sprite[iNum].top + Sprite[iNum].bottom) / 3869.0f };
	m_VertexList[0].TexPos = x1;
	m_VertexList[1].TexPos = x2;
	m_VertexList[2].TexPos = x3;
	m_VertexList[3].TexPos = x4;
	m_rtCollision.right = Sprite[iNum].right;
	m_rtCollision.bottom = Sprite[iNum].bottom;
	fTimer += g_fSecPerFrame;
	if (fTimer >= 0.3f)
	{
		fTimer -= 0.3f;
		iNum++;
	}
	
	m_ConstantData.Color.x = cosf(g_fGameTime) * 0.5f + 0.5f;
	m_ConstantData.Color.y = sinf(g_fGameTime) * 0.5f + 0.5f;
	m_ConstantData.Color.z = cosf(g_fGameTime) * 0.5f + 0.5f;
	m_ConstantData.Color.w = 1.0f;
	m_ConstantData.Util.x = g_fGameTime * 50.0f, m_ConstantData.Util.y = 1.0f, m_ConstantData.Util.z = 1.0f, m_ConstantData.Util.w = 1.0f;
	return Object::Frame();
}