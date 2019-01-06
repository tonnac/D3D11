#include "GeometryStroage.h"

void GeometryStroage::SaveGeometry(std::unique_ptr<MeshGeometry>& geometry)
{
	mGeometries.insert(std::make_pair(geometry->Name, std::move(geometry)));
}

MeshGeometry * GeometryStroage::operator[](const std::wstring& name)
{
	auto iter = mGeometries.find(name);
	if (iter == mGeometries.end())
		return nullptr;

	return mGeometries[name].get();
}
