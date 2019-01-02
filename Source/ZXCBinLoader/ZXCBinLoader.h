#pragma once
#include "ZXCLoader.h"

struct ZXCTexmap
{
	int SubNo = -1;
	std::wstring Filename;
};

struct ZXCMaterial
{
	std::wstring Name;
	std::wstring ClassName;

	DirectX::XMFLOAT3 Ambient;
	DirectX::XMFLOAT3 Diffuse;
	DirectX::XMFLOAT3 Specular;
	float Shininess;

	std::vector<ZXCTexmap>	 TexMap;
	std::vector<ZXCMaterial> SubMaterial;
};

class ZXCBinLoader : public ZXCLoader
{
public:
	bool LoadZXCBin(
		const std::wstring & FileName,
		std::vector<Vertex>& vertices,
		std::vector<DWORD>& indices,
		std::vector<Subset>& subsets,
		std::vector<ZXCSMaterial>& materials,
		std::vector<MeshNode>& nodes);

private:

	template<typename X>
	void ReadBinary(std::ifstream& fin, X& dest, UINT size = sizeof(X))
	{
		fin.read(reinterpret_cast<char*>(&dest), size);
	}

	//template<typename X>
	//void ReadBinary(std::ifstream& fin, X* dest, UINT size = sizeof(X))
	//{
	//	fin.read(reinterpret_cast<char*>(dest), size);
	//}

	void ReadString(std::ifstream& fin, std::wstring& str)
	{
		int size;
		ReadBinary(fin, size);
		str.resize(size);
		ReadBinary(fin, str.data(), sizeof(wchar_t) * size);
	}

};