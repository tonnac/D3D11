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

	std::array<DirectX::XMFLOAT4X4, 100> mTWorldarr;


	BoxShape m_Box;

	Frustum mFrustum;

	boundingBox box[100];

	Microsoft::WRL::ComPtr<ID3D11Buffer> mInstanceBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mInstanceView = nullptr;
};
