#include "Fade.h"

Fade::Fade() : m_fDivideTime(0.45f), m_fMaxAngle(0.5* CASTING(FLOAT, D3DX_PI))
{}

bool Fade::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& ShaderFilepath,
					const std::string& VSFunc, const std::string& PSFunc)
{
	m_VertexList[0].Pos = { 0.0f,0.0f,0.5f };
	m_VertexList[1].Pos = { 960.0f,0.0f,0.5f };
	m_VertexList[2].Pos = { 0.0f,720.0f,0.5f };
	m_VertexList[3].Pos = { 960.0f,720.0f,0.5f };

	CreateVertexBuffer(pDevice);
	m_Object.CreateShader(pDevice, Name, ShaderFilepath, VSFunc, PSFunc);
	m_Object.CreateRasterizer(pDevice);
	CreateIndexBuffer(pDevice);
	CreateConstantBuffer(pDevice);
	return true;
}
bool Fade::Frame()
{
	if (isOn == true)
	{
		if (abs(m_fInitVal - 0.0f) < 0.01f)
		{
			if (m_fAngle - m_fMaxAngle / 0.5f * g_fSecPerFrame * m_fDivideTime < 0.0f)
			{
				m_fAngle = 0.0f;
			}
			else
			{
				m_fAngle -= m_fMaxAngle / 0.5f * g_fSecPerFrame * m_fDivideTime;
			}
			if (abs(m_fAngle - 0.0f) < 0.01f)
			{
				m_fInitVal = m_fMaxAngle;
				isOn = false;
				return false;
			}
		}
		else
		{
			if (m_fAngle + m_fMaxAngle / 0.5f * g_fSecPerFrame * m_fDivideTime > m_fMaxAngle)
			{
				m_fAngle = m_fMaxAngle;
			}
			else
			{
				m_fAngle += m_fMaxAngle / 0.5f * g_fSecPerFrame * m_fDivideTime;
			}
			if (abs(m_fAngle - m_fMaxAngle) < 0.01f)
			{
				m_fInitVal = 0.0f;
				isOn = false;
				return false;
			}
		}
	}
	m_ConstantData.Util.z = m_fAngle;
	return true;
}
void Fade::setOn(const bool& Switch)
{
	isOn = Switch;
}
bool Fade::getOn()
{
	return isOn;
}
void Fade::FadeIn()
{
	m_fInitVal = 0.5f * CASTING(FLOAT, D3DX_PI);
	m_fAngle = 0.0f;
	isOn = true;
}
void Fade::FadeOut()
{
	m_fInitVal = 0.0f;
	m_fAngle = 0.5f * CASTING(FLOAT, D3DX_PI);
	isOn = true;
}
void Fade::setDivideTime(const FLOAT& time)
{
	m_fDivideTime = 1.0f / time;
}