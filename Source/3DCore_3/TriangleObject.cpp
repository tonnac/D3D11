#include "TriangleObject.h"

bool TriangleObject::Frame()
{
	m_ConstantData.Color.x = cosf(g_fGameTime) * 0.5f + 0.5f;
	m_ConstantData.Color.y = sinf(g_fGameTime) * 0.5f + 0.5f;
	m_ConstantData.Color.z = cosf(g_fGameTime) * 0.5f + 0.5f;
	m_ConstantData.Color.w = 1.0f;
	m_ConstantData.Util.x = g_fGameTime * 50.0f, m_ConstantData.Util.y = 1.0f, m_ConstantData.Util.z = 1.0f, m_ConstantData.Util.w = 1.0f;
	return true;
}
bool TriangleObject::PostRender(ID3D11DeviceContext* pContext)
{
	pContext->Draw(CASTING(UINT, m_VertexList.size()), 0);
	return true;
}
void TriangleObject::CreateVertexBuffer(ID3D11Device* pDevice)
{
	D2D1_RECT_F rt = { 10 / 980.0f, 87 / 3869.0f, 10 / 980.0f + 25 / 980.0f, 87 / 3869.0f + 36 / 3869.0f };
	m_VertexList.push_back({ DirectX::XMFLOAT3(-0.15f, 0.15f, 0.5f), DirectX::XMFLOAT2(rt.left, rt.top) });
	m_VertexList.push_back({ DirectX::XMFLOAT3(0.15f,0.15f,0.5f), DirectX::XMFLOAT2(rt.right, rt.top) });
	m_VertexList.push_back({ DirectX::XMFLOAT3(-0.15f,-0.15f,0.5f), DirectX::XMFLOAT2(rt.left, rt.bottom) });
	Object::CreateVertexBuffer(pDevice);
}
void TriangleObject::CreateIndexBuffer(ID3D11Device* pDevice)
{
	return;
}