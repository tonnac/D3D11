#include "DxState.h"

using namespace Microsoft::WRL;

std::array<ComPtr<ID3D11DepthStencilState>, (int)E_DSS::Count> DxState::m_DSS;
std::array<ComPtr<ID3D11RasterizerState>, (int)E_RSS::Count> DxState::m_RSS;
std::array<ComPtr<ID3D11BlendState>, (int)E_BSS::Count> DxState::m_BSS;

std::array<ComPtr<ID3D11SamplerState>, (int)E_SS::Count> DxState::m_SS;
std::array<ID3D11SamplerState*, (int)E_SS::Count> DxState::m_SamplerStateArray;

std::array<ID3D11SamplerState*, (int)E_SS::Count> DxState::GetSamArray()
{
	return m_SamplerStateArray;
}

void DxState::InitState(ID3D11Device* pd3dDevice)
{
	InitDepthStencliState(pd3dDevice);
	InitRasterizerState(pd3dDevice);
	InitBlendState(pd3dDevice);
	InitSamplerState(pd3dDevice);
}

void DxState::InitDepthStencliState(ID3D11Device* pd3dDevice)
{
	ComPtr<ID3D11DepthStencilState> DSS;
	D3D11_DEPTH_STENCIL_DESC dsDesc;

	ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	
	ThrowifFailed(pd3dDevice->CreateDepthStencilState(&dsDesc, DSS.GetAddressOf()));
	m_DSS[(int)E_DSS::Default] = DSS;

	DSS.Reset();
	dsDesc.DepthEnable = FALSE;
	ThrowifFailed(pd3dDevice->CreateDepthStencilState(&dsDesc, DSS.GetAddressOf()));
	m_DSS[(int)E_DSS::Disable] = DSS;

	DSS.Reset();
	dsDesc.DepthFunc = D3D11_COMPARISON_GREATER;
	ThrowifFailed(pd3dDevice->CreateDepthStencilState(&dsDesc, DSS.GetAddressOf()));
	m_DSS[(int)E_DSS::Greater] = DSS;

	DSS.Reset();
	D3D11_DEPTH_STENCIL_DESC sDesc;
	ZeroMemory(&sDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	sDesc.DepthEnable = TRUE;
	sDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	sDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	sDesc.StencilEnable = TRUE;
	sDesc.StencilReadMask = 0xff;
	sDesc.StencilWriteMask = 0xff;
	D3D11_DEPTH_STENCILOP_DESC Front;
	Front.StencilFunc = D3D11_COMPARISON_ALWAYS;
	Front.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	Front.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	Front.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	sDesc.FrontFace = Front;
	Front.StencilFunc = D3D11_COMPARISON_NEVER;
	sDesc.BackFace = Front;
	ThrowifFailed(pd3dDevice->CreateDepthStencilState(&sDesc, DSS.GetAddressOf()));
	m_DSS[(int)E_DSS::Stencil] = DSS;
}

void DxState::InitRasterizerState(ID3D11Device* pd3dDevice)
{
	ComPtr<ID3D11RasterizerState> RSS;
	D3D11_RASTERIZER_DESC rsDesc;

	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_BACK;
	rsDesc.DepthClipEnable = TRUE;


	ThrowifFailed(pd3dDevice->CreateRasterizerState(&rsDesc, RSS.GetAddressOf()));
	m_RSS[(int)E_RSS::Default] = RSS;

	RSS.Reset();
	rsDesc.FillMode = D3D11_FILL_WIREFRAME;
	ThrowifFailed(pd3dDevice->CreateRasterizerState(&rsDesc, RSS.GetAddressOf()));
	m_RSS[(int)E_RSS::Wireframe] = RSS;

	RSS.Reset();
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_FRONT;
	ThrowifFailed(pd3dDevice->CreateRasterizerState(&rsDesc, RSS.GetAddressOf()));
	m_RSS[(int)E_RSS::Solidfront] = RSS;

	RSS.Reset();
	rsDesc.CullMode = D3D11_CULL_NONE;
	ThrowifFailed(pd3dDevice->CreateRasterizerState(&rsDesc, RSS.GetAddressOf()));
	m_RSS[(int)E_RSS::SolidBack] = RSS;
}

void DxState::InitBlendState(ID3D11Device* pd3dDevice)
{
	ComPtr<ID3D11BlendState> BSS;
	D3D11_BLEND_DESC bsDesc;
	ZeroMemory(&bsDesc, sizeof(D3D11_BLEND_DESC));
	bsDesc.RenderTarget[0].BlendEnable = TRUE;

	bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	bsDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bsDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bsDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	ThrowifFailed(pd3dDevice->CreateBlendState(&bsDesc, BSS.GetAddressOf()));
	m_BSS[(int)E_BSS::Default] = BSS;

	BSS.Reset();
//	bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
//	bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_ALPHA;
	bsDesc.RenderTarget[0].BlendEnable = FALSE;
	ThrowifFailed(pd3dDevice->CreateBlendState(&bsDesc, BSS.GetAddressOf()));
	m_BSS[(int)E_BSS::No] = BSS;
}

void DxState::InitSamplerState(ID3D11Device* pd3dDevice)
{
	ComPtr<ID3D11SamplerState> pSamplerstate;

	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sd.MaxLOD = FLT_MAX;
	sd.MinLOD = FLT_MIN;

	ThrowifFailed(pd3dDevice->CreateSamplerState(&sd, pSamplerstate.GetAddressOf()));
	m_SS[(int)E_SS::PointWrap] = pSamplerstate;
	
	pSamplerstate.Reset();

	///////////////////////////////////////////////////////////////////////////////////

	sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

	ThrowifFailed(pd3dDevice->CreateSamplerState(&sd, pSamplerstate.GetAddressOf()));
	m_SS[(int)E_SS::PointClamp] = pSamplerstate;
	pSamplerstate.Reset();

	///////////////////////////////////////////////////////////////////////////////////

	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	ThrowifFailed(pd3dDevice->CreateSamplerState(&sd, pSamplerstate.GetAddressOf()));
	m_SS[(int)E_SS::LinearWrap] = pSamplerstate;
	pSamplerstate.Reset();

	///////////////////////////////////////////////////////////////////////////////////

	sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	ThrowifFailed(pd3dDevice->CreateSamplerState(&sd, pSamplerstate.GetAddressOf()));
	m_SS[(int)E_SS::LinearClamp] = pSamplerstate;
	pSamplerstate.Reset();

	///////////////////////////////////////////////////////////////////////////////////

	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.Filter = D3D11_FILTER_ANISOTROPIC;
	sd.MipLODBias = 0.0f;
	sd.MaxAnisotropy = 8;

	ThrowifFailed(pd3dDevice->CreateSamplerState(&sd, pSamplerstate.GetAddressOf()));
	m_SS[(int)E_SS::AnisotropicWrap] = pSamplerstate;
	pSamplerstate.Reset();

	///////////////////////////////////////////////////////////////////////////////////

	sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.Filter = D3D11_FILTER_ANISOTROPIC;	

	ThrowifFailed(pd3dDevice->CreateSamplerState(&sd, pSamplerstate.GetAddressOf()));
	m_SS[(int)E_SS::AnisoTropicClamp] = pSamplerstate;
	pSamplerstate.Reset();

	for (E_SS index = E_SS::PointWrap; index < E_SS::Count; index = Casting(E_SS,(Casting(int,index) + 1)))
	{
		m_SamplerStateArray[Casting(int, index)] = m_SS[Casting(int, index)].Get();
	}
}