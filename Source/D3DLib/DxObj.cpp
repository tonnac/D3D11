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
	dxobj->m_pVertexShader = storage->getVertexShader(L"default");
	dxobj->m_pPixelShader = storage->getPixelShader(L"default");
	dxobj->m_pInputLayout = storage->getInputLayout(L"default");

	_DxObj[DxType::DEFAULT] = std::move(dxobj);

	dxobj = std::make_unique<DxObj>();
	dxobj->m_pVertexShader = storage->getVertexShader(L"line");
	dxobj->m_pPixelShader = storage->getPixelShader(L"line");
	dxobj->m_pInputLayout = storage->getInputLayout(L"line");

	_DxObj[DxType::LINE] = std::move(dxobj);

	dxobj = std::make_unique<DxObj>();
	dxobj->m_pVertexShader = storage->getVertexShader(L"composite");
	dxobj->m_pPixelShader = storage->getPixelShader(L"composite");
	dxobj->m_pInputLayout = nullptr;

	_DxObj[DxType::COMPOSITE] = std::move(dxobj);

	dxobj = std::make_unique<DxObj>();
	dxobj->m_pVertexShader = storage->getVertexShader(L"skinned");
	dxobj->m_pPixelShader = storage->getPixelShader(L"default");
	dxobj->m_pInputLayout = storage->getInputLayout(L"skinned");

	_DxObj[DxType::SKINNED] = std::move(dxobj);

	dxobj = std::make_unique<DxObj>();
	dxobj->m_pVertexShader = storage->getVertexShader(L"sky");
	dxobj->m_pPixelShader = storage->getPixelShader(L"sky");
	dxobj->m_pInputLayout = storage->getInputLayout(L"sky");
	dxobj->m_RasterizerState = E_RSS::SolidBack;
	_DxObj[DxType::SKY] = std::move(dxobj);

	dxobj = std::make_unique<DxObj>();
	dxobj->m_pVertexShader = storage->getVertexShader(L"default");
	dxobj->m_pPixelShader = storage->getPixelShader(L"notex");
	dxobj->m_pInputLayout = storage->getInputLayout(L"default");
	dxobj->m_RasterizerState = E_RSS::Default;
	_DxObj[DxType::NOTEX] = std::move(dxobj);

	dxobj = std::make_unique<DxObj>();
	dxobj->m_pVertexShader = storage->getVertexShader(L"normal");
	dxobj->m_pGeometryShader = storage->getGeometryShader(L"normal");
	dxobj->m_pPixelShader = storage->getPixelShader(L"normal");
	dxobj->m_pInputLayout = storage->getInputLayout(L"skinned");
	_DxObj[DxType::NORMAL] = std::move(dxobj);

	dxobj = std::make_unique<DxObj>();
	dxobj->m_pVertexShader = storage->getVertexShader(L"skinned");
	dxobj->m_pPixelShader = storage->getPixelShader(L"bump");
	dxobj->m_pInputLayout = storage->getInputLayout(L"skinned");

	_DxObj[DxType::SKINBUMP] = std::move(dxobj);

	dxobj = std::make_unique<DxObj>();
	dxobj->m_pVertexShader = storage->getVertexShader(L"default");
	dxobj->m_pPixelShader = storage->getPixelShader(L"bump");
	dxobj->m_pInputLayout = storage->getInputLayout(L"default");

	_DxObj[DxType::BUMP] = std::move(dxobj);

	return _DxObj;
}
