#pragma once
#include "ZXCLoader.h"

class HelperViewer
{
	struct Vertex
	{
		DirectX::XMFLOAT3 Center;
		DirectX::XMFLOAT3 Max;
	};
public:
	HelperViewer(ID3D11Device* device, const std::tstring& fileName);
public:
	void BuildVBIB();

	void AddRenderItem(MeshNode* node);

	bool Render(ID3D11DeviceContext* context);

private:
	void CreateInputLayout(ID3DBlob* vertexBlob);

private:
	std::vector<RenderItem*> mDebugItem;

	MeshGeometry* mGeometry = nullptr;
	DxObj mDebugShader;

	std::vector<HelperViewer::Vertex> mVertices;
	std::vector<DWORD> mIndices;

	ID3D11Device * md3Device = nullptr;
};