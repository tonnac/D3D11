#include "ZXCExporter.h"

void ObjectExporter::LoadObject(std::unordered_map<std::wstring, INode*>& nodes,
	std::vector<std::unique_ptr<ZXCObject>>& ObjectList,
	std::unordered_map<std::wstring, size_t>& nodeIndex)
{
	ObjectList.resize(nodeIndex.size());

	for (auto& x : nodes)
	{
		std::unique_ptr<ZXCObject> maxObj = std::make_unique<ZXCObject>();
		size_t objIndex = nodeIndex[x.first];
		maxObj->mNodeName = std::make_pair(x.first, objIndex);
		INode* parent = x.second->GetParentNode();

		maxObj->mParentName = std::make_pair(L"NONE", -1);
		if (parent != nullptr && (!parent->IsRootNode()))
		{
			std::wstring parentN;
			for (auto&k : nodes)
			{
				if (parent == k.second)
				{
					parentN = k.first;
				}
			}
			maxObj->mParentName = std::make_pair(parentN, nodeIndex[parentN]);
			std::ReplaceString(maxObj->mParentName.first);
		}

		std::ReplaceString(maxObj->mNodeName.first);

		TimeValue t = mExporter->mInterval.Start();

		Matrix3 world = x.second->GetNodeTM(t);
		MaxUtil::ConvertMatrix(world, maxObj->mWorld);
		Matrix3 painv = Inverse(parent->GetNodeTM(t));
		Matrix3 local = x.second->GetNodeTM(t) * painv;

		AffineParts ap;
		decomp_affine(local, &ap);

		MaxUtil::ConvertVector(ap.t, maxObj->InititalPos.t);
		MaxUtil::ConvertVector(ap.q, maxObj->InititalPos.q);
		MaxUtil::ConvertVector(ap.u, maxObj->InititalPos.u);
		MaxUtil::ConvertVector(ap.k, maxObj->InititalPos.k);

		LoadMesh(x.second, maxObj.get());

		ObjectState os = x.second->EvalWorldState(t);
		os.obj->SuperClassID();

		switch (os.obj->SuperClassID())
		{
		case GEOMOBJECT_CLASS_ID:
			maxObj->Type = ObjectType::MESH;
			break;
		case HELPER_CLASS_ID:
		{
			if (os.obj->ClassID() == Class_ID(BONE_CLASS_ID, 0))
				maxObj->Type = ObjectType::BONE;
			if (os.obj->ClassID() == Class_ID(DUMMY_CLASS_ID, 0))
				maxObj->Type = ObjectType::DUMMY;
			if (os.obj->ClassID() == BIPSLAVE_CONTROL_CLASS_ID ||
				os.obj->ClassID() == BIPBODY_CONTROL_CLASS_ID)
				maxObj->Type = ObjectType::BIPED;
		}
		break;
		default:
			break;
		}
		ObjectList[objIndex] = std::move(maxObj);
	}

	for (auto&x : ObjectList)
		BuildVBIB(x.get());
}

void ObjectExporter::LoadMesh(INode* node, ZXCObject* o)
{
	TimeValue t = mExporter->mInterval.Start();

	Matrix3 tm = node->GetObjTMAfterWSM(t);

	ObjectState os = node->EvalWorldState(t);
	if (os.obj == nullptr || os.obj->SuperClassID() != GEOMOBJECT_CLASS_ID)
	{
		Box3 box;
		os.obj->GetDeformBBox(t, box, &tm);
		MaxUtil::ConvertVector(box.pmin, o->mBoundingBox.pmin);
		MaxUtil::ConvertVector(box.pmax, o->mBoundingBox.pmax);
		return;
	}

	bool needDel;
	TriObject * tri = GetTriObject(os.obj, t, needDel);
	if (tri == nullptr) return;

	Mesh mesh = tri->GetMesh();
	mesh.buildBoundingBox();
	Box3 box = mesh.getBoundingBox(&tm);
	MaxUtil::ConvertVector(box.pmin, o->mBoundingBox.pmin);
	MaxUtil::ConvertVector(box.pmax, o->mBoundingBox.pmax);

	std::uint32_t i0, i1, i2;
	i0 = 0, i1 = 1, i2 = 2;
	if (MaxUtil::isTMNegative(tm))
	{
		i0 = 2, i1 = 1, i2 = 0;
	}

	o->mMaterialRef = GetMaterialRef(node->GetMtl());
	o->mTriangles.resize(mesh.getNumFaces());

	for (int i = 0; i < (int)o->mTriangles.size(); ++i)
	{
		Matrix3 Inv = Inverse(node->GetNodeTM(t));

		Point3 v;

		int vertt = mesh.getNumVerts();

		if (mesh.getNumVerts() > 0)
		{
			v = mesh.verts[mesh.faces[i].v[i0]] * tm;
			MaxUtil::ConvertVector(v, o->mTriangles[i].v[0].p);
			o->mTriangles[i].v[0].VertexNum = mesh.faces[i].v[i0];

			v = mesh.verts[mesh.faces[i].v[i2]] * tm;
			MaxUtil::ConvertVector(v, o->mTriangles[i].v[1].p);
			o->mTriangles[i].v[1].VertexNum = mesh.faces[i].v[i2];

			v = mesh.verts[mesh.faces[i].v[i1]] * tm;
			MaxUtil::ConvertVector(v, o->mTriangles[i].v[2].p);
			o->mTriangles[i].v[2].VertexNum = mesh.faces[i].v[i1];
		}
		if (mesh.getNumTVerts() > 0)
		{
			o->mTriangles[i].v[0].t.x = mesh.tVerts[mesh.tvFace[i].t[i0]].x;
			o->mTriangles[i].v[0].t.y = 1.0f - mesh.tVerts[mesh.tvFace[i].t[i0]].y;

			o->mTriangles[i].v[1].t.x = mesh.tVerts[mesh.tvFace[i].t[i2]].x;
			o->mTriangles[i].v[1].t.y = 1.0f - mesh.tVerts[mesh.tvFace[i].t[i2]].y;

			o->mTriangles[i].v[2].t.x = mesh.tVerts[mesh.tvFace[i].t[i1]].x;
			o->mTriangles[i].v[2].t.y = 1.0f - mesh.tVerts[mesh.tvFace[i].t[i1]].y;
		}
		if (mesh.getNumVertCol() > 0)
		{
			CopyMemory(&o->mTriangles[i].v[0].c, &mesh.vertCol[mesh.vcFace[i].t[i0]], sizeof(VertColor));
			CopyMemory(&o->mTriangles[i].v[1].c, &mesh.vertCol[mesh.vcFace[i].t[i2]], sizeof(VertColor));
			CopyMemory(&o->mTriangles[i].v[2].c, &mesh.vertCol[mesh.vcFace[i].t[i1]], sizeof(VertColor));
		}

		mesh.buildNormals();

		int vert = mesh.faces[i].getVert(i0);
		Point3 vn = GetVertexNormal(mesh, i, mesh.getRVert(vert));
		MaxUtil::ConvertVector(vn, o->mTriangles[i].v[0].n);

		vert = mesh.faces[i].getVert(i2);
		vn = GetVertexNormal(mesh, i, mesh.getRVert(vert));
		MaxUtil::ConvertVector(vn, o->mTriangles[i].v[1].n);

		vert = mesh.faces[i].getVert(i1);
		vn = GetVertexNormal(mesh, i, mesh.getRVert(vert));
		MaxUtil::ConvertVector(vn, o->mTriangles[i].v[2].n);

		o->mTriangles[i].mSubMtrl = mesh.faces[i].getMatID();
		if (o->mMaterialRef == -1)
		{
			o->mTriangles[i].mSubMtrl = -2;
		}
		else if ((int)mExporter->mOutputMaterial[o->mMaterialRef].SubMaterial.size() == 0)
		{
			o->mTriangles[i].mSubMtrl = -1;
		}
	}

	if (needDel)
		delete tri;
}

void ObjectExporter::BuildVBIB(ZXCObject* mesh)
{
	static int Cnt = 0;

	std::sort(mesh->mTriangles.begin(), mesh->mTriangles.end());
	for (int i = 0; i < (int)mesh->mTriangles.size(); ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			int mTlid = mesh->mTriangles[i].mSubMtrl;
			auto& vertices = mesh->mVertices[mTlid];
			auto& indices = mesh->mIndices[mTlid];
			int vNumber = [&](Vertex& rhs) -> int
			{
				for (int k = 0; k < (int)vertices.size(); ++k)
				{
					auto verts = vertices[k];
					if (verts == rhs)
					{
						return k;
					}
				}
				return -1;
			}(mesh->mTriangles[i].v[j]);

			if (vNumber == -1)
			{
				vertices.push_back(mesh->mTriangles[i].v[j]);
				indices.push_back((UINT)(vertices.size() - 1));
			}
			else
			{
				indices.push_back(vNumber);
			}
		}
	}

	for (auto & x : mesh->mVertices)
	{
		auto & k = mesh->mIndices[x.first];

		for (UINT i = 0; i < (UINT)k.size() / 3; ++i)
		{
			std::uint32_t i0 = k[i * 3 + 0];
			std::uint32_t i1 = k[i * 3 + 1];
			std::uint32_t i2 = k[i * 3 + 2];

			auto& v0 = x.second[i0].p;
			auto& v1 = x.second[i1].p;
			auto& v2 = x.second[i2].p;

			auto& t0 = x.second[i0].t;
			auto& t1 = x.second[i1].t;
			auto& t2 = x.second[i2].t;

			Point3 e0 = v1 - v0;
			Point3 e1 = v2 - v0;

			Point2 UV0 = t1 - t0;
			Point2 UV1 = t2 - t0;

			float a = UV0.x;
			float b = UV0.y;
			float c = UV1.x;
			float d = UV1.y;

			float r = 1.0f / (a * d - b * c);

			Point3 Tangent;
			Point3 Bitangent;

			if (fabsf(a * d - b * c) < Epsilon)
			{
				Tangent = Point3(1.0f, 0.0f, 0.0f);
				Bitangent = Point3(0.0f, 1.0f, 0.0f);
			}
			else
			{
				Tangent = r * (d * e0 - b * e1);
				Bitangent = r * (a * e1 - c * e0);
			}

			x.second[i0].Tangent += Tangent;
			x.second[i0].Bitangent += Bitangent;

			////////

			e0 = v0 - v1;
			e1 = v2 - v1;

			UV0 = t0 - t1;
			UV1 = t2 - t1;

			a = UV0.x;
			b = UV0.y;
			c = UV1.x;
			d = UV1.y;

			r = 1.0f / (a * d - b * c);

			if (fabsf(a * d - b * c) < Epsilon)
			{
				Tangent = Point3(1.0f, 0.0f, 0.0f);
				Bitangent = Point3(0.0f, 1.0f, 0.0f);
			}
			else
			{
				Tangent = r * (d * e0 - b * e1);
				Bitangent = r * (a * e1 - c * e0);
			}

			x.second[i1].Tangent += Tangent;
			x.second[i1].Bitangent += Bitangent;

			////////

			e0 = v0 - v2;
			e1 = v1 - v2;

			UV0 = t0 - t2;
			UV1 = t1 - t2;

			a = UV0.x;
			b = UV0.y;
			c = UV1.x;
			d = UV1.y;

			r = 1.0f / (a * d - b * c);

			if (fabsf(a * d - b * c) < Epsilon)
			{
				Tangent = Point3(1.0f, 0.0f, 0.0f);
				Bitangent = Point3(0.0f, 1.0f, 0.0f);
			}
			else
			{
				Tangent = r * (d * e0 - b * e1);
				Bitangent = r * (a * e1 - c * e0);
			}

			x.second[i2].Tangent += Tangent;
			x.second[i2].Bitangent += Bitangent;
		}
	}

	for (auto &x : mesh->mVertices)
	{
		for (auto & k : x.second)
		{
			k.Tangent = k.Tangent.Normalize();
			k.Bitangent = k.Bitangent.Normalize();

			k.Tangent = k.Tangent - DotProd(k.Tangent, k.n) * k.n;

			if (DotProd(CrossProd(k.n, k.Tangent), k.Bitangent) < 0.0f)
			{
				k.Tangent *= -1;
			}
		}
	}

	for (auto &x : mesh->mIndices)
	{
		auto& indices = x.second;
		for (int i = 0; i < (int)indices.size() / 3; ++i)
		{
			std::uint32_t i0 = indices[i * 3 + 0];
			std::uint32_t i1 = indices[i * 3 + 1];
			std::uint32_t i2 = indices[i * 3 + 2];

			for (int k = i; k >= 1; --k)
			{
				std::uint32_t n0 = indices[k * 3 + 0];
				std::uint32_t n1 = indices[k * 3 + 1];
				std::uint32_t n2 = indices[k * 3 + 2];

				std::uint32_t p0 = indices[(k - 1) * 3 + 0];
				std::uint32_t p1 = indices[(k - 1) * 3 + 1];
				std::uint32_t p2 = indices[(k - 1) * 3 + 2];

				if (n0 < p0)
				{
					std::swap(indices[(k - 1) * 3 + 0], indices[k * 3 + 0]);
					std::swap(indices[(k - 1) * 3 + 1], indices[k * 3 + 1]);
					std::swap(indices[(k - 1) * 3 + 2], indices[k * 3 + 2]);
					continue;
				}

				if (n0 == p0 && n1 < p1)
				{
					std::swap(indices[(k - 1) * 3 + 1], indices[k * 3 + 1]);
					std::swap(indices[(k - 1) * 3 + 2], indices[k * 3 + 2]);
					continue;
				}

				if (n0 == p0 && n1 == p1 && n2 < p2)
				{
					std::swap(indices[(k - 1) * 3 + 2], indices[k * 3 + 2]);
					continue;
				}
				break;
			}
		}
	}
}

TriObject * ObjectExporter::GetTriObject(Object* obj, TimeValue t, bool & isDelete)
{
	isDelete = false;
	if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
	{
		TriObject * tri = (TriObject*)obj->ConvertToType(t, Class_ID(TRIOBJ_CLASS_ID, 0));
		if (obj != tri) isDelete = true;

		return tri;
	}
	else
	{
		return nullptr;
	}
}

Point3 ObjectExporter::GetVertexNormal(Mesh & mesh, int faceNo, const RVertex & rv)
{
	Face* f = &mesh.faces[faceNo];
	DWORD smGroup = f->smGroup;
	int numNormals = rv.rFlags & NORCT_MASK;
	Point3 n;

	if (rv.rFlags & SPECIFIED_NORMAL)
	{
		n = rv.rn.getNormal();
	}
	else if (numNormals && smGroup)
	{
		if (numNormals == 1)
		{
			n = rv.rn.getNormal();
		}
		else
		{
			for (int i = 0; i < numNormals; ++i)
			{
				if (rv.ern[i].getSmGroup() & smGroup)
					n = rv.ern[i].getNormal();
			}
		}
	}
	else
	{
		n = mesh.getFaceNormal(faceNo);
	}
	return n;
}

int ObjectExporter::GetMaterialRef(Mtl * mtl)
{
	for (int i = 0; i < (int)mExporter->mMaxMaterial.size(); ++i)
	{
		if (mExporter->mMaxMaterial[i] == mtl)
		{
			return i;
		}
	}
	return -1;
}

ObjectExporter::ObjectExporter(ZXCExporter * exporter)
	: mExporter(exporter)
{
}
