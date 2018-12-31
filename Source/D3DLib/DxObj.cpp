#include "DxObj.h"


void DxObj::SetResource(ID3D11DeviceContext* pContext)
{
	pContext->IASetInputLayout(m_pInputLayout);
	pContext->VSSetShader(m_pVertexShader, nullptr, 0);
	pContext->PSSetShader(m_pPixelShader, nullptr, 0);
	pContext->GSSetShader(m_pGeometryShader, nullptr, 0);

	pContext->RSSetState(DxState::m_RSS[(int)m_RasterizerState].Get());
	pContext->OMSetDepthStencilState(DxState::m_DSS[(int)m_DepthStencilState].Get(), 0);
	pContext->OMSetBlendState(DxState::m_BSS[(int)m_BlendState].Get(), 0, -1);
}

std::unordered_map<DxType, std::unique_ptr<DxObj>> DxObjStorage::GetDxobjList()
{
	std::unordered_map<DxType, std::unique_ptr<DxObj>> _DxObj;

	ShaderStorage * storage = ShaderStorage::Storage();
	std::unique_ptr<DxObj> dxobj = std::make_unique<DxObj>();
	dxobj->m_pVertexShader = storage->getVertexShader("default");
	dxobj->m_pPixelShader = storage->getPixelShader("default");
	dxobj->m_pInputLayout = storage->getInputLayout("default");

	_DxObj[DxType::DEFAULT] = std::move(dxobj);

	dxobj = std::make_unique<DxObj>();
	dxobj->m_pVertexShader = storage->getVertexShader("line");
	dxobj->m_pPixelShader = storage->getPixelShader("line");
	dxobj->m_pInputLayout = storage->getInputLayout("line");

	_DxObj[DxType::LINE] = std::move(dxobj);

	dxobj = std::make_unique<DxObj>();
	dxobj->m_pVertexShader = storage->getVertexShader("skinned");
	dxobj->m_pPixelShader = storage->getPixelShader("default");
	dxobj->m_pInputLayout = storage->getInputLayout("skinned");
	dxobj->m_BlendState = E_BSS::No;

	_DxObj[DxType::SKINNED] = std::move(dxobj);

	dxobj = std::make_unique<DxObj>();
	dxobj->m_pVertexShader = storage->getVertexShader("sky");
	dxobj->m_pPixelShader = storage->getPixelShader("sky");
	dxobj->m_pInputLayout = storage->getInputLayout("sky");
	dxobj->m_RasterizerState = E_RSS::SolidBack;
	_DxObj[DxType::SKY] = std::move(dxobj);

	dxobj = std::make_unique<DxObj>();
	dxobj->m_pVertexShader = storage->getVertexShader("default");
	dxobj->m_pPixelShader = storage->getPixelShader("notex");
	dxobj->m_pInputLayout = storage->getInputLayout("default");
	dxobj->m_RasterizerState = E_RSS::Default;
	_DxObj[DxType::NOTEX] = std::move(dxobj);

	return _DxObj;
}
