#include "Character.h"
#include "DirectInput.h"
#include "State.h"

Character::Character() : m_fScale(2.6f), isDebug(false), m_pShader(nullptr), m_pCurrentState(nullptr)
{}

void Character::SetPos(ID3D11Device * pDevice,const D3DXVECTOR2& Centerpos)
{
	ID3DBlob * pPSBlob = nullptr;
	ID3DBlob * pErrBlob = nullptr;
	ShaderifFailed(D3DX11CompileFromFile(L"VertexShader.txt", nullptr, nullptr,
		"TerrainPS", "ps_5_0", D3DCOMPILE_DEBUG, 0, nullptr, &pPSBlob, &pErrBlob, nullptr));
	ThrowifFailed(pDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pShader));
	m_Centerpos = Centerpos;
}
bool Character::Init()
{
	State* pState = new State(this);
	pState->setSprite(L"Kaho", L"Idle");
	m_pCurrentState = pState;
	m_StateList.insert(std::make_pair(std::tstring(L"PE"),pState));
	return true;
}
bool Character::Frame()
{
	if (S_Input.getKeyState(DIK_DELETE) == Input::KEYSTATE::KEY_PUSH)
	{
		isDebug = !isDebug;
	}

	m_pCurrentState->Frame();

	m_LengthDiff.x = (m_VertexList[1].TexPos.x - m_VertexList[0].TexPos.x) * m_fScale * 0.5f;
	m_LengthDiff.y = (m_VertexList[2].TexPos.y - m_VertexList[0].TexPos.y) * m_fScale * 0.5f;

	return Plane_Object::Frame();
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
bool Character::Release()
{
	for (auto& it : m_StateList)
	{
		delete it.second;
	}
	return Plane_Object::Release();
}