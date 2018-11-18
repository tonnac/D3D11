#include "Camera.h"

using namespace DirectX;

XMFLOAT4X4 Camera::SetViewMatrix(XMFLOAT3 vPos, XMFLOAT3 vTarget, XMFLOAT3 vUp)
{
	m_vPos = vPos; m_vTarget = vTarget; m_vUp = vUp;


	XMVECTOR Pos = { vPos.x, vPos.y, vPos.z, 1.0f };
	XMVECTOR Target = XMVectorZero();
	XMVECTOR Up = XMVectorSet(vUp.x, vUp.y, vUp.z, 0.0f);

	XMMATRIX view = XMMatrixLookAtLH(Pos, Target, Up);

	XMStoreFloat4x4(&m_matView, view);

	UpdateVector();
	return m_matView;
}

XMFLOAT4X4 Camera::SetProjMatrix(float fFov, float fAspect, float fNear, float fFar)
{
	m_fFov = fFov; m_fNear = fNear;
	m_fAspect = fAspect; m_fFar = fFar;
	XMMATRIX Proj = XMMatrixPerspectiveFovLH(fFov, fAspect, fNear, fFar);

	XMStoreFloat4x4(&m_matProj, Proj);

	UpdateVector();
	return m_matProj;
}

void Camera::UpdateProjMatrix(float fAspect)
{
	XMMATRIX Proj = XMMatrixPerspectiveFovLH(m_fFov, fAspect, m_fNear, m_fFar);
	XMStoreFloat4x4(&m_matProj, Proj);
	UpdateVector();
}

bool Camera::Frame()
{
	//m_fSpeed -= g_fSecPerFrame * 1.0f;
	//if (m_fSpeed <= 1.0f) m_fSpeed = 1.0f;
	//else
	//{
	//	m_vPos += m_vLook * m_fSpeed * 0.5f;
	//}
	return true;
}

void Camera::UpdateVector()
{
	m_vLook.x = m_matView._13;
	m_vLook.y = m_matView._23;
	m_vLook.z = m_matView._33;

	m_vUpvector.x = m_matView._12;
	m_vUpvector.y = m_matView._22;
	m_vUpvector.z = m_matView._32;

	m_vSide.x = m_matView._11;
	m_vSide.y = m_matView._21;
	m_vSide.z = m_matView._31;


	XMVECTOR look = XMVectorSet(m_vLook.x, m_vLook.y, m_vLook.z, 0.0f);
	XMVECTOR up = { m_vUpvector.x, m_vUpvector.y, m_vUpvector.z };
	XMVECTOR side = { m_vSide.x, m_vSide.y, m_vSide.z };
	XMVECTOR LookN = XMVector3Normalize(look);
	XMVECTOR UpN = XMVector3Normalize(up);
	XMVECTOR SideN = XMVector3Normalize(side);

	XMStoreFloat3(&m_vLook, LookN);
	XMStoreFloat3(&m_vUpvector, UpN);
	XMStoreFloat3(&m_vSide, SideN);
}

bool Camera::Update(XMFLOAT4 vValue)
{

	//D3DXQUATERNION qRotation;
	//D3DXQuaternionRotationYawPitchRoll(&qRotation, vValue.y, vValue.x, vValue.z);

	//m_vPos += m_vLook * vValue.w;

	//XMFLOAT4X4AffineTransformation(&matRotation, 1.0f, nullptr, &qRotation, &m_vPos);

	//XMFLOAT4X4Inverse(&m_matView, nullptr, &matRotation);

	//UpdateVector();
	return true;
}

void Camera::MoveLook(float fValue)
{
	XMVECTOR Pos = XMLoadFloat3(&m_vPos);
	XMVECTOR Look = XMLoadFloat3(&m_vLook);

	Pos += Look * fValue * m_fSpeed;

	XMStoreFloat3(&m_vPos, Pos);
}

void Camera::MoveSide(float fValue)
{
	XMVECTOR Pos = XMLoadFloat3(&m_vPos);
	XMVECTOR Side = XMLoadFloat3(&m_vSide);

	Pos += Side * fValue * m_fSpeed;

	XMStoreFloat3(&m_vPos, Pos);
}