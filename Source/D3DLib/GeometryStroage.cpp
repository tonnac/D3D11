#include "GeometryStroage.h"

void GeometryStroage::SaveGeometry(std::unique_ptr<MeshGeometry>& geometry)
{
	mGeometries.push_back(std::move(geometry));
}

MeshGeometry * GeometryStroage::operator[](int index)
{
	return mGeometries[index].get();
}
