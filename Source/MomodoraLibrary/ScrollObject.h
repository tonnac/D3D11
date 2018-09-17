#pragma once
#include "TerrainObject.h"

class ScrollObject : public TerrainObject
{
public:
	bool				Init() override;
	virtual bool		Frame() override;
	bool				Render() override;
	bool				Release() override;
	virtual	bool		MoveObject(Object*, const RECT&);
	virtual bool		Collision(Object*) override;
	void				ReverseSet(const LONG&);
};