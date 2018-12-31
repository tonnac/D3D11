#include <iostream>
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;
using namespace std;

int main()
{
	XMFLOAT3 f0(1.0f, 0.0f, 0.0f);
	XMFLOAT3 f1(0.0f, 0.0f, 1.0f);

	XMVECTOR F0 = XMLoadFloat3(&f0);
	XMVECTOR F1 = XMLoadFloat3(&f1);

	XMVECTOR Q0 = XMVector3Cross(F0, F1);
	XMVECTOR Q1 = XMVector3Cross(F1, F0);

	XMFLOAT3 q0;
	XMFLOAT3 q1;

	XMStoreFloat3(&q0, Q0);
	XMStoreFloat3(&q1, Q1);

	XMFLOAT3 v0(-4.987, -3.011, -1.182);
	XMFLOAT3 v1(-6.785, -3.011, -1.182);
	XMFLOAT3 v2(-6.785, -3.610, -1.781);

	XMVECTOR V0 = XMLoadFloat3(&v0);
	XMVECTOR V1 = XMLoadFloat3(&v1);
	XMVECTOR V2 = XMLoadFloat3(&v2);

	XMVECTOR e1 = V1 - V0;
	XMVECTOR e2 = V2 - V0;

	XMFLOAT2 t0 = { 0.1947f, 0.3684f };
	XMFLOAT2 t1 = { 0.1947f, 0.3684f };
	XMFLOAT2 t2 = { 0.1717f, 0.4056f };

	XMVECTOR T0 = XMLoadFloat2(&t0);
	XMVECTOR T1 = XMLoadFloat2(&t1);
	XMVECTOR T2 = XMLoadFloat2(&t2);

	XMVECTOR p1 = T1 - T0;
	XMVECTOR p2 = T2 - T0;

	XMFLOAT3X3 Tan = XMFLOAT3X3(XMVectorGetX(p1), XMVectorGetY(p1), XMVectorGetZ(p1),
								XMVectorGetX(p2), XMVectorGetY(p2), XMVectorGetZ(p2),
								0.0f, 0.0f, 1.0f);

	XMMATRIX x = XMLoadFloat3x3(&Tan);
	x = XMMatrixInverse(&XMMatrixDeterminant(x), x);

	XMFLOAT3X3 I = XMFLOAT3X3(XMVectorGetX(e1), XMVectorGetY(e1), XMVectorGetZ(e1),
					XMVectorGetX(e2), XMVectorGetY(e2), XMVectorGetZ(e2),
					0.0f, 0.0f, 1.0f);

	XMMATRIX o = XMLoadFloat3x3(&I);

	XMMATRIX result = x * o;

	XMVECTOR E0 = result.r[0];
	XMVECTOR E1 = result.r[1];

	XMFLOAT3 ret1;
	XMFLOAT3 ret2;

	XMStoreFloat3(&ret1, E0);
	XMStoreFloat3(&ret2, E1);


	int at = 5;
	
	

	return 0;
}