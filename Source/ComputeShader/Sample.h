#pragma once

#include "Core.h"
#include "Minimap.h"
#include "Frustum.h"
#include "Mesh.h"
#include "GeometryGenerator.h"

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

	Microsoft::WRL::ComPtr<ID3D11Buffer> mDataA = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mDataB = nullptr;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mDataAView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mDataBView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> mUav = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Buffer> mOutput = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mReadBack = nullptr;

	Microsoft::WRL::ComPtr<ID3D11ComputeShader> mComputeShader = nullptr;
};

