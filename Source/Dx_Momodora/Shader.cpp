#include "Shader.h"
bool Shader::Release()
{
	RELEASE(m_pVSBlob);
	RELEASE(m_pVertexShader);
	RELEASE(m_pPixelShader);
	return true;
}
void Shader::LoadShader(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& Filepath,
	const std::string& VSFunc, const std::string& PSFunc)
{
	m_Name = Name;
	DWORD CompileFlag = D3DCOMPILE_DEBUG;
	ID3DBlob* pErrBlob = nullptr;
	ShaderifFailed(D3DX11CompileFromFile(Filepath.c_str(), nullptr, nullptr, VSFunc.c_str(), "vs_5_0", CompileFlag, 0, nullptr, &m_pVSBlob, &pErrBlob, nullptr));
	ThrowifFailed(pDevice->CreateVertexShader(m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), nullptr, &m_pVertexShader));

	ID3DBlob* pPSBlob = nullptr;
	ShaderifFailed(D3DX11CompileFromFile(Filepath.c_str(), nullptr, nullptr, PSFunc.c_str(), "ps_5_0", CompileFlag, 0, nullptr, &pPSBlob, &pErrBlob, nullptr));
	ThrowifFailed(pDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pPixelShader));

	RELEASE(pErrBlob);
	RELEASE(pPSBlob);
}
ID3D11PixelShader* Shader::getPixelShader() const
{
	GETPTR(m_pPixelShader);
}
ID3D11VertexShader* Shader::getVertexShader() const
{
	GETPTR(m_pVertexShader);
}
ID3DBlob* Shader::getVSBlob() const
{
	GETPTR(m_pVSBlob);
}

bool ShaderMgr::Release()
{
	for (auto& iter : m_DataMap)
	{
		iter.second->Release();
		delete iter.second;
	}
	m_DataMap.clear();
	return true;
}
void ShaderMgr::AddCache(const std::tstring& Name, Shader* pShader)
{
	CacheData_<Shader*> Cache;
	Cache.Name = Name;
	Cache.Data = pShader;
	if (m_DataCache.size() >= Max_Cache)
	{
		m_DataCache.pop_back();
	}
	m_DataCache.push_front(Cache);
}
Shader* ShaderMgr::LoadShader(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& Filepath,
	const std::string& VSFunc, const std::string& PSFunc)
{
	Shader* pData = nullptr;
	if ((pData = getShader(Name)) != nullptr)
	{
		return pData;
	}

	if (m_DataMap.find(Name) != m_DataMap.cend())
	{
		pData = m_DataMap[Name];
		AddCache(Name, m_DataMap[Name]);
		return pData;
	}

	pData = new Shader;
	pData->LoadShader(pDevice, Name, Filepath, VSFunc, PSFunc);
	m_DataMap.insert(std::make_pair(Name, pData));
	AddCache(Name, pData);
	return pData;
}
Shader* ShaderMgr::getShader(const std::tstring& Name)
{
	CacheIter iter;
	auto foo = [&Name](CacheData_<Shader*> Cache) {if (Name == Cache.Name) { return true; } return false;};
	iter = std::find_if(m_DataCache.cbegin(), m_DataCache.cend(), foo);
	if (iter != m_DataCache.cend())
	{
		return iter->Data;
	}
	return nullptr;
}