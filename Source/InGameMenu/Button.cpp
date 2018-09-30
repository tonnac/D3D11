#include "Button.h"

Button::Button() : m_bActive(false)
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
	return true;
}

bool Button::Frame()
{
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

bool Button::getActive() const
{
	return m_bActive;
}
void Button::setActive(const bool& flag)
{
	m_bActive = flag;
}