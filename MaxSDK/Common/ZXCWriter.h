#pragma once

#include "Writer.h"

class ZXCWriter : public Writer
{
public:
	ZXCWriter(const OutputData& outData);

public:
	virtual bool Savefile()override;

private:
	void SaveMaterial(std::wofstream& os);
	void SaveNodes(std::wofstream& os);
	void SaveSubset(std::wofstream& os);
	void SaveVertices(std::wofstream& os);
	void SaveIndices(std::wofstream& os);

private:
	const std::vector<OutVertex>& mVertices;
};