#pragma once
#include "Object.h"

class NPC : public Object
{
public:
	NPC();
	~NPC();
public:
	bool		Frame() override;
private:
	int			m_iDirection[2];
};