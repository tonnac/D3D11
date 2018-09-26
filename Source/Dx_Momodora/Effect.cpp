#include "Effect.h"
#include "DirectInput.h"

Effect::Effect() : isDebug(false), m_iDir(1)
{
	m_fScale = 2.6f;
}
bool Effect::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
	const std::string& VSFunc, const std::string& PSFunc)
{
	m_pShader = S_Shader.getShader(L"Terrain")->getPixelShader();
	return Plane_Object::InitSet(pDevice, Name, TexFilepath, ShaderFilepath, VSFunc, PSFunc);
}
bool Effect::Frame()
{
	SetTexPos(m_pEffectSprite->getSpriteVt());
	if (!m_pEffectSprite->Frame())
	{
		m_pEffectSprite->setIndex(0);
		return false;
	}

	m_LengthDiff.x = (m_VertexList[1].TexPos.x - m_VertexList[0].TexPos.x) * m_fScale * 0.5f;
	m_LengthDiff.y = (m_VertexList[2].TexPos.y - m_VertexList[0].TexPos.y) * m_fScale * 0.5f;

	if (m_iDir == -1)
	{
		reverseSet();
	}

	return Plane_Object::Frame();
}
bool Effect::PostRender(ID3D11DeviceContext* pContext)
{
	Plane_Object::PostRender(pContext);
	if (g_DebugMode == true)
	{
		pContext->PSSetShader(m_pShader, nullptr, 0);
		pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
		pContext->DrawIndexed(CASTING(UINT, m_indiciesList.size()), 0, 0);
	}
	return true;
}
bool Effect::Release()
{
	return Plane_Object::Release();
}
void Effect::setSpeed(const FLOAT& pt)
{
//	m_fSpeed = pt;
}
void Effect::setIndex(const int& index)
{
	m_pEffectSprite->setIndex(index);
}
void Effect::reverseDir()
{
	m_iDir *= -1;
}
void Effect::reverseSet()
{
	D3DXVECTOR2 temp = m_VertexList[0].TexPos;
	m_VertexList[0].TexPos = m_VertexList[1].TexPos;
	m_VertexList[1].TexPos = temp;
	temp = m_VertexList[3].TexPos;
	m_VertexList[3].TexPos = m_VertexList[2].TexPos;
	m_VertexList[2].TexPos = temp;
}