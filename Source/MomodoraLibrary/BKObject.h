#pragma once
#include "CharacterObject.h"
#include "UI.h"

class TerrainObject;
class PlatObject;

class BKObject : public Object
{
public:
	BKObject();
public:
//	bool		Init			() override;
	bool		Frame			() override;
	bool		Render			() override;
	bool		Release			() override;
	bool		MoveScrollBk	(const LONG&) override;
	void		AddTerrain		(TerrainObject *);
	void		AddPlat			(PlatObject *);
	void		AddUI			(UI* ui);
	bool		Collision		(Object* ) override;
	void		ReverseSet		();
	void		PlatUp			();
	bool		isPlatUp		();
private:
	std::vector<TerrainObject*> m_pObjList;
	std::vector<PlatObject*>	m_pPlatList;
	std::vector<UI*>			m_pUIList;
};