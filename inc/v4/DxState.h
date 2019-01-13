#pragma once
#include "d3dUtil.h"

template <typename X>
void IncreaseEnum(X& arg)
{
	arg = (X)(((int)arg + 1) % (int)X::Count);
}

enum class E_DSS : unsigned char
{
	Default = 0,
	Disable,
	Greater,
	Stencil,
	Count
};

enum class E_RSS : unsigned char
{
	Default = 0,
	Solidfront,
	SolidBack,
	Wireframe,
	Count
};

enum class E_BSS : unsigned char
{
	Default = 0,
	No,
	Count
};

enum class E_SS : unsigned char
{
	PointWrap = 0,
	PointClamp,
	LinearWrap,
	LinearClamp,
	AnisotropicWrap,
	AnisoTropicClamp,
	Count
};

class DxState
{
public:

	static std::array<Microsoft::WRL::ComPtr<ID3D11BlendState>,			(int)E_BSS::Count> m_BSS;
	static std::array<Microsoft::WRL::ComPtr<ID3D11DepthStencilState>,	(int)E_DSS::Count> m_DSS;
	static std::array<Microsoft::WRL::ComPtr<ID3D11RasterizerState>,	(int)E_RSS::Count> m_RSS;

	static std::array<ID3D11SamplerState*, (int)E_SS::Count> GetSamArray();
	static void InitState(ID3D11Device* pd3dDevice);
private:
	static void InitDepthStencliState(ID3D11Device* pd3dDevice);
	static void InitRasterizerState(ID3D11Device* pd3dDevice);
	static void InitBlendState(ID3D11Device* pd3dDevice);
	static void InitSamplerState(ID3D11Device* pd3dDevice);

	static std::array<ID3D11SamplerState*, (int)E_SS::Count> m_SamplerStateArray;
	static std::array<Microsoft::WRL::ComPtr<ID3D11SamplerState>, (int)E_SS::Count>  m_SS;
};