#pragma once

#include "Core.h"
#include "Shape.h"
#include "Minimap.h"

class Sample : public Core
{
public:
	Sample(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName);

protected:
	bool Init();
	bool Frame();
	bool Render();

private:
	void FramePassCB();
	void FrameTexCB();

private:
	PassConstants mMainPassCB;
	PassConstants mTexPassCB;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mPassCB = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mTexCB = nullptr;

	Minimap m_Minimap;

	BoxShape m_Box;
};
