#pragma once
#include "CharacterObject.h"

class Player : public CharacterObject
{
public:
	Player();
	void		setHP(const INT&) override;
};