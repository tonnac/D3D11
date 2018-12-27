#include "ZXCSExporter.h"

void ObjectExporter::LoadObject(std::unordered_map<std::wstring, INode*>& nodes,
	std::vector<std::unique_ptr<ZXCSObject>>& ObjectList,
	std::unordered_map<std::wstring, size_t>& nodeIndex)
{
	ObjectList.resize(nodeIndex.size());

	for (auto& x : nodes)
	{
		std::unique_ptr<ZXCSObject> maxObj = std::make_unique<ZXCSObject>();
		size_t objIndex = nodeIndex[x.first];
		maxObj->mNodeName = std::make_pair(x.first, objIndex);
		INode* parent = x.second->GetParentNode();

		maxObj->mParentName = std::make_pair(L"NONE", -1);
		if (parent != nullptr && (!parent->IsRootNode()))
		{
			std::wstring parentN;
			for (auto&x : nodes)
			{
				if (parent == x.second)
				{
					parentN = x.first;
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

void ObjectExporter::LoadMesh(INode* node, ZXCSObject* o)
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

	std::vector<BipedVertex> bv;
	LoadBipedInfo(node, bv, o->mNodeName.first);

	for (int i = 0; i < (int)o->mTriangles.size(); ++i)
	{
		Matrix3 Inv = Inverse(node->GetNodeTM(t));
		
		Point3 v;

		int vertt = mesh.getNumVerts();

		if (mesh.getNumVerts() > 0)
		{
			v = mesh.verts[mesh.faces[i].v[i0]] * tm;
			MaxUtil::ConvertVector(v, o->mTriangles[i].v[0].p);
			o->mTriangles[i].v[0].VertexWNum = mesh.faces[i].v[i0];

			v = mesh.verts[mesh.faces[i].v[i2]] * tm;
			MaxUtil::ConvertVector(v, o->mTriangles[i].v[1].p);
			o->mTriangles[i].v[1].VertexWNum = mesh.faces[i].v[i2];

			v = mesh.verts[mesh.faces[i].v[i1]] * tm;
			MaxUtil::ConvertVector(v, o->mTriangles[i].v[2].p);
			o->mTriangles[i].v[2].VertexWNum = mesh.faces[i].v[i1];

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
				if (o->mTriangles[i].v[0].VertexWNum >= (int)bv.size() || 
					o->mTriangles[i].v[1].VertexWNum >= (int)bv.size() ||
					o->mTriangles[i].v[2].VertexWNum >= (int)bv.size())
				{
					if (o->mTriangles[i].v[0].VertexWNum >= (int)bv.size())
					{
						o->mTriangles[i].v[0].w[0] = 1.0f;
						o->mTriangles[i].v[0].i[0] = (int)o->mNodeName.second;
						o->mTriangles[i].v[0].w[3] = 1.0f;
					}
					else
					{
						InputBipedes(o->mTriangles[i].v[0], bv[o->mTriangles[i].v[0].VertexWNum]);
					}

					if (o->mTriangles[i].v[1].VertexWNum >= (int)bv.size())
					{
						o->mTriangles[i].v[1].w[0] = 1.0f;
						o->mTriangles[i].v[1].i[0] = (int)o->mNodeName.second;
						o->mTriangles[i].v[1].w[3] = 1.0f;
					}
					else
					{
						InputBipedes(o->mTriangles[i].v[1], bv[o->mTriangles[i].v[1].VertexWNum]);
					}

					if (o->mTriangles[i].v[2].VertexWNum >= (int)bv.size())
					{
						o->mTriangles[i].v[2].w[0] = 1.0f;
 						o->mTriangles[i].v[2].i[0] = (int)o->mNodeName.second;
						o->mTriangles[i].v[2].w[3] = 1.0f;
					}
					else
					{
						InputBipedes(o->mTriangles[i].v[2], bv[o->mTriangles[i].v[2].VertexWNum]);
					}
				}
				else
				{
					InputBipedes(o->mTriangles[i].v[0], bv[o->mTriangles[i].v[0].VertexWNum]);
					InputBipedes(o->mTriangles[i].v[1], bv[o->mTriangles[i].v[1].VertexWNum]);
					InputBipedes(o->mTriangles[i].v[2], bv[o->mTriangles[i].v[2].VertexWNum]);
				}
			}

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
		Point3 vn = GetVertexWNormal(mesh, i, mesh.getRVert(vert));
		MaxUtil::ConvertVector(vn, o->mTriangles[i].v[0].n);

		vert = mesh.faces[i].getVert(i2);
		vn = GetVertexWNormal(mesh, i, mesh.getRVert(vert));
		MaxUtil::ConvertVector(vn, o->mTriangles[i].v[1].n);

		vert = mesh.faces[i].getVert(i1);
		vn = GetVertexWNormal(mesh, i, mesh.getRVert(vert));
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

void ObjectExporter::BuildVBIB(ZXCSObject* mesh)
{
	std::sort(mesh->mTriangles.begin(), mesh->mTriangles.end());
	for (int i = 0; i < (int)mesh->mTriangles.size(); ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			int mTlid = mesh->mTriangles[i].mSubMtrl;
			auto& vertices = mesh->mVertices[mTlid];
			auto& indices = mesh->mIndices[mTlid];
			int vNumber = [&](VertexW& rhs) -> int
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

Point3 ObjectExporter::GetVertexWNormal(Mesh & mesh, int faceNo, const RVertex & rv)
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

void ObjectExporter::LoadBipedInfo(INode * node, std::vector<BipedVertex>& bipedes, const std::wstring& name)
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

void ObjectExporter::InputBipedes(VertexW & vertex, const BipedVertex & bipedes)
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

void ObjectExporter::ExportPhysiqueData(INode * node, std::vector<BipedVertex>& bipedes, Modifier * phyMod, const std::wstring& name)
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

void ObjectExporter::ExportSkinData(INode * node, std::vector<BipedVertex>& bipedes, Modifier * skinMod, const std::wstring& name)
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

void ObjectExporter::GetNodeName(INode * node, std::wstring & name)
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

ObjectExporter::ObjectExporter(ZXCSExporter * exporter)
	: mExporter(exporter)
{
}
