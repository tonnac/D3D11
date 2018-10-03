#pragma once
#include "Item.h"

class FlowerItem : public Item
{
public:
	FlowerItem();
public:
	bool Initset(ID3D11Device* pDevice)override;
public:
	bool UseItem() override;
};

class SeedItem : public Item
{
public:
	SeedItem();
public:
	bool Initset(ID3D11Device* pDevice)override;
public:
	bool UseItem() override;
};