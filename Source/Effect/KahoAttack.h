#pragma once
#include "Effect.h"

class PlayerEffect : public Effect
{};

class KahoAttack : public PlayerEffect
{
public:
	virtual ~KahoAttack();
public:
	virtual bool	Frame() override;
	virtual bool	Release() final;
protected:
	INT				m_iValidIndex;
};

class KahoAttack1 : public KahoAttack
{
public:
	bool			InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
					const std::string& VSFunc = "VS", const std::string& PSFunc = "PS") override;
};

class KahoAttack2 : public KahoAttack
{
public:
	bool			InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
					const std::string& VSFunc = "VS", const std::string& PSFunc = "PS") override;
};

class KahoAttack3 : public KahoAttack
{
public:
	bool			InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
					const std::string& VSFunc = "VS", const std::string& PSFunc = "PS") override;
};

class KahoAirAttack : public KahoAttack
{
public:
	bool			InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
					const std::string& VSFunc = "VS", const std::string& PSFunc = "PS") override;
};

class KahoBowAttack : public PlayerEffect
{
public:
	bool			InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
					const std::string& VSFunc, const std::string& PSFunc) override;
	bool			Frame() override;
};