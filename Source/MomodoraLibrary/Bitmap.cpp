#include "Bitmap.h"



bool Bitmap::Init()
{
	return true;
}
bool Bitmap::Frame()
{
	return true;
}
bool Bitmap::Render()
{
	return true;
}
bool Bitmap::Release()
{
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
	return true;
}
bool Bitmap::LoadFile(T_STR szFile)
{
	m_hBitmap =
		static_cast<HBITMAP>(LoadImage(g_hInstance,
			szFile.c_str(),
			IMAGE_BITMAP, 0, 0,
			LR_DEFAULTSIZE | LR_LOADFROMFILE));

	if (m_hBitmap == nullptr)
	{
		return false;
	}

	GetObject(m_hBitmap, sizeof(BITMAP), &m_BitmapInfo);

	m_hMemDC = CreateCompatibleDC(g_hOffScreenDC);
	SelectObject(m_hMemDC, m_hBitmap);

	return true;
}

HBITMAP Bitmap::getHBITMAP()
{
	return m_hBitmap;
}
HDC	Bitmap::getMemDC()
{
	return m_hMemDC;
}
BITMAP Bitmap::getBitmapInfo()
{
	return m_BitmapInfo;
}

void * Bitmap::operator new(size_t sz, const char* FileName, int iLine)
{
	std::string ad = FileName;
	MEMINFO mem;
	void* pfs = new char[sz];
	mem.addr = pfs;
	mem.filename = ad.substr(ad.find_last_of('\\') + 1, ad.length() - (ad.find_last_of(".cpp") + 4));
	mem.line = iLine;
	mem.dwAllocateTime = timeGetTime();
	MemoryMap.insert(std::make_pair(pfs, mem));
	++::g_inewCount;
	return pfs;
}
void Bitmap::operator delete(void * p)
{
	std::map<void*, MEMINFO>::iterator it;
	it = MemoryMap.find(p);
	MemoryMap.erase(it);
	--::g_inewCount;
	delete p;
}