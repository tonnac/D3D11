#pragma once

#include "Writer.h"

struct SkinMesh;

class SkinWriter : public Writer
{
public:
	SkinWriter(const OutputData& outData);
public:
	bool Savefile();

private:
	void SaveMaterial(std::wofstream& os);
	void SaveNodes(std::wofstream& os);
	void SaveSubset(std::wofstream& os);
	void SaveVertices(std::wofstream& os);
	void SaveIndices(std::wofstream& os);

};

