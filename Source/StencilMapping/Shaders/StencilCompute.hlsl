Texture2D<uint2> gInput : register(t0);
RWTexture2D<float4> gOutput : register(u0);

[numthreads(16, 16, 1)]
void SobelCS(uint3 DTid : SV_DispatchThreadID)
{
	uint2 c[3][3];

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			int2 xy = DTid.xy + int2(-1 + j, -1 + i);
			c[i][j] = gInput[xy];
		}
	}

	uint2 ret = 0;
	for (int a = 0; a < 3; ++a)
	{
		for (int p = 0; p < 3; ++p)
		{
			ret += c[a][p] - c[1][1];
		}
	}

	if (abs(ret.x) > 0)
	{
		gOutput[DTid.xy] = float4(1.0f, 1.0f, 0.0f, 1.0f);
	}
	else
	{
		gOutput[DTid.xy] = float4(0.0f, 0.0f, 0.0f, 0.0f);
	}

}