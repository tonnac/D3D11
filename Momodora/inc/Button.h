#pragma once
#include "UI.h"

class Button : public UI
{
public:
	Button();
public:
	void LoadResourceView(ID3D11Device* pDevice, const std::tstring& TexFilepath);
	bool InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
				const std::string& VSFunc = "VS", const std::string& PSFunc = "PS") override;
	bool Frame() override;
	bool PreRender(ID3D11DeviceContext* pContext) override;
	bool Release() override;
public:
	bool getActive() const;
	void setActive(const bool& flag);
	void setMove(const bool& flag);
private:
	FLOAT m_fTimer;
	ID3D11ShaderResourceView * m_SRV;
	bool m_bActive;
	bool m_Move;
};