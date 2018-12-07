#pragma once

#include "Core.h"
#include "Minimap.h"
#include "Frustum.h"

class Sample : public Core
{
public:
	Sample(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName);

protected:
	bool Init();
	bool Frame();
	bool Render();

private:
	std::array<DirectX::XMFLOAT4X4, 100> mWorldarr;

	BoxShape m_Box;

	boundingBox box[100];

	Minimap mMinimap;

	Microsoft::WRL::ComPtr<ID3D11Buffer> mInstanceBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mConstant = nullptr;

	Frustum mFrustum;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader[2];

	std::vector<DirectX::XMFLOAT4X4> mInstance;
};
