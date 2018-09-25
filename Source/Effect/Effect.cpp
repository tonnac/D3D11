#include "Effect.h"
#include "DirectInput.h"

Effect::Effect() : isDebug(false)
{
	m_fScale = 2.6f;
}

void Effect::SetPos(ID3D11Device * pDevice, const D3DXVECTOR2& Centerpos)
{
	ID3DBlob * pPSBlob = nullptr;
	ID3DBlob * pErrBlob = nullptr;
	ShaderifFailed(D3DX11CompileFromFile(Filepath::m_Txtpath[L"Shader"].c_str(), nullptr, nullptr,
		"TerrainPS", "ps_5_0", D3DCOMPILE_DEBUG, 0, nullptr, &pPSBlob, &pErrBlob, nullptr));
	ThrowifFailed(pDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pShader));
	m_Centerpos = Centerpos;
}
bool Effect::Frame()
{
	if (S_Input.getKeyState(DIK_DELETE) == Input::KEYSTATE::KEY_PUSH)
	{
		isDebug = !isDebug;
	}

	SetTexPos(m_pEffectSprite->getSpriteVt());
	if (!m_pEffectSprite->Frame())
	{
		m_pEffectSprite->setIndex(0);
		return false;
	}

	m_LengthDiff.x = (m_VertexList[1].TexPos.x - m_VertexList[0].TexPos.x) * m_fScale * 0.5f;
	m_LengthDiff.y = (m_VertexList[2].TexPos.y - m_VertexList[0].TexPos.y) * m_fScale * 0.5f;

	return Plane_Object::Frame();
}
bool Effect::PostRender(ID3D11DeviceContext* pContext)
{
	Plane_Object::PostRender(pContext);
	if (isDebug == true)
	{
		pContext->PSSetShader(m_pShader, nullptr, 0);
		pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
		pContext->DrawIndexed(CASTING(UINT, m_indiciesList.size()), 0, 0);
	}
	return true;
}
void Effect::setSpeed(const FLOAT& pt)
{
//	m_fSpeed = pt;
}
void Effect::setIndex(const int& index)
{
	m_pEffectSprite->setIndex(index);
}