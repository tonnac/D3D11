#include "Shape.h"

using namespace DirectX;

void PlaneShape::CreateVertexData()
{
	m_VertexList.resize(4);
	m_VertexList[0] = Vertex(XMFLOAT3(-1.0f, +1.0f, +0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f));
	m_VertexList[1] = Vertex(XMFLOAT3(+1.0f, +1.0f, +0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f));
	m_VertexList[2] = Vertex(XMFLOAT3(+1.0f, -1.0f, +0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));
	m_VertexList[3] = Vertex(XMFLOAT3(-1.0f, -1.0f, +0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f));
	m_DxObject.m_iVertexSize = sizeof(Vertex);
	m_DxObject.m_iNumVertex = (UINT)m_VertexList.size();
}

void PlaneShape::CreateIndexData()
{
	m_IndexList.resize(6);
	m_IndexList[0] = 0;
	m_IndexList[1] = 1;
	m_IndexList[2] = 2;
	m_IndexList[3] = 0;
	m_IndexList[4] = 2;
	m_IndexList[5] = 3;

	m_DxObject.m_iNumIndex = (UINT)m_IndexList.size();
	m_DxObject.m_iIndexSize = sizeof(DWORD);
}