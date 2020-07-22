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

	// ת��������ռ�
	vout.PosW = posW.xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);

	// ת������βü��ռ�
	vout.PosH = mul(mul(posW, gView), gProj);

	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

	return vout;
}