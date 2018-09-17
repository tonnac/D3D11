#pragma once
#include "EffectObj.h"


class KahoAttack1 : public EffectObj
{
public:
	bool		Init() override;
	bool		Frame() override;
};

class KahoAttack2 : public EffectObj
{
public:
	bool		Init() override;
	bool		Frame() override;
};

class KahoAttack3 : public EffectObj
{
public:
	bool		Init() override;
	bool		Frame() override;
};

class KahoAirAttack : public EffectObj
{
public:
	bool		Init() override;
	bool		Frame() override;
};

class KahoBowAttack : public EffectObj
{
public:
	bool		Init() override;
	bool		Frame() override;
};