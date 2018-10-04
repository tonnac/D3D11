#include "ConsumableItem.h"
#include "Player.h"

FlowerItem::FlowerItem() : Item(0)
{}

bool FlowerItem::Initset()
{
	std::tstring TexFilepath = Filepath::m_Pngpath[L"Flower"];
	std::tstring Filename;
	Filename.assign(TexFilepath, TexFilepath.find_last_of('/') + 1, TexFilepath.find_last_of('.'));
	m_Icon = S_Texture.LoadTexture(Filename);
	std::tstring DescPath;
	DescPath.assign(TexFilepath, 0, TexFilepath.find_last_of('.'));
	DescPath += L"Desc.png";
	std::tstring DescName;
	DescName.assign(DescPath, DescPath.find_last_of('/') + 1, DescPath.find_last_of('.'));
	m_Desc = S_Texture.LoadTexture(DescName);

	std::tstring NamePath;
	NamePath.assign(TexFilepath, 0, TexFilepath.find_last_of('.'));
	NamePath += L"Name.png";
	std::tstring NameName;
	NameName.assign(NamePath, DescPath.find_last_of('/') + 1, NamePath.find_last_of('.'));
	m_Name = S_Texture.LoadTexture(NameName);
	return true;
}
bool FlowerItem::UseItem()
{
	g_Player->setHP(-30);
	--m_iCount;
	return true;
}

SeedItem::SeedItem() : Item(1)
{}
bool SeedItem::Initset()
{
	std::tstring TexFilepath = Filepath::m_Pngpath[L"Seed"];
	std::tstring Filename;
	Filename.assign(TexFilepath, TexFilepath.find_last_of('/') + 1, TexFilepath.find_last_of('.'));
	m_Icon = S_Texture.LoadTexture(Filename);
	std::tstring DescPath;
	DescPath.assign(TexFilepath, 0, TexFilepath.find_last_of('.'));
	DescPath += L"Desc.png";
	std::tstring DescName;
	DescName.assign(DescPath, DescPath.find_last_of('/') + 1, DescPath.find_last_of('.'));
	m_Desc = S_Texture.LoadTexture(DescName);

	std::tstring NamePath;
	NamePath.assign(TexFilepath, 0, TexFilepath.find_last_of('.'));
	NamePath += L"Name.png";
	std::tstring NameName;
	NameName.assign(NamePath, DescPath.find_last_of('/') + 1, NamePath.find_last_of('.'));
	m_Name = S_Texture.LoadTexture(NameName);
	return true;
}
bool SeedItem::UseItem()
{
	g_Player->setBuff(true);
	--m_iCount;
	return true;
}