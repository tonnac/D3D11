#include "Button.h"

Button::Button() : m_bActive(false), m_Move(false), m_fTimer(0.0f)
{}

void Button::LoadResourceView(ID3D11Device* pDevice, const std::tstring& TexFilepath)
{
	ThrowifFailed(D3DX11CreateShaderResourceViewFromFile(pDevice, TexFilepath.c_str(), nullptr, nullptr, &m_SRV, nullptr));
}

bool Button::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
					const std::string& VSFunc, const std::string& PSFunc)
{
	UI::InitSet(pDevice, Name, TexFilepath, ShaderFilepath, VSFunc, PSFunc);
	D3DXVECTOR2 TexPos = m_Object.getTexture()->getImageSize();
	SetTexPos({ 0.0f,0.0f,TexPos.x,TexPos.y });
	m_ConstantData.Util.w = 0.5f * CASTING(FLOAT, D3DX_PI);
	m_ConstantData.Color.x = 0.0f;
	return true;
}

bool Button::Frame()
{
	m_ConstantData.Color.x = g_fGameTime;
	m_ConstantData.Color.y = m_Centerpos.x;
	m_ConstantData.Color.z = m_Centerpos.y;
	//if (m_ConstantData.Color.x > 2.0f * CASTING(FLOAT, D3DX_PI))
	//{
	//	m_ConstantData.Color.x = 0.0f;
	//}
	if (m_Move == true)
	{
		m_fTimer += g_fSecPerFrame;
		if (m_fTimer >= 0.15f)
		{
			m_fTimer = 0.0f;
			m_Move = false;
			m_ConstantData.Util.w = 0.5f * CASTING(FLOAT, D3DX_PI);
		}
		m_ConstantData.Util.w += 0.5f * CASTING(FLOAT, D3DX_PI) * g_fSecPerFrame * 35.0f;
	}
	m_LengthDiff.x = (m_VertexList[1].TexPos.x - m_VertexList[0].TexPos.x) * 0.5f;
	m_LengthDiff.y = (m_VertexList[2].TexPos.y - m_VertexList[0].TexPos.y) * 0.5f;
	return UI::Frame();
}
bool Button::PreRender(ID3D11DeviceContext* pContext)
{
	UI::PreRender(pContext);
	if (m_bActive == true)
	{
		pContext->PSSetShaderResources(0, 1, &m_SRV);
	}
	return true;
}
bool Button::Release()
{
	m_SRV->Release();
	return UI::Release();
}
bool Button::getActive() const
{
	return m_bActive;
}
void Button::setActive(const bool& flag)
{
	m_bActive = flag;
}
void Button::setMove(const bool& flag)
{
	m_Move = flag;
}