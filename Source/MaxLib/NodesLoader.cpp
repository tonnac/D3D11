#include "BaseExporter.h"

void NodesLoader::LoadObject(std::unordered_map<std::wstring, INode*>& nodes,
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

		// InputOffsets
//		InputOffsets(x.second, offsets[objIndex]);

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
}

void NodesLoader::LoadMesh(INode* node, ZXCObject* o)
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


#ifdef SKINNED

	o->mTriangles.resize(mesh.getNumFaces());
	std::vector<BipedVertex> bv;
	LoadBipedInfo(node, bv, o->mNodeName.first);
	for (int i = 0; i < (int)o->mTriangles.size(); ++i)
	{
		LoadVertex(mesh, tm, &o->mTriangles[i], i, i0, i1, i2);

		if (bv.empty())
		{
			o->mTriangles[i].v[0].w[0] = 1.0f;
			o->mTriangles[i].v[0].i[0] = (int)o->mNodeName.second;
			o->mTriangles[i].v[0].w[3] = 1.0f;

			o->mTriangles[i].v[1].w[0] = 1.0f;
			o->mTriangles[i].v[1].i[0] = (int)o->mNodeName.second;
			o->mTriangles[i].v[1].w[3] = 1.0f;

			o->mTriangles[i].v[2].w[0] = 1.0f;
			o->mTriangles[i].v[2].i[0] = (int)o->mNodeName.second;
			o->mTriangles[i].v[2].w[3] = 1.0f;
		}
		else
		{
			if (o->mTriangles[i].v[0].VertexNum >= (int)bv.size() ||
				o->mTriangles[i].v[1].VertexNum >= (int)bv.size() ||
				o->mTriangles[i].v[2].VertexNum >= (int)bv.size())
			{
				if (o->mTriangles[i].v[0].VertexNum >= (int)bv.size())
				{
					o->mTriangles[i].v[0].w[0] = 1.0f;
					o->mTriangles[i].v[0].i[0] = (int)o->mNodeName.second;
					o->mTriangles[i].v[0].w[3] = 1.0f;
				}
				else
				{
					InputBipedes(o->mTriangles[i].v[0], bv[o->mTriangles[i].v[0].VertexNum]);
				}

				if (o->mTriangles[i].v[1].VertexNum >= (int)bv.size())
				{
					o->mTriangles[i].v[1].w[0] = 1.0f;
					o->mTriangles[i].v[1].i[0] = (int)o->mNodeName.second;
					o->mTriangles[i].v[1].w[3] = 1.0f;
				}
				else
				{
					InputBipedes(o->mTriangles[i].v[1], bv[o->mTriangles[i].v[1].VertexNum]);
				}

				if (o->mTriangles[i].v[2].VertexNum >= (int)bv.size())
				{
					o->mTriangles[i].v[2].w[0] = 1.0f;
					o->mTriangles[i].v[2].i[0] = (int)o->mNodeName.second;
					o->mTriangles[i].v[2].w[3] = 1.0f;
				}
				else
				{
					InputBipedes(o->mTriangles[i].v[2], bv[o->mTriangles[i].v[2].VertexNum]);
				}
			}
			else
			{
				InputBipedes(o->mTriangles[i].v[0], bv[o->mTriangles[i].v[0].VertexNum]);
				InputBipedes(o->mTriangles[i].v[1], bv[o->mTriangles[i].v[1].VertexNum]);
				InputBipedes(o->mTriangles[i].v[2], bv[o->mTriangles[i].v[2].VertexNum]);
			}
		}

		LoadTexCoord(mesh, tm, &o->mTriangles[i], i, i0, i1, i2);
		LoadColor(mesh, tm, &o->mTriangles[i], i, i0, i1, i2);
		LoadNormal(mesh, tm, &o->mTriangles[i], i, i0, i1, i2);
		LoadBiNormal(mesh, &o->mTriangles[i], i, i0, i1, i2);
		o->mTriangles[i].mSubMtrl = mesh.faces[i].getMatID();
		if (o->mMaterialRef == -1)
		{
			o->mTriangles[i].mSubMtrl = -2;
		}
		else if ((int)mExporter->mOutData.Materials[o->mMaterialRef].SubMaterial.size() == 0)
		{
			o->mTriangles[i].mSubMtrl = -1;
		}
	}

#else

	o->mTriangles.resize(mesh.getNumFaces());
	for (int i = 0; i < (int)o->mTriangles.size(); ++i)
	{
		LoadVertex(mesh, tm, &o->mTriangles[i], i, i0, i1, i2);
		LoadTexCoord(mesh, tm, &o->mTriangles[i], i, i0, i1, i2);
		LoadColor(mesh, tm, &o->mTriangles[i], i, i0, i1, i2);
		LoadNormal(mesh, tm, &o->mTriangles[i], i, i0, i1, i2);
		LoadBiNormal(mesh, &o->mTriangles[i], i, i0, i1, i2);
		o->mTriangles[i].mSubMtrl = mesh.faces[i].getMatID();
		if (o->mMaterialRef == -1)
		{
			o->mTriangles[i].mSubMtrl = -2;
		}
		else if ((int)mExporter->mOutData.Materials[o->mMaterialRef].SubMaterial.size() == 0)
		{
			o->mTriangles[i].mSubMtrl = -1;
		}
	}

#endif

	if (needDel)
		delete tri;
}

void NodesLoader::LoadVertex(const Mesh & mesh, const Matrix3 & tm, VertexTri * t, int i, std::uint32_t i0, std::uint32_t i1, std::uint32_t i2)
{
	Point3 v;
	if (mesh.getNumVerts() > 0)
	{
		v = mesh.verts[mesh.faces[i].v[i0]] * tm;
		MaxUtil::ConvertVector(v, t->v[0].p);
		t->v[0].VertexNum = mesh.faces[i].v[i0];

		v = mesh.verts[mesh.faces[i].v[i2]] * tm;
		MaxUtil::ConvertVector(v, t->v[1].p);
		t->v[1].VertexNum = mesh.faces[i].v[i2];

		v = mesh.verts[mesh.faces[i].v[i1]] * tm;
		MaxUtil::ConvertVector(v, t->v[2].p);
		t->v[2].VertexNum = mesh.faces[i].v[i1];
	}
}

void NodesLoader::LoadTexCoord(const Mesh & mesh, const Matrix3 & tm, VertexTri* t, int i, std::uint32_t i0, std::uint32_t i1, std::uint32_t i2)
{
	if (mesh.getNumTVerts() > 0)
	{
		t->v[0].t.x = mesh.tVerts[mesh.tvFace[i].t[i0]].x;
		t->v[0].t.y = 1.0f - mesh.tVerts[mesh.tvFace[i].t[i0]].y;
		
		t->v[1].t.x = mesh.tVerts[mesh.tvFace[i].t[i2]].x;
		t->v[1].t.y = 1.0f - mesh.tVerts[mesh.tvFace[i].t[i2]].y;
		
		t->v[2].t.x = mesh.tVerts[mesh.tvFace[i].t[i1]].x;
		t->v[2].t.y = 1.0f - mesh.tVerts[mesh.tvFace[i].t[i1]].y;
	}
}

void NodesLoader::LoadColor(const Mesh & mesh, const Matrix3 & tm, VertexTri* t, int i, std::uint32_t i0, std::uint32_t i1, std::uint32_t i2)
{
	if (mesh.getNumVertCol() > 0)
	{
		CopyMemory(&t->v[0].c, &mesh.vertCol[mesh.vcFace[i].t[i0]], sizeof(VertColor));
		CopyMemory(&t->v[1].c, &mesh.vertCol[mesh.vcFace[i].t[i2]], sizeof(VertColor));
		CopyMemory(&t->v[2].c, &mesh.vertCol[mesh.vcFace[i].t[i1]], sizeof(VertColor));
	}
}

void NodesLoader::LoadNormal(Mesh & mesh, const Matrix3 & tm, VertexTri* t, int i, std::uint32_t i0, std::uint32_t i1, std::uint32_t i2)
{
	mesh.buildNormals();

	int vert = mesh.faces[i].getVert(i0);
	Point3 vn = GetVertexNormal(mesh, i, mesh.getRVert(vert));
	MaxUtil::ConvertVector(vn, t->v[0].n);

	vert = mesh.faces[i].getVert(i2);
	vn = GetVertexNormal(mesh, i, mesh.getRVert(vert));
	MaxUtil::ConvertVector(vn, t->v[1].n);

	vert = mesh.faces[i].getVert(i1);
	vn = GetVertexNormal(mesh, i, mesh.getRVert(vert));
	MaxUtil::ConvertVector(vn, t->v[2].n);
}

void NodesLoader::LoadBiNormal(Mesh & mesh, VertexTri * t, int i, std::uint32_t i0, std::uint32_t i1, std::uint32_t i2)
{
	auto& v0 = mesh.verts[mesh.faces[i].v[i0]];
	auto& v1 = mesh.verts[mesh.faces[i].v[i2]];
	auto& v2 = mesh.verts[mesh.faces[i].v[i1]];

	auto& t0 = mesh.tVerts[mesh.tvFace[i].t[i0]];
	auto& t1 = mesh.tVerts[mesh.tvFace[i].t[i2]];
	auto& t2 = mesh.tVerts[mesh.tvFace[i].t[i1]];

	std::array<Point3, 3> vArr = { v0, v1, v2 };
	std::array<Point3, 3> tArr = { t0, t1, t2 };

	std::array<Point3, 2> Tangent;

	ComputeTangentAndBinormal(tArr.data(), vArr.data(), Tangent.data());

	Point3 Tan0;
	Point3 BiTan0;

	MaxUtil::ConvertVector(Tangent[0], Tan0);
	MaxUtil::ConvertVector(Tangent[1], BiTan0);

	t->v[0].Tangent += Tan0;
	t->v[1].Tangent += Tan0;
	t->v[2].Tangent += Tan0;

	t->v[0].Bitangent += BiTan0;
	t->v[1].Bitangent += BiTan0;
	t->v[2].Bitangent += BiTan0;
}

TriObject * NodesLoader::GetTriObject(Object* obj, TimeValue t, bool & isDelete)
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

Point3 NodesLoader::GetVertexNormal(Mesh & mesh, int faceNo, const RVertex & rv)
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

int NodesLoader::GetMaterialRef(Mtl * mtl)
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

//void NodesLoader::InputOffsets(INode * node, D3D_MATRIX & matrix)
//{
//	Matrix3 oM = Inverse(node->GetNodeTM(mExporter->mInterval.Start()));
//	MaxUtil::ConvertMatrix(oM, matrix);
//}

#ifdef SKINNED

void NodesLoader::LoadBipedInfo(INode * node, std::vector<BipedVertex>& bipedes, const std::wstring& name)
{
	Modifier * phyMod = MaxUtil::FindModifer(node, Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B));
	Modifier * skinMod = MaxUtil::FindModifer(node, SKIN_CLASSID);

	if (phyMod != nullptr)
	{
		ExportPhysiqueData(node, bipedes, phyMod, name);
	}
	else if (skinMod != nullptr)
	{
		ExportSkinData(node, bipedes, skinMod, name);
	}
}

void NodesLoader::InputBipedes(Vertex& vertex , const BipedVertex & bipedes)
{
	int i = 0;
	for (auto & x : bipedes.mWeightList)
	{
		vertex.w[i] = x.first;
		vertex.i[i] = x.second;
		++i;
	}
	vertex.w[3] = (float)i;
}

void NodesLoader::ExportPhysiqueData(INode * node, std::vector<BipedVertex>& bipedes, Modifier * phyMod, const std::wstring& name)
{
	IPhysiqueExport * phyExport = (IPhysiqueExport*)phyMod->GetInterface(I_PHYEXPORT);
	IPhyContextExport * mcExport = (IPhyContextExport *)phyExport->GetContextInterface(node);

	mcExport->ConvertToRigid(true);
	mcExport->AllowBlending(true);

	int numverts = mcExport->GetNumberVertices();

	ObjectState os = node->EvalWorldState(mExporter->mInterval.Start());

	auto& nodeindex = mExporter->mNodeIndex;

	bipedes.resize(numverts);

	for (int i = 0; i < numverts; ++i)
	{
		float totalWeight = 0.0f, wieght = 0.0f;
		std::wstring nodeName;

		IPhyVertexExport * vi = mcExport->GetVertexInterface(i);
		if (vi == nullptr)
		{
			continue;
		}

		int type = vi->GetVertexType();

		switch (type)
		{
		case RIGID_BLENDED_TYPE:
		{
			IPhyBlendedRigidVertex* rb_vtx = (IPhyBlendedRigidVertex*)vi;

			int iNode = rb_vtx->GetNumberNodes();
			bipedes[i].mNumWeight = iNode;

			if (iNode > MAXWEIGHTNUM)
			{
				std::multimap<float, int, std::greater<float>> weightList;
				bipedes[i].mNumWeight = MAXWEIGHTNUM;
				for (int j = 0; j < iNode; ++j)
				{
					INode* node0 = rb_vtx->GetNode(j);

					GetNodeName(node0, nodeName);
					if (nodeName.empty())
						nodeName = node->GetName();

					float weight = rb_vtx->GetWeight(j);

					auto pair = std::make_pair(weight, (int)nodeindex[nodeName]);
					weightList.insert(pair);
				}
				int Count = 0;
				for (bipedes[i].iter = weightList.cbegin(); Count < MAXWEIGHTNUM; ++Count, ++bipedes[i].iter)
				{
					bipedes[i].mWeightList.insert(std::make_pair(bipedes[i].iter->first, bipedes[i].iter->second));
				}
			}
			else
			{
				for (int j = 0; j < iNode; ++j)
				{
					INode* node0 = rb_vtx->GetNode(j);

					GetNodeName(node0, nodeName);
					if (nodeName.empty())
						nodeName = node->GetName();

					float weight = rb_vtx->GetWeight(j);

					auto pair = std::make_pair(weight, (int)nodeindex[nodeName]);
					bipedes[i].mWeightList.insert(pair);
				}
			}
			bipedes[i].mNodeIndex = (int)mExporter->mNodeIndex[name];
			bipedes[i].mNodename = name;
		}break;
		case RIGID_NON_BLENDED_TYPE:
		{
			IPhyRigidVertex* r_vtx = (IPhyRigidVertex*)vi;

			INode * node0 = r_vtx->GetNode();

			bipedes[i].mNumWeight = 1;

			GetNodeName(node0, nodeName);
			if (nodeName.empty())
				nodeName = node->GetName();

			bipedes[i].mWeightList.insert(std::make_pair(1.0f, (int)nodeindex[nodeName]));

			bipedes[i].mNodeIndex = (int)mExporter->mNodeIndex[name];
			bipedes[i].mNodename = name;
		}break;
		default:
			MessageBox(nullptr, L"No Biped ID.", L"Error", 0);
			break;
		}
		mcExport->ReleaseVertexInterface(vi);
	}
	phyExport->ReleaseContextInterface(mcExport);
	phyMod->ReleaseInterface(I_PHYINTERFACE, phyExport);
}

void NodesLoader::ExportSkinData(INode * node, std::vector<BipedVertex>& bipedes, Modifier * skinMod, const std::wstring& name)
{
	if (node == nullptr || skinMod == nullptr) return;

	ISkin * skin = (ISkin*)skinMod->GetInterface(I_SKIN);
	ISkinContextData * skinData = skin->GetContextInterface(node);

	auto& nodeindex = mExporter->mNodeIndex;

	std::wstring nodeName;

	if (skin != nullptr && skinData != nullptr)
	{
		int numOfPoints = skinData->GetNumPoints();

		bipedes.resize(numOfPoints);

		for (int i = 0; i < numOfPoints; ++i)
		{
			int numOfWeights = skinData->GetNumAssignedBones(i);

			bipedes[i].mNumWeight = numOfWeights;

			if (numOfWeights > MAXWEIGHTNUM)
			{
				std::multimap<float, int, std::greater<float>> weightList;
				for (int j = 0; j < numOfWeights; ++j)
				{
					int boneIndex = skinData->GetAssignedBone(i, j);
					INode * bone = skin->GetBone(boneIndex);
					if (bone == nullptr) break;

					GetNodeName(bone, nodeName);

					float boneWeight = skinData->GetBoneWeight(i, j);

					weightList.insert(std::make_pair(boneWeight, (int)nodeindex[nodeName]));
				}

				int Cnt = 0;
				for (bipedes[i].iter = weightList.cbegin(); Cnt < MAXWEIGHTNUM; ++Cnt, ++bipedes[i].iter)
				{
					bipedes[i].mWeightList.insert(std::make_pair(bipedes[i].iter->first, bipedes[i].iter->second));
				}
				bipedes[i].mNumWeight = MAXWEIGHTNUM;
			}
			else
			{
				for (int j = 0; j < numOfWeights; ++j)
				{
					int boneIndex = skinData->GetAssignedBone(i, j);
					INode * bone = skin->GetBone(boneIndex);
					if (bone == nullptr) break;

					GetNodeName(bone, nodeName);

					float boneWeight = skinData->GetBoneWeight(i, j);

					bipedes[i].mWeightList.insert(std::make_pair(boneWeight, (int)nodeindex[nodeName]));
				}
			}
			bipedes[i].mNodeIndex = (int)mExporter->mNodeIndex[name];
			bipedes[i].mNodename = name;
		}
	}
}

void NodesLoader::GetNodeName(INode * node, std::wstring & name)
{
	auto iter = std::find_if(std::begin(mExporter->mMaxObject), std::end(mExporter->mMaxObject), [&name, node](std::pair<std::wstring, INode*> rhs)
	{
		if (node == rhs.second)
		{
			name = rhs.first;
			return true;
		}
		return false;
	});

	if (iter == mExporter->mMaxObject.end())
	{
		name.clear();
	}
}

#endif

NodesLoader::NodesLoader(BaseExporter * exporter)
	: mExporter(exporter)
{
}
