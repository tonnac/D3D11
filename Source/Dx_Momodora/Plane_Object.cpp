#include "Plane_Object.h"
#include "DirectInput.h"

bool Plane_Object::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath)
{
	CreateIndexBuffer(pDevice);
	m_Object.setBlendState(pDevice);
	return Object::InitSet(pDevice, Name, TexFilepath, ShaderFilepath);
}
bool Plane_Object::Init()
{
	Sprite.push_back({ 10,87,25,36 });
	Sprite.push_back({ 58,87,26,36 });
	Sprite.push_back({ 109,87,26,36 });
	Sprite.push_back({ 163,87,27,36 });
	Sprite.push_back({ 212,87,27,36 });
	Sprite.push_back({ 261,87,27,36 });
	Sprite.push_back({ 310,87,27,36 });
	return true;
}
bool Plane_Object::Frame()
{
	static int iNum = 0;
	static float fTimer = 0.0f;
	static int iDir = 1;
	if (iNum == Sprite.size() - 1) iNum = 0;
	if (S_Input.getKeyState(DIK_9) == Input::KEYSTATE::KEY_HOLD)
	{
		for (int i = 0; i < m_VertexList.size(); ++i)
		{
			m_Centerpos.x += iDir * g_fSecPerFrame * 50.0f;
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
void Plane_Object::CreateIndexBuffer(ID3D11Device* pDevice)
{
	for (int i = 0; i < m_VertexList.size() / 4; ++i)
	{
		m_indiciesList.push_back(i * 4);
		m_indiciesList.push_back(i * 4 + 1);
		m_indiciesList.push_back(i * 4 + 2);
		m_indiciesList.push_back(i * 4 + 3);
		m_indiciesList.push_back(i * 4 + 2);
		m_indiciesList.push_back(i * 4 + 1);
	}
	m_Object.CreateBuffer(pDevice, D3D11_BIND_INDEX_BUFFER, &m_indiciesList.at(0), sizeof(DWORD) * CASTING(UINT, m_indiciesList.size()));
}
bool Plane_Object::PreRender(ID3D11DeviceContext* pContext)
{
	ID3D11Buffer * pIndex = m_Object.getIndexBuffer();
	pContext->IASetIndexBuffer(pIndex, DXGI_FORMAT_R32_UINT, 0);
	return Object::PreRender(pContext);
}
bool Plane_Object::PostRender(ID3D11DeviceContext* pContext)
{
	pContext->DrawIndexed(CASTING(UINT, m_indiciesList.size()), 0, 0);
	return true;
}

