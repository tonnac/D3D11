#pragma once
#include "Object.h"

class TriangleObject : public Object
{
public:
	TriangleObject() = default;
public:
	bool				InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath) override;
	bool				Frame() override;
private:
	void				CreateVertex(ID3D11Device* pDevice);
};