#pragma once

#include "Writer.h"

class ZXCWriter : public Writer
{
public:
	ZXCWriter(const std::wstring& ExporterVersion,
		const std::wstring& Filename,
		const std::vector<ZXCMaterial>& material,
		const std::vector<OutputObject>& object,
		const std::vector<OutVertex>& vertices,
		const std::vector<std::uint32_t>& indices,
		const std::vector<Subset>& subsets);

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