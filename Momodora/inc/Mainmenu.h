#pragma once
#include "UI.h"

class Mainmenu : public UI
{
public:
	void SetPos(const FLOAT& rtLeft, const FLOAT& rtTop, const FLOAT& rtRight, const FLOAT& rtBottom) override;
	bool Frame() override;
private:

};