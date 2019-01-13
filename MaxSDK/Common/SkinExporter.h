#pragma once

#include "BaseExporter.h"

class SkinExporter : public BaseExporter
{
private:
	virtual void LoadObjects()override;
	virtual void CreateWriter()override;
	virtual void BuildSubset()override;
	virtual void BuildVBIB()override;
	virtual void BuildBoundingBox()override;
};