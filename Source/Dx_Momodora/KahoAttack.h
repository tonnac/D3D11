#pragma once
#include "Effect.h"

class PlayerEffect : public Effect
{
public:
	PlayerEffect();
	virtual ~PlayerEffect();
public:
	bool			getEnd() const;
	void			setDir(const INT& Dir);
protected:
	bool			isEnd;
};

class KahoAttack : public PlayerEffect
{
public:
	KahoAttack();
public:
	virtual bool	Frame() override;

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
					const std::string& VSFunc = "VS", const std::string& PSFunc = "PS") override;
	bool			Frame() override;
};