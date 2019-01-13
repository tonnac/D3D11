#pragma once

#include "Writer.h"

struct SkinMesh;

class SkinWriter : public Writer
{
public:
	SkinWriter(const OutputData& outData);

private:
	void SaveVertices(std::wofstream& os)override;

};

