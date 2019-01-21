#pragma once

#include <DirectXMath.h>
#include <float.h>
#include <cstdint>
#include <cstdlib>

class MathHelper
{
public:

	static float RandF()
	{
		return (float)(rand()) / (float)RAND_MAX;
	}

	static float RandF(float a, float b)
	{
		return a + RandF() * (b - a);
	}

	static int Rand(int a, int b)
	{
		return a + rand() % ((b - a) + 1);
	}

	template <typename X>
	static X Min(const X& a, const X& b)
	{
		return a < b ? a : b;
	}

	template <typename X>
	static X Max(const X& a, const X& b)
	{
		return a > b ? a : b;
	}

	template <typename X>
	static X Lerp(const X& a, const X& b, float t)
	{
		return a + (b - a) * t;
	}

	template <typename X>
	static X Clamp(const X& x, const X& low, const X& high)
	{
		return x < low ? low : (x > high ? high : x);
	}

	static DirectX::XMFLOAT4X4 Identity4x4()
	{
		static DirectX::XMFLOAT4X4 I(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
		return I;
	}

	static DirectX::XMMATRIX InverseTranspose(DirectX::CXMMATRIX M);

	static constexpr float Infinity = FLT_MAX;
	static constexpr float PI = 3.1415926535f;
};