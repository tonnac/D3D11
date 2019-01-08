#pragma once

#include "ZXCLoader.h"

class BinaryIO
{
public:
	static void WriteString(std::ofstream& fout, const std::wstring& str)
	{
		int size = (int)str.length();
		WriteBinary(fout, &size);
		WriteBinary(fout, str.data(), sizeof(wchar_t) * size);
	}

	template<typename X>
	static void WriteBinary(std::ofstream& fout, const X& src, UINT size = sizeof(X))
	{
		fout.write(reinterpret_cast<const char*>(&src), size);
	}

	template<typename X>
	static void WriteBinary(std::ofstream& fout, X* src, UINT size = sizeof(X))
	{
		fout.write(reinterpret_cast<const char*>(src), size);
	}

	static void WriteMaterial(std::ofstream& fout, const ZXCSMaterial& material)
	{
		BinaryIO::WriteString(fout, material.Name);
		BinaryIO::WriteString(fout, material.ClassName);
		BinaryIO::WriteBinary(fout, &material.Ambient);
		BinaryIO::WriteBinary(fout, &material.Diffuse);
		BinaryIO::WriteBinary(fout, &material.Specular);
		BinaryIO::WriteBinary(fout, &material.Shininess);
		int size = (int)material.TexMap.size();
		BinaryIO::WriteBinary(fout, &size);
		auto& p = std::cbegin(material.TexMap);
		for (;p != std::cend(material.TexMap); ++p)
		{
			BinaryIO::WriteBinary(fout, p->first);
			BinaryIO::WriteString(fout, p->second);
		}
		size = (int)material.SubMaterial.size();
		BinaryIO::WriteBinary(fout, &size);
		for (int i = 0; i < size; ++i)
		{
			BinaryIO::WriteMaterial(fout, material.SubMaterial[i]);
		}
	}

	static void WriteNodes(std::ofstream& fout, const MeshNode& mesh)
	{
		BinaryIO::WriteString(fout, mesh.NodeName);
		BinaryIO::WriteString(fout, mesh.ParentName);
		BinaryIO::WriteBinary(fout, mesh.ParentIndex, sizeof(MeshNode) - (sizeof(MeshNode::NodeName) + sizeof(MeshNode::ParentName) + sizeof(MeshNode::Ritem)));
	}
};