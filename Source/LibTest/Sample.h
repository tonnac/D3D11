#pragma once

#include "Core.h"
#include "Shape.h"

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

private:
	
	PassConstants mMainPassCB;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mPassCB = nullptr;

//	PlaneShape m_Plane;
	LineShape m_line;
	BoxShape m_Box;
};
