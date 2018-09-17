#include "PlaneObject.h"


void PlaneObject::CreateVertexBuffer(ID3D11Device* pDevice)
{
	D2D1_RECT_F rt = { 10 / 980.0f, 87 / 3869.0f, 10 / 980.0f + 25 / 980.0f, 87 / 3869.0f + 36 / 3869.0f };
	//for (auto &iter : m_VertexList)
	//{
	//	iter.Pos.x += 0.001f;
	//}
	m_VertexList.push_back({ DirectX::XMFLOAT3(-0.15f, 0.15f, 0.5f), DirectX::XMFLOAT2(rt.left, rt.top) });
	m_VertexList.push_back({ DirectX::XMFLOAT3(0.15f,0.15f,0.5f), DirectX::XMFLOAT2(rt.right, rt.top) });
	m_VertexList.push_back({ DirectX::XMFLOAT3(-0.15f,-0.15f,0.5f), DirectX::XMFLOAT2(rt.left, rt.bottom) });
	m_VertexList.push_back({ DirectX::XMFLOAT3(0.15f,-0.15f,0.5f), DirectX::XMFLOAT2(rt.right, rt.bottom) });
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
bool PlaneObject::Frame()
{
	for (auto& iter : m_VertexList)
	{
		iter.Pos.x += 0.03f;
	}
	
	m_ConstantData.Color.x = cosf(g_fGameTime) * 0.5f + 0.5f;
	m_ConstantData.Color.y = sinf(g_fGameTime) * 0.5f + 0.5f;
	m_ConstantData.Color.z = cosf(g_fGameTime) * 0.5f + 0.5f;
	m_ConstantData.Color.w = 1.0f;
	m_ConstantData.Util.x = g_fGameTime * 50.0f, m_ConstantData.Util.y = 1.0f, m_ConstantData.Util.z = 1.0f, m_ConstantData.Util.w = 1.0f;
	return true;
}