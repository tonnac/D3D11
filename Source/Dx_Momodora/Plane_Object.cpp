#include "Plane_Object.h"
#include "DirectInput.h"

Plane_Object::Plane_Object() : m_fScale(1.0f)
{
	m_VertexList.resize(4);
}

bool Plane_Object::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
							const std::string& VSFunc, const std::string& PSFunc)
{
	CreateIndexBuffer(pDevice);
	m_Object.setBlendState(pDevice);
	return Object::InitSet(pDevice, Name, TexFilepath, ShaderFilepath, VSFunc, PSFunc);
}
void Plane_Object::SetTexPos(const D3DXVECTOR4& DrawVec)
{
	m_VertexList[0].TexPos = { DrawVec.x, DrawVec.y };
	m_VertexList[1].TexPos = { DrawVec.z, DrawVec.y };
	m_VertexList[2].TexPos = { DrawVec.x, DrawVec.w };
	m_VertexList[3].TexPos = { DrawVec.z, DrawVec.w };
}
bool Plane_Object::Frame()
{
	m_VertexList[0].Pos.x = m_Centerpos.x - m_LengthDiff.x;
	m_VertexList[0].Pos.y = m_Centerpos.y - m_LengthDiff.y;
	m_VertexList[1].Pos.x = m_Centerpos.x + m_LengthDiff.x;
	m_VertexList[1].Pos.y = m_VertexList[0].Pos.y;
	m_VertexList[2].Pos.x = m_VertexList[0].Pos.x;
	m_VertexList[2].Pos.y = m_Centerpos.y + m_LengthDiff.y;
	m_VertexList[3].Pos.x = m_VertexList[1].Pos.x;
	m_VertexList[3].Pos.y = m_VertexList[2].Pos.y;

	m_rtCollision.left = m_VertexList[0].Pos.x;
	m_rtCollision.top = m_VertexList[0].Pos.y;
	m_rtCollision.right = m_VertexList[3].Pos.x;
	m_rtCollision.bottom = m_VertexList[3].Pos.y;
	return true;
}
COL Plane_Object::Collision(Object* pObject, FLOAT* ColSize)
{
	D2D1_RECT_F ColRT = pObject->getCollisionRT();
	D3DXVECTOR2 Cen = pObject->getCenterPos();

	FLOAT xDiff = abs(m_Centerpos.x - Cen.x);
	FLOAT yDiff = abs(m_Centerpos.y - Cen.y);

	FLOAT xLen1 = m_rtCollision.right - m_Centerpos.x;
	FLOAT yLen1 = m_rtCollision.bottom - m_Centerpos.y;

	FLOAT xLen2 = ColRT.right - Cen.x;
	FLOAT yLen2 = ColRT.bottom - Cen.y;

	if (xLen1 + xLen2 >= xDiff && yLen1 + yLen2 >= yDiff)
	{
		D2D1_RECT_F ColArea;
		ColArea.left = (m_rtCollision.left < ColRT.left) ? ColRT.left : m_rtCollision.left;
		ColArea.top = (m_rtCollision.top < ColRT.top) ? ColRT.top : m_rtCollision.top;
		ColArea.right = (m_rtCollision.right < ColRT.right) ? m_rtCollision.right : ColRT.right;
		ColArea.bottom = (m_rtCollision.bottom < ColRT.bottom) ? m_rtCollision.bottom : ColRT.bottom;
		if (abs(ColArea.right - ColArea.left) < abs(ColArea.bottom - ColArea.top))
		{
			if (ColSize != nullptr)
			{
				*ColSize = ColArea.right - ColArea.left;
			}
			if (abs(ColArea.left - m_rtCollision.left) < Epsilon)
			{
				return COL::LEFT;
			}
			else
			{
				return COL::RIGHT;
			}
		}
		else
		{
			if (ColSize != nullptr)
			{
				*ColSize = ColArea.bottom - ColArea.top;
			}
			if (abs(ColArea.top - m_rtCollision.top) < Epsilon)
			{
				return COL::TOP;
			}
			else
			{
				return COL::BOTTOM;
			}
		}
	}
	return COL::NONE;
}
void Plane_Object::CreateIndexBuffer(ID3D11Device* pDevice)
{
	for (int i = 0; i < m_VertexList.size() / 4; ++i)
	{
		m_indiciesList.push_back(i * 4);
		m_indiciesList.push_back(i * 4 + 1);
		m_indiciesList.push_back(i * 4 + 3);
		m_indiciesList.push_back(i * 4 + 3);
		m_indiciesList.push_back(i * 4 + 2);
		m_indiciesList.push_back(i * 4);
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

