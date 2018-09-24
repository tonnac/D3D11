#include "Character.h"
#include "DirectInput.h"

Character::Character() : m_fScale(2.6f), isDebug(false), m_pShader(nullptr), m_pSprite(nullptr)
{}

void Character::SetPos(ID3D11Device * pDevice,const D3DXVECTOR2& Centerpos, const D3DXVECTOR4& DrawVec)
{
	ID3DBlob * pPSBlob = nullptr;
	ID3DBlob * pErrBlob = nullptr;
	ShaderifFailed(D3DX11CompileFromFile(L"VertexShader.txt", nullptr, nullptr,
		"TerrainPS", "ps_5_0", D3DCOMPILE_DEBUG, 0, nullptr, &pPSBlob, &pErrBlob, nullptr));
	ThrowifFailed(pDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pShader));
	Plane_Object::SetPos(Centerpos, DrawVec);

	S_Sprite.SpriteSet(L"Kaho.txt");
	m_pSprite = S_Sprite.LoadSprite(L"Kaho", L"Idle");
}
bool Character::Frame()
{
	if (S_Input.getKeyState(DIK_DELETE) == Input::KEYSTATE::KEY_PUSH)
	{
		isDebug = !isDebug;
	}
	D3DXVECTOR2 Size = 	m_Object.getTexture()->getImageSize();

	FLOAT fWidth = (m_VertexList[1].TexPos.x - m_VertexList[0].TexPos.x) * m_fScale;
	FLOAT fHeight = (m_VertexList[2].TexPos.y - m_VertexList[0].TexPos.y) * m_fScale;

	m_rtCollision.left = m_Centerpos.x - fWidth * 0.5f;
	m_rtCollision.top = m_Centerpos.y - fHeight * 0.5f;
	m_rtCollision.right = m_Centerpos.x + fWidth * 0.5f;
	m_rtCollision.bottom = m_Centerpos.y + fHeight * 0.5f;

	m_VertexList[0].Pos.x = m_rtCollision.left;
	m_VertexList[0].Pos.y = m_rtCollision.top;
	m_VertexList[1].Pos.x = m_rtCollision.right;
	m_VertexList[1].Pos.y = m_VertexList[0].Pos.y;
	m_VertexList[2].Pos.x = m_VertexList[0].Pos.x;
	m_VertexList[2].Pos.y = m_rtCollision.bottom;
	m_VertexList[3].Pos.x = m_VertexList[1].Pos.x;
	m_VertexList[3].Pos.y = m_VertexList[2].Pos.y;

	Plane_Object::SetPos(m_pSprite->getSpriteVt());
	if (m_pSprite->Frame() == false)
	{
		m_pSprite->setIndex(0);
	}
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