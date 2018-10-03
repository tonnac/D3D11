#include "Texture.h"

Texture::Texture()
{
	ZeroMemory(&m_SamplerDesc, sizeof(D3D11_SAMPLER_DESC));
}
bool Texture::Release()
{
	RELEASE(m_pTexSRV);
	RELEASE(m_pSamplerState);
	return true;
}
void Texture::LoadTexture(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& Filepath)
{
	m_Name = Name;
	D3DX11_IMAGE_INFO Imageinfo;
	D3DX11GetImageInfoFromFile(Filepath.c_str(), nullptr, &Imageinfo, nullptr);
	m_ImageSize.x = CASTING(float, Imageinfo.Width);
	m_ImageSize.y = CASTING(float, Imageinfo.Height);
	ThrowifFailed(D3DX11CreateShaderResourceViewFromFile(pDevice, Filepath.c_str(), nullptr, nullptr, &m_pTexSRV, nullptr));
	

	CreateSamplerState(pDevice);
}
void Texture::CreateSamplerState(ID3D11Device* pDevice)
{
	m_SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	m_SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	m_SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	m_SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	ThrowifFailed(pDevice->CreateSamplerState(&m_SamplerDesc, &m_pSamplerState));
}
ID3D11SamplerState* Texture::getSamplerState()
{
	GETPTR(m_pSamplerState);
}
ID3D11ShaderResourceView* Texture::getResourceView()
{
	GETPTR(m_pTexSRV);
}
D3DXVECTOR2 Texture::getImageSize()
{
	return m_ImageSize;
}
bool TextureMgr::Release()
{
	for (auto& iter : m_DataMap)
	{
		iter.second->Release();
		delete iter.second;
	}
	m_DataMap.clear();
	return true;
}
void TextureMgr::AddCache(const std::tstring& Name, Texture* pTexture)
{
	CacheData_<Texture*> Cache;
	Cache.Name = Name;
	Cache.Data = pTexture;
	if (m_DataCache.size() >= Max_Cache)
	{
		m_DataCache.pop_back();
	}
	m_DataCache.push_front(Cache);
}
Texture* TextureMgr::LoadTexture(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& Filepath)
{
	Texture* pData = nullptr;
	if ((pData = getTexture(Name)) != nullptr)
	{
		return pData;
	}

	if (m_DataMap.find(Name) != m_DataMap.cend())
	{
		pData = m_DataMap[Name];
		AddCache(Name, m_DataMap[Name]);
		return pData;
	}

	pData = new Texture;
	pData->LoadTexture(pDevice, Name, Filepath);
	m_DataMap.insert(std::make_pair(Name, pData));
	AddCache(Name, pData);
	return pData;
}
Texture* TextureMgr::LoadTexture(const std::tstring& Name)
{
	Texture* pData = nullptr;
	if ((pData = getTexture(Name)) != nullptr)
	{
		return pData;
	}
	else
	{
		pData = m_DataMap[Name];
		AddCache(Name, m_DataMap[Name]);
		return pData;
	}
}
Texture* TextureMgr::getTexture(const std::tstring& Name)
{
	CacheIter iter;
	auto foo = [&Name](CacheData_<Texture*> Cache) {if (Name == Cache.Name) { return true; } return false;};
	iter = std::find_if(m_DataCache.cbegin(), m_DataCache.cend(), foo);
	if (iter != m_DataCache.cend())
	{
		return iter->Data;
	}
	return nullptr;
}