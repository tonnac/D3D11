#pragma once
#include "Object.h"
#include "Player.h"
#include "Collision.h"

class TerrainObject : public Object
{
public:
	bool				Init			() override;
	virtual bool		Frame			() override;
	bool				Render			() override;
	bool				Release			() override;
	virtual	bool		MoveObject		(Object*, const RECT&);
	virtual bool		Collision		(Object*) override;
	void				ReverseSet		(const LONG&);
	virtual bool		isPlatUP		();
};