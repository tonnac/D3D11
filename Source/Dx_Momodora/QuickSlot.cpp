#include "QuickSlot.h"

ItemPtr QuickSlot::getItem(const INT& index)
{
	if (m_itemarray[index] != nullptr)
	{
		return m_itemarray[index];
	}
	return nullptr;
}
void QuickSlot::AddItem(ItemPtr item, const INT& index)
{
	m_itemarray[index] = std::move(item);
}
void QuickSlot::DelItem(const INT& index)
{
	m_itemarray[index] = nullptr;
}