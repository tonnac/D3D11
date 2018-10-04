#include "Misc.h"
#include "Player.h"

bool Misc::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
	const std::string& VSFunc, const std::string& PSFunc)
{
	bFlag = false;
	m_fScale = 3.4f;
	m_ConstantData.Color.x = 0.0f;
	m_ConstantData.Color.y = 0.0f;
	m_ConstantData.Color.z = 1.0f;
	m_ConstantData.Color.w = 1.0f;
	return Plane_Object::InitSet(pDevice, Name, TexFilepath, ShaderFilepath, VSFunc, PSFunc);
}
bool Misc::Frame()
{
	m_ConstantData.Color.x += 2.0f * CASTING(FLOAT, D3DX_PI) * g_fSecPerFrame * 0.5f;
	m_LengthDiff.x = (m_VertexList[1].TexPos.x - m_VertexList[0].TexPos.x) * m_fScale * 0.5f;
	m_LengthDiff.y = (m_VertexList[2].TexPos.y - m_VertexList[0].TexPos.y) * m_fScale * 0.5f;

	m_ConstantData.Util.z = m_Centerpos.x;
	m_ConstantData.Util.w = m_Centerpos.y;

	return Plane_Object::Frame();
}
bool Misc::PostRender(ID3D11DeviceContext* pContext)
{
	return Plane_Object::PostRender(pContext);
}
void Misc::setColorX(const FLOAT& fVal)
{
	m_ConstantData.Color.x = fVal;
}
void Misc::setFlag(const bool& flag)
{
	bFlag = flag;
}