#pragma once
#include "Bar.h"
#include "Button.h"
#include "Font.h"

class InGameBar : public Bar
{
public:
	bool InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
					const std::string& VSFunc = "VS", const std::string& PSFunc = "PS") override;
	bool Frame() override;
	bool Render(ID3D11DeviceContext* pContext) override;
	bool Release() override;
public:
	void setMove(const bool& flag = true);
	INT	getIndex() const;
	void setIndex(const INT& index);
private:
	INT						m_iIndex;
	std::array<Button, 5>	m_Button;
	std::array<Font, 5>		m_Font;
};