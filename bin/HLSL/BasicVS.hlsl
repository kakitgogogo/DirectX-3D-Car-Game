#include "Basic.hlsli"

cbuffer cbPerObject : register(b0)
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gTexTransform;
}

cbuffer cbPerFrame : register(b1)
{
	float4x4 gView;
}

cbuffer cbChangeOnResize : register(b2)
{
	float4x4 gProj;
}

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	float4 posW = mul(float4(vin.PosL, 1.0f), gWorld);

	// 转换到世界空间
	vout.PosW = posW.xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);

	// 转换到齐次裁剪空间
	vout.PosH = mul(mul(posW, gView), gProj);

	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

	return vout;
}