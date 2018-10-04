#include "Item.h"

Item::Item(const INT& id) : m_ID(id), m_iCount(1)
{}

bool Item::UseItem()
{
	return true;
}
Texture * Item::getIcon()
{
	GETPTR(m_Icon);
}
Texture * Item::getName()
{
	GETPTR(m_Name);
}
Texture * Item::getDesc()
{
	GETPTR(m_Desc);
}
INT	Item::getID() const
{
	return m_ID;
}
INT	Item::getCount() const
{
	return m_iCount;
}
void Item::setCount(const INT& iVal)
{
	m_iCount = iVal;
}
void Item::IncraseCount(const INT& iVal)
{
	m_iCount += iVal;
}