#pragma once

#include <DirectXMath.h>
#include <float.h>
#include <cstdint>

class MathHelper
{
public:

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

	static constexpr float Infinity = FLT_MAX;
	static constexpr float PI = 3.1415926535f;
};