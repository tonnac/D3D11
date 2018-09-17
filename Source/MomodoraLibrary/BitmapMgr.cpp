#include "BitmapMgr.h"

BitmapMgr::BitmapMgr()
{

}

BitmapMgr::~BitmapMgr()
{
	Release();
}

bool BitmapMgr::Init()
{
	return true;
}
bool BitmapMgr::Frame()
{
	return true;
}
bool BitmapMgr::Render()
{
	return true;
}
bool BitmapMgr::Release()
{
	for (auto it : m_BitmapList)
	{
		it.second->Release();
		delete it.second;
	}
	m_BitmapList.clear();
	return true;
}
T_STR BitmapMgr::LoadFile(T_STR szName, T_STR szFile)
{
	if (getBitmapPtr(szName))
	{
		return szName;
	}

	std::map<T_STR, Bitmap*>::iterator it;
	it = m_BitmapList.find(szName);
	if (it != m_BitmapList.end())
	{
		AddCache(it->first, it->second);
		return szName;
	}

	Bitmap * newBitmap = new Bitmap;
	if (!newBitmap->LoadFile(szFile)) return nullptr;

	m_BitmapList.insert(std::make_pair(szName, newBitmap));
	AddCache(szName, newBitmap);
	return szName;
}

void BitmapMgr::AddCache(T_STR szName, Bitmap* hBItmap)
{
	CacheData_ Cache_;
	Cache_.m_tName = szName;
	Cache_.m_bBitmap = hBItmap;
	if (m_CacheList.size() >= 3)
	{
		m_CacheList.pop_front();
	}
	m_CacheList.push_back(Cache_);
}
Bitmap* BitmapMgr::getBitmapPtr(T_STR szName)
{
	for (auto it : m_CacheList)
	{
		if (it.m_tName == szName)
			return it.m_bBitmap;
	}
	for (auto it2 : m_BitmapList)
	{
		if (it2.first == szName)
			return it2.second;
	}
	return nullptr;
}
