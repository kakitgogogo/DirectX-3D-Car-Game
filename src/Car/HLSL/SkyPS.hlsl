#include "Sky.hlsli"

TextureCube gCubeMap : register(t0);
SamplerState gSampler : register(s0);

float4 PS(VertexOut pin) : SV_Target
{
	return gCubeMap.Sample(gSampler, pin.PosL);
}