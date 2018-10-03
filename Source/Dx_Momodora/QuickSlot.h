#pragma once
#include "d3dUtil.h"
#include "ConsumableItem.h"

class QuickSlot
{
public:
	ItemPtr getItem(const INT& index);
	void AddItem(ItemPtr item, const INT& index);
	void DelItem(const INT& index);
private:
	std::array<ItemPtr, 5> m_itemarray;
};