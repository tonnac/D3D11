#include "Plane_Object.h"
#include "DirectInput.h"

Plane_Object::Plane_Object()
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

