#include "SobelFilter.h"

void SobelFilter::Initialize(ID3D11Device * pd3Device, UINT width, UINT height)
{
	CD3D11_TEXTURE2D_DESC texDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R8G8B8A8_UNORM, Casting(UINT, width), Casting(UINT, height));
	

}

void SobelFilter::Execute(ID3D11DeviceContext * context, ID3D11ShaderResourceView** texture)
{

}
