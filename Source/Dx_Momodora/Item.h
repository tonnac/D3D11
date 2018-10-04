#pragma once
#include "d3dUtil.h"
#include "Texture.h"

class Item
{
public:
	Item(const INT& id = 0);
public:
	virtual bool Initset() = 0;
	virtual bool UseItem();
public:
	Texture *	getIcon();
	Texture *	getName();
	Texture *	getDesc();
	INT			getID() const;
	INT			getCount() const;
public:
	void		setCount(const INT& iVal);
	void		IncraseCount(const INT& iVal);
protected:
	INT			m_iCount;
	INT			m_ID;
	Texture *	m_Icon;
	Texture *	m_Name;
	Texture *	m_Desc;
};

using ItemPtr = std::shared_ptr<Item>;