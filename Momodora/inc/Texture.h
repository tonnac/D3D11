#pragma once
#include "d3dUtil.h"

class Texture
{
public:
	Texture();
public:
	bool						Release();
public:
	void						LoadTexture(ID3D11Device* pDevice ,const std::tstring& Name, const std::tstring& Filepath);
	void						CreateSamplerState(ID3D11Device* pDevice);
	ID3D11SamplerState*			getSamplerState();
	ID3D11ShaderResourceView*	getResourceView();
	D3DXVECTOR2					getImageSize();
private:
	D3DXVECTOR2					m_ImageSize;
	std::tstring				m_Name;
	D3D11_SAMPLER_DESC			m_SamplerDesc;
	ID3D11ShaderResourceView *	m_pTexSRV = nullptr;
	ID3D11SamplerState*			m_pSamplerState = nullptr;
};


class TextureMgr : public Singleton<TextureMgr>
{
	friend class Singleton<TextureMgr>;
	using DataMap = std::map<std::tstring, Texture*>;
	using CacheList = std::list<CacheData_<Texture*>>;
	using CacheIter = CacheList::const_iterator;
private:
	TextureMgr() = default;
public:
	bool				Release();
public:
	Texture*			LoadTexture(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& Filepath);
	Texture*			LoadTexture(const std::tstring& Name);
private:
	void				AddCache(const std::tstring& Name, Texture* pTexture);
	Texture*			getTexture(const std::tstring& Name);
private:
	const int			Max_Cache = 3;
	CacheList			m_DataCache;
	DataMap				m_DataMap;
};

#define S_Texture TextureMgr::getInst()