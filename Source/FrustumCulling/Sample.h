#pragma once

#include "Core.h"
#include "Minimap.h"
#include "Frustum.h"

class Sample : public Core
{
public:
	Sample(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName);

protected:
	virtual bool Init()override;
	virtual bool Frame()override;
	virtual bool Render()override;

	void UpdateMinimapCB();
private:
	std::array<DirectX::XMFLOAT4X4, 100> mWorldarr;

	Minimap m_Minimap;

	BoxShape m_Box;

	Frustum mFrustum;

	boundingBox box[100];

	Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader[2];

	PassConstants mMinimapConstants;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mMinimapCB;
	DirectX::XMFLOAT4X4 mMinimapView;
};
