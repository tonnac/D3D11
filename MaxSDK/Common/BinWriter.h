#pragma once
#include "Writer.h"

class BinWriter : public Writer
{
public:
	BinWriter(const std::wstring& ExporterVersion,
		const std::wstring& Filename,
		const SceneInfo& sceneinfo,
		const std::vector<ZXCMaterial>& material,
		const std::vector<OutputObject>& object,
		const std::vector<OutVertex>& vertices,
		const std::vector<std::uint32_t>& indices,
		const std::vector<Subset>& subsets);

public:
	virtual bool Savefile()override;

public:
	void SaveScene(std::ofstream& os);
	void SaveMaterial(std::ofstream& os);
	void SaveNodes(std::ofstream& os);
	void SaveSubset(std::ofstream& os);
	void SaveVertices(std::ofstream& os);
	void SaveIndices(std::ofstream& os);

private:
	const std::vector<OutVertex>& mVertices;
};