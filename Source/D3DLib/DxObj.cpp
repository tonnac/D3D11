#include "DxObj.h"


bool DxObj::PreRender(ID3D11DeviceContext* pContext)
{
	pContext->IASetInputLayout(m_pInputLayout.Get());

	UINT stride = m_iVertexSize;
	UINT offset = 0;
	pContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);

	pContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	pContext->VSSetConstantBuffers(1, 1, m_pConstantBuffer.GetAddressOf());
	pContext->PSSetConstantBuffers(1, 1, m_pConstantBuffer.GetAddressOf());

	pContext->PSSetShaderResources(0, 1, m_pTextureSRV.GetAddressOf());

	pContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	pContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
	pContext->GSSetShader(m_pGeometryShader.Get(), nullptr, 0);
	return true;
}

bool DxObj::PostRender(ID3D11DeviceContext* pContext)
{
	if (m_pIndexBuffer != nullptr)
		pContext->DrawIndexed(m_iNumIndex, 0, 0);
	else
		pContext->Draw(m_iNumIndex, 0);
	return true;
}

bool DxObj::Render(ID3D11DeviceContext* pContext)
{
	PreRender(pContext);
	PostRender(pContext);
	return true;
}