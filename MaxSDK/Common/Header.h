#pragma once

#include <istdplug.h>
#include <iparamb2.h>
#include <iparamm2.h>
#include <maxtypes.h>
#include <utilapi.h>
#include <max.h>
#include <stdmat.h>
#include <decomp.h>
#include <CS\\bipexp.h>
#include <CS\\phyexp.h>
#include <iskin.h>

#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <memory>
#include <algorithm>
#include <functional>
#include <array>
#include <queue>
#include <chrono>

constexpr float Epsilon = 1.0e-3f;
constexpr int MAXWEIGHTNUM = 4;

struct D3D_MATRIX
{
	D3D_MATRIX()
	{
		m[0][0] = 1.0f, m[0][1] = 0.0f, m[0][2] = 0.0f, m[0][3] = 0.0f,
		m[1][0] = 0.0f, m[1][1] = 1.0f, m[1][2] = 0.0f, m[1][3] = 0.0f,
		m[2][0] = 0.0f, m[2][1] = 0.0f, m[2][2] = 1.0f, m[2][3] = 0.0f,
		m[3][0] = 0.0f, m[3][1] = 0.0f, m[3][2] = 0.0f, m[3][3] = 1.0f;
	};
	union {
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};
};

struct Subset
{
	int NodeIndex;
	int MtrlRef;
	int SubMtlID;

	UINT VertexStart;
	UINT VertexCount;
	UINT FaceStart;
	UINT FaceCount;
};

struct Vertex
{
	Point3 p;
	Point3 n;
	Point4 c = { 1.0f, 1.0f, 1.0f, 1.0f };
	Point2 t;
	Point3 Tangent = { 0.0f, 0.0f, 0.0f };
	Point3 Bitangent = { 0.0f, 0.0f, 0.0f };
	int VertexNum = -1;
	inline bool operator == (const Vertex& rhs)
	{
		if (p.Equals(rhs.p, Epsilon) && n.Equals(rhs.n, Epsilon) && c.Equals(rhs.c, Epsilon) && t.Equals(rhs.t, Epsilon))
		{
			return true;
		}
		return false;
	}
};

struct SkinnedVertex : public Vertex
{
	std::array<float, 4> w;
	std::array<UINT, 4> i;
	inline bool operator == (const SkinnedVertex& rhs)
	{
		return Vertex::operator==(rhs);
	}
};

struct OutVertex
{
	Point3 p = Point3(0.0f, 0.0f, 0.0f);
	Point3 n = Point3(0.0f, 0.0f, 0.0f);
	Point4 c = Point4(0.0f, 0.0f, 0.0f, 0.0f);
	Point2 t = Point2(0.0f, 0.0f);
	Point3 Tangent = Point3(0.0f, 0.0f, 0.0f);

	OutVertex() = default;

	inline OutVertex(const Vertex& rhs)
	{
		p = std::move(rhs.p);
		n = std::move(rhs.n);
		c = std::move(rhs.c);
		t = std::move(rhs.t);
		Tangent = std::move(rhs.Tangent);
	}
};

struct OutSkinned : public OutVertex
{
	std::array<float, 4> w;
	std::array<BYTE, 4> i;

	inline OutSkinned(SkinnedVertex& rhs)
	{
		p = std::move(rhs.p);
		n = std::move(rhs.n);
		c = std::move(rhs.c);
		t = std::move(rhs.t);
		Tangent = std::move(rhs.Tangent);
		std::copy(rhs.w.begin(), rhs.w.end(), w.data());
		std::copy(rhs.i.begin(), rhs.i.end(), i.data());
	}
};

struct VertexTri
{
	int mSubMtrl = -1;
	Vertex v[3];
	inline bool operator< (const VertexTri& rhs) const
	{
		return mSubMtrl < rhs.mSubMtrl;
	}
};

struct SkinnedTri
{
	int mSubMtrl = -1;
	SkinnedVertex v[3];
	inline bool operator< (const SkinnedTri& rhs) const
	{
		return mSubMtrl < rhs.mSubMtrl;
	}
};

struct SceneInfo
{
	int FirstFrame = -1;
	int LastFrame = -1;
	int FrameSpeed = -1;
	int TickperFrame = -1;
};

struct ZXCMaterial
{
	std::wstring Name;
	std::wstring ClassName;

	Color Ambient;
	Color Diffuse;
	Color Specular;
	float Shininess;

	std::map<int, std::wstring> TexMap;
	std::vector<ZXCMaterial> SubMaterial;
};

enum class ObjectType
{
	MESH,
	DUMMY,
	BONE,
	BIPED,
};

struct ZXCObject
{
	int mMaterialRef = -1;

	std::pair<std::wstring, size_t> mNodeName;
	std::pair<std::wstring, size_t> mParentName;

	ObjectType Type = ObjectType::MESH;

	Box3 mBoundingBox;
	AffineParts InititalPos;

	D3D_MATRIX mWorld;

	std::vector<VertexTri> mTriangles;
	std::vector<SkinnedTri> mSkinTri;

	std::map<int, std::vector<Vertex>> mVertices;
	std::map<int, std::vector<SkinnedVertex>> mSkinVertices;
	std::map<int, std::vector<std::uint32_t>> mIndices;
};

struct BoundingBox
{
	Point3 Center;
	Point3 Extents;
};

struct OutputObject
{
	OutputObject(const ZXCObject& rhs)
	{
		NodeName = std::move(rhs.mNodeName.first);
		ParentName = std::move(rhs.mParentName.first);
		ParentIndex = std::move((int)rhs.mParentName.second);
		Type = std::move(rhs.Type);
		World = std::move(rhs.mWorld);
		Translations = std::move(rhs.InititalPos.t);
		RotationQuat = std::move(rhs.InititalPos.q);
		ScaleQuat = std::move(rhs.InititalPos.u);
		Scale = std::move(rhs.InititalPos.k);
		box.Center = (rhs.mBoundingBox.pmax + rhs.mBoundingBox.pmin) * 0.5f;
		box.Extents = (rhs.mBoundingBox.pmax - rhs.mBoundingBox.pmin) * 0.5f;
	}

	std::wstring NodeName;
	std::wstring ParentName;
	int ParentIndex = -1;
	ObjectType Type = ObjectType::MESH;
	D3D_MATRIX World;
	Point3 Translations;
	Quat RotationQuat;
	Quat ScaleQuat;
	Point3 Scale;
	BoundingBox box;
};

struct KeyFrame
{
	KeyFrame() {}
	~KeyFrame() {}

	int Tick;
	Point3 Translations;
	Point3 Scale;
	Quat ScaleQuat;
	Quat RotationQuat;
};

struct BoneAnimation
{
	std::vector<KeyFrame> Keyframes;
};

struct AnimationClip
{
	std::vector<BoneAnimation> BoneAnimations;
};

namespace std
{
	static inline void ReplaceString(wstring& src, const wstring& findString = L" ", const wstring replaceString = L"")
	{
		size_t pos = 0;
		while ((pos = src.find(findString)) != wstring::npos)
			src.replace(pos, findString.length(), replaceString);
	}
}

class Unicodecvt : public std::codecvt<wchar_t, char, mbstate_t>
{
protected:
	virtual bool do_always_noconv() const noexcept
	{
		return true;
	}
};

class MaxUtil
{
public:
	template<typename X>
	static void ConvertVector(const X& src, X& dest)
	{
		X copysrc = src;
		copysrc.y = src.z;
		copysrc.z = src.y;
		CopyMemory(&dest, &copysrc, sizeof(X));
	}

	static void ConvertMatrix(const Matrix3& maxMatrix, D3D_MATRIX& dxMatrix)
	{
		Point3 row0, row1, row2, row3;
		row0 = maxMatrix.GetRow(0);
		row1 = maxMatrix.GetRow(1);
		row2 = maxMatrix.GetRow(2);
		row3 = maxMatrix.GetRow(3);

		dxMatrix.m[0][0] = row0.x, dxMatrix.m[0][1] = row0.z, dxMatrix.m[0][2] = row0.y;
		dxMatrix.m[1][0] = row2.x, dxMatrix.m[1][1] = row2.z, dxMatrix.m[1][2] = row2.y;
		dxMatrix.m[2][0] = row1.x, dxMatrix.m[2][1] = row1.z, dxMatrix.m[2][2] = row1.y;
		dxMatrix.m[3][0] = row3.x, dxMatrix.m[3][1] = row3.z, dxMatrix.m[3][2] = row3.y;
	}

	static bool isTMNegative(const Matrix3& m)
	{
		return (DotProd(CrossProd(m.GetRow(0), m.GetRow(1)), m.GetRow(2)) < 0.0f) ? true : false;
	}

	static Modifier* FindModifer(INode * node, Class_ID classID)
	{
		Object * object = node->GetObjectRef();
		if (object == nullptr) return nullptr;

		while (object->SuperClassID() == GEN_DERIVOB_CLASS_ID && object)
		{
			IDerivedObject* DerivedObject = static_cast<IDerivedObject*>(object);

			int modStackIndex = 0;

			while (modStackIndex < DerivedObject->NumModifiers())
			{
				Modifier * modifier = DerivedObject->GetModifier(modStackIndex);

				if (modifier->ClassID() == classID)
					return modifier;

				++modStackIndex;
			}
			object = DerivedObject->GetObjRef();
		}
		return nullptr;
	}

	static std::wstring nowtime()
	{
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		time_t nowTime = std::chrono::system_clock::to_time_t(now);

		char buf[256] = { 0, };
		wchar_t wbuf[256] = { 0, };
		ctime_s(buf, sizeof(buf), &nowTime);

		MultiByteToWideChar(CP_ACP, 0, buf, -1, wbuf, 256);

		return std::wstring(wbuf);
	}
};

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

	static void WriteMaterial(std::ofstream& fout, const ZXCMaterial& material)
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

	static void WriteNodes(std::ofstream& fout, const OutputObject& mesh)
	{
		BinaryIO::WriteString(fout, mesh.NodeName);
		BinaryIO::WriteString(fout, mesh.ParentName);
		BinaryIO::WriteBinary(fout, mesh.ParentIndex, sizeof(OutputObject) - (sizeof(OutputObject::NodeName) + sizeof(OutputObject::ParentName)));
	}
};


static inline Quat operator-(Quat& lhs)
{
	Quat temp = lhs;
	temp.x *= -1;
	temp.y *= -1;
	temp.z *= -1;
	temp.w *= -1;
	return temp;
}