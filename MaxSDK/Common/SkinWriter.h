#pragma once

#include "Writer.h"

struct SkinMesh;

class SkinWriter : public Writer
{
public:
	SkinWriter(const std::wstring& ExporterVersion,
		const std::wstring& Filename,
		const std::vector<ZXCMaterial>& material,
		const std::vector<OutputObject>& object,
		const std::vector<OutSkinned>& vertices,
		const std::vector<std::uint32_t>& indices,
		const std::vector<Subset>& subsets);
public:
	bool Savefile();

private:
	void SaveMaterial(std::wofstream& os);
	void SaveNodes(std::wofstream& os);
	void SaveSubset(std::wofstream& os);
	void SaveVertices(std::wofstream& os);
	void SaveIndices(std::wofstream& os);

	const std::vector<OutSkinned>& mVertices;
};

