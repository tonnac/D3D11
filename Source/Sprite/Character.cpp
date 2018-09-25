#include "Character.h"
#include "DirectInput.h"

Character::Character() : m_fScale(2.6f), isDebug(false), m_pShader(nullptr), m_pSprite(nullptr)
{}

void Character::SetPos(ID3D11Device * pDevice,const D3DXVECTOR2& Centerpos)
{
	ID3DBlob * pPSBlob = nullptr;
	ID3DBlob * pErrBlob = nullptr;
	ShaderifFailed(D3DX11CompileFromFile(L"VertexShader.txt", nullptr, nullptr,
		"TerrainPS", "ps_5_0", D3DCOMPILE_DEBUG, 0, nullptr, &pPSBlob, &pErrBlob, nullptr));
	ThrowifFailed(pDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pShader));
	m_Centerpos = Centerpos;

	S_Sprite.SpriteSet(L"Kaho.txt");
	m_pSprite = S_Sprite.LoadSprite(L"Kaho", L"Idle");
}
bool Character::Frame()
{
	if (S_Input.getKeyState(DIK_DELETE) == Input::KEYSTATE::KEY_PUSH)
	{
		isDebug = !isDebug;
	}

	Plane_Object::SetTexPos(m_pSprite->getSpriteVt());
	if (m_pSprite->Frame() == false)
	{
		m_pSprite->setIndex(0);
	}

	m_LengthDiff.x = (m_VertexList[1].TexPos.x - m_VertexList[0].TexPos.x) * m_fScale * 0.5f;
	m_LengthDiff.y = (m_VertexList[2].TexPos.y - m_VertexList[0].TexPos.y) * m_fScale * 0.5f;

	Plane_Object::Frame();
	return true;
}
bool Character::PostRender(ID3D11DeviceContext* pContext)
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