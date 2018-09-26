#pragma once
#include "d3dUtil.h"

class Shader
{
public:
	Shader() = default;
public:
	bool						Release();
public:
	void						LoadShader(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& Filepath,
								const std::string& VSFunc, const std::string& PSFunc);
	ID3D11PixelShader*			getPixelShader() const;
	ID3D11VertexShader*			getVertexShader() const;
	ID3DBlob*					getVSBlob() const;
private:
	std::tstring				m_Name;
	ID3DBlob*					m_pVSBlob = nullptr;
	ID3D11VertexShader *		m_pVertexShader = nullptr;
	ID3D11PixelShader*			m_pPixelShader = nullptr;

};


class ShaderMgr : public Singleton<ShaderMgr>
{
	friend class Singleton<ShaderMgr>;
	using DataMap = std::map<std::tstring, Shader*>;
	using CacheList = std::list<CacheData_<Shader*>>;
	using CacheIter = CacheList::const_iterator;
private:
	ShaderMgr() = default;
public:
	bool				Release();
public:
	Shader*				LoadShader(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& Filepath,
						const std::string& VSFunc, const std::string& PSFunc);
	Shader *			getShader(const std::tstring& Name);
private:
	void				AddCache(const std::tstring& Name, Shader* pShader);
private:
	const int			Max_Cache = 3;
	CacheList			m_DataCache;
	DataMap				m_DataMap;
};

#define S_Shader ShaderMgr::getInst()