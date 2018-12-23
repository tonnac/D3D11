#include "DxObj.h"


void DxObj::SetResource(ID3D11DeviceContext* pContext)
{
	pContext->IASetInputLayout(m_pInputLayout.Get());
	pContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	pContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
	pContext->GSSetShader(m_pGeometryShader.Get(), nullptr, 0);
}
