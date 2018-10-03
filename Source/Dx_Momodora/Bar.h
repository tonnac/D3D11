#pragma once
#include "UI.h"

class Bar : public UI
{
public:
	bool		Frame() override;
private:

};

class HPBar : public Bar
{
public:
	bool		Frame() override;
private:
	
};