#pragma once

#include "Core.h"
#include "Minimap.h"
#include "Frustum.h"
#include "Mesh.h"
#include "GeometryGenerator.h"
#include "SobelFilter.h"

class Sample : public Core
{
public:
	Sample(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName);

protected:
	virtual bool Init()override;
	virtual bool Frame()override;
	virtual bool Render()override;

	void BuildBuffer();
	void BuildView();


private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> mVS = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> mPS = nullptr;

	std::unique_ptr<SobelFilter> mSobelFilter = nullptr;
	DxRT mOffRT;
	Mesh mesh;
};

