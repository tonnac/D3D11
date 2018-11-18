#pragma once
#include <DirectXMath.h>
#include <Windows.h>

class Camera
{

public:
	Camera() = default;
	virtual ~Camera() = default;

public:
	DirectX::XMFLOAT4X4 SetViewMatrix(DirectX::XMFLOAT3 vPos, DirectX::XMFLOAT3 vTarget = DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3 vUp = DirectX::XMFLOAT3(0, 1, 0));
	DirectX::XMFLOAT4X4 SetProjMatrix(float fFov, float fAspect, float fNear = 1.0f, float fFar = 1000.0f);

	void UpdateProjMatrix(float fAspect);

	bool Frame();
	bool Update(DirectX::XMFLOAT4 vValue);
	void UpdateVector();
	void MoveLook(float fValue);
	void MoveSide(float fValue);

public:
	DirectX::XMFLOAT4X4 m_matView;
	DirectX::XMFLOAT4X4 m_matProj;

	float m_fFov;
	float m_fAspect;
	float m_fNear;
	float m_fFar;
	float m_fSpeed = 1.0f;

	DirectX::XMFLOAT3 m_vPos;
	DirectX::XMFLOAT3 m_vTarget;
	DirectX::XMFLOAT3 m_vUp;

	DirectX::XMFLOAT3 m_vLook;
	DirectX::XMFLOAT3 m_vSide;
	DirectX::XMFLOAT3 m_vUpvector;
};