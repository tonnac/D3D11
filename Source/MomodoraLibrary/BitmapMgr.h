#pragma once
#include "Bitmap.h"

struct CacheData_
{
	T_STR	m_tName;
	Bitmap* m_bBitmap;
};


class BitmapMgr : public Singleton<BitmapMgr>
{
	friend class Singleton<BitmapMgr>;
public:
	BitmapMgr();
	~BitmapMgr();
public:
	bool						Init();
	bool						Frame();
	bool						Render();
	bool						Release();
public:
	T_STR						LoadFile(T_STR, T_STR);
	Bitmap*						getBitmapPtr(T_STR);
private:
	void						AddCache(T_STR, Bitmap*);
private:
	std::map<T_STR, Bitmap*>	m_BitmapList;
	std::list<CacheData_>		m_CacheList;
};

#define S_BitmapMgr	BitmapMgr::getInstance()