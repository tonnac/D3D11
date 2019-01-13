#pragma once

#include "Writer.h"

class ZXCWriter : public Writer
{
public:
	ZXCWriter(const OutputData& outData);

public:
	virtual bool Savefile()override;

};