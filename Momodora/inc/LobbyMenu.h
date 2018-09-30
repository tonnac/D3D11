#pragma once
#include "Setting.h"

class Bar;

enum class LOBBYSTATE : unsigned char
{
	DEFAULT,
	START,
	MAINMENU,
	SELECT
};


class Lobbymenu : public Menu
{
public:
	Lobbymenu();
public:
	bool InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
				const std::string& VSFunc = "VS", const std::string& PSFunc = "PS") override;
	void SetPos(const FLOAT& rtLeft, const FLOAT& rtTop, const FLOAT& rtRight, const FLOAT& rtBottom) override;
	bool Frame() override;
	bool Render(ID3D11DeviceContext* pContext) override;
	bool Release() override;
public:
	bool DownKey() override;
	bool UPKey() override;
public:
	void setState(const LOBBYSTATE& type);
private:
	bool StateFrame();
private:
	LOBBYSTATE					m_State;
	std::shared_ptr<Bar>		m_Bar;
	std::shared_ptr<Bar>		m_Start;
	std::array<D3DXVECTOR4, 2>	m_DrawArray;
	std::array<D3DXVECTOR2, 8>	m_BarCenterPos;
	std::array<D3DXVECTOR4, 8>	m_BarDrawArray;
	std::array<D3DXVECTOR2, 6>	m_StartCenterPos;
};