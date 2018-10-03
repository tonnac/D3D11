#pragma once
#include "Font.h"

class ItemFont : public Font
{
public:
	bool InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& ShaderFilepath,
				const std::string& VSFunc = "VS", const std::string& PSFunc = "PS");
	bool Frame() override;
	bool Render(ID3D11DeviceContext* pContext);
public:
	void setTexture(Texture* pTexture);
public:
	Texture * getTexture();
private:
	Texture * m_pTexture;
};