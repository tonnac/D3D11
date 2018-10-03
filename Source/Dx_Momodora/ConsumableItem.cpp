#include "ConsumableItem.h"

FlowerItem::FlowerItem() : Item(0)
{}

bool FlowerItem::Initset(ID3D11Device* pDevice)
{
	std::tstring TexFilepath = Filepath::m_Pngpath[L"Flower"];
	std::tstring Filename;
	Filename.assign(TexFilepath, TexFilepath.find_last_of('/') + 1, TexFilepath.find_last_of('.'));
	m_Icon = S_Texture.LoadTexture(pDevice, Filename, TexFilepath);
	std::tstring DescPath;
	DescPath.assign(TexFilepath, 0, TexFilepath.find_last_of('.'));
	DescPath += L"Desc.png";
	std::tstring DescName;
	DescName.assign(DescPath, DescPath.find_last_of('/') + 1, DescPath.find_last_of('.'));
	m_Desc = S_Texture.LoadTexture(pDevice, DescName, DescPath);

	std::tstring NamePath;
	NamePath.assign(TexFilepath, 0, TexFilepath.find_last_of('.'));
	NamePath += L"Name.png";
	std::tstring NameName;
	NameName.assign(NamePath, DescPath.find_last_of('/') + 1, NamePath.find_last_of('.'));
	m_Name = S_Texture.LoadTexture(pDevice, NameName, NamePath);
	return true;
}
bool FlowerItem::UseItem()
{
	return true;
}

SeedItem::SeedItem() : Item(1)
{}
bool SeedItem::Initset(ID3D11Device* pDevice)
{
	std::tstring TexFilepath = Filepath::m_Pngpath[L"Seed"];
	std::tstring Filename;
	Filename.assign(TexFilepath, TexFilepath.find_last_of('/') + 1, TexFilepath.find_last_of('.'));
	m_Icon = S_Texture.LoadTexture(pDevice, Filename, TexFilepath);
	std::tstring DescPath;
	DescPath.assign(TexFilepath, 0, TexFilepath.find_last_of('.'));
	DescPath += L"Desc.png";
	std::tstring DescName;
	DescName.assign(DescPath, DescPath.find_last_of('/') + 1, DescPath.find_last_of('.'));
	m_Desc = S_Texture.LoadTexture(pDevice, DescName, DescPath);

	std::tstring NamePath;
	NamePath.assign(TexFilepath, 0, TexFilepath.find_last_of('.'));
	NamePath += L"Name.png";
	std::tstring NameName;
	NameName.assign(NamePath, DescPath.find_last_of('/') + 1, NamePath.find_last_of('.'));
	m_Name = S_Texture.LoadTexture(pDevice, NameName, NamePath);
	return true;
}
bool SeedItem::UseItem()
{
	return true;
}