#include "LineObject.h"


bool LineObject::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& ShaderFilepath,
	const std::string& VSFunc, const std::string& PSFunc)
{
	m_ConstantData.Color.x = 1.0f;
	m_ConstantData.Color.y = 0.0f;
	m_ConstantData.Color.z = 0.0f;
	m_ConstantData.Color.w = 1.0f;

	CreateVertexBuffer(pDevice);
	m_Object.CreateShader(pDevice, Name, ShaderFilepath, VSFunc, PSFunc);
	m_Object.CreateRasterizer(pDevice);
	CreateIndexBuffer(pDevice);
	CreateConstantBuffer(pDevice);
	Init();
	return true;
}
void LineObject::setVertex(const D2D1_RECT_F& Rt)
{
	m_VertexList[0].Pos = { Rt.left, Rt.top, 0.5f };
	m_VertexList[1].Pos = { Rt.right, Rt.top, 0.5f };
	m_VertexList[2].Pos = { Rt.left, Rt.bottom, 0.5f };
	m_VertexList[3].Pos = { Rt.right, Rt.bottom, 0.5f };
}
bool LineObject::Render(ID3D11DeviceContext* pContext)
{
	PreRender(pContext);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	return PostRender(pContext);
}