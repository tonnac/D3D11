#include "Character.h"
#include "DirectInput.h"
#include "State.h"

#include "DirectWrite.h"

Character::Character() : m_pShader(nullptr), m_pCurrentState(nullptr), m_iDir(1), m_fSpeed(0.0f),
							m_bLanding(true)
{
	m_fScale = 2.8f;
}
Character::~Character()
{
	Release();
}

bool Character::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
						const std::string& VSFunc, const std::string& PSFunc)
{
	m_pShader = S_Shader.LoadShader(pDevice, L"Terrain", Filepath::m_Txtpath[L"Shader"], "VS", "TerrainPS")->getPixelShader();
	m_ConstantData.Color.x = 0.0f;
	m_ConstantData.Color.y = 0.0f;
	m_ConstantData.Color.z = 1.0f;
	m_ConstantData.Color.w = 1.0f;
	return Plane_Object::InitSet(pDevice, Name, TexFilepath, ShaderFilepath, VSFunc, PSFunc);
}
bool Character::Init()
{
	for (auto& it : m_StateList)
	{
		it.second->Init();
	}
	return true;
}
bool Character::Frame()
{
	m_pCurrentState->Frame();

	m_LengthDiff.x = (m_VertexList[1].TexPos.x - m_VertexList[0].TexPos.x) * m_fScale * 0.5f;
	m_LengthDiff.y = (m_VertexList[2].TexPos.y - m_VertexList[0].TexPos.y) * m_fScale * 0.5f;

	if (m_iDir == -1)
	{
		reverseSet();
	}

	return Plane_Object::Frame();
}
bool Character::PostRender(ID3D11DeviceContext* pContext)
{
	Plane_Object::PostRender(pContext);

	//std::tstring pos = std::to_tstring(m_Centerpos.x) + L", " + std::to_tstring(m_Centerpos.y);
	//S_Write.DrawText({ 0.0f,50.0f,CASTING(FLOAT,g_rtClient.right), CASTING(FLOAT,g_rtClient.bottom) }, pos, D2D1::ColorF::White);

	if (g_DebugMode == true)
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
void Character::AddState(const std::tstring& statename, State* pState)
{
	m_StateList.insert(std::make_pair(statename, pState));
}
void Character::setState(const std::tstring& statename)
{
	m_pCurrentState = m_StateList[statename];
}
void Character::reverseDir()
{
	m_iDir *= -1;
}
bool Character::isLanding() const
{
	return m_bLanding;
}
void Character::setLanding(const bool& landing)
{
	m_bLanding = landing;
}
std::tstring Character::getCurrentState()
{
	std::map<std::tstring, State*>::iterator iter;
	auto foo = [this](std::pair<std::tstring, State*> search) { if (m_pCurrentState == search.second) { return true; } return false;};
	iter = std::find_if(m_StateList.begin(), m_StateList.end(), foo);
	return iter->first;
}
INT Character::getDir() const
{
	return m_iDir;
}
void Character::reverseSet()
{
	D3DXVECTOR2 temp = m_VertexList[0].TexPos;
	m_VertexList[0].TexPos = m_VertexList[1].TexPos;
	m_VertexList[1].TexPos = temp;
	temp = m_VertexList[3].TexPos;
	m_VertexList[3].TexPos = m_VertexList[2].TexPos;
	m_VertexList[2].TexPos = temp;
}