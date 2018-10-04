#pragma once
#include "Effect.h"

class Misc : public Effect
{
public:
	bool		InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
				const std::string& VSFunc = "VS", const std::string& PSFunc = "PS") override;
	bool		Frame() override;
	bool		PostRender(ID3D11DeviceContext* pContext) override;
	void		setColorX(const FLOAT& fVal);
	void		setFlag(const bool& flag);
private:
	bool		bFlag;
};