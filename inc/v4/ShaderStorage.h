#pragma once

#include "d3dUtil.h"

enum Inputlayout
{
	DEFAULT,
	SKINNED,
	LINE,
	SKY
};

class ShaderStorage
{
private:
	ShaderStorage() = default;

public:
	ShaderStorage(const ShaderStorage& rhs) = delete;
	ShaderStorage& operator = (const ShaderStorage& rhs) = delete;
	~ShaderStorage() = default;

	static ShaderStorage * Storage()
	{
		static ShaderStorage storage;
		return &storage;
	}

public:
	void Initialize(ID3D11Device * Device);

	ID3D11InputLayout* getInputLayout(const std::string& name);
	ID3D11VertexShader* getVertexShader(const std::string& name);
	ID3D11PixelShader* getPixelShader(const std::string& name);
	ID3D11GeometryShader* getGeometryShader(const std::string& name);
	ID3D11DomainShader* getDomainShader(const std::string& name);
	ID3D11ComputeShader* getComputeShader(const std::string& name);
	ID3D11HullShader * getHullShader(const std::string& name);

private:
	void CreateInputLayout(ID3D11Device * Device, Inputlayout type, ID3D11InputLayout ** inputLayout, ID3DBlob* blob);
	template<class X>
	bool Finding(const std::string& name, std::wstring& text, std::unordered_map<std::string, X> list)
	{
		auto iter = list.find(name);
		if (iter == list.end())
		{
			text.append(name.begin(), name.end());
			return false;
		}
		return true;
	}

private:
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11InputLayout>> mInputlayout;
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11VertexShader>> mVertexShader;
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11PixelShader>> mPixelShader;
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11GeometryShader>> mGeometryShader;
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11HullShader>> mHullShader;
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11DomainShader>> mDomainShader;
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ComputeShader>> mComputeShader;
};