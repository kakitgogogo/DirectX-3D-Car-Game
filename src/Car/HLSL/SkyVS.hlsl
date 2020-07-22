#include "Sky.hlsli"

cbuffer cbPerFrame : register(b0)
{
	matrix gWorldViewProj;
}

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	// 令 z = w，则 z/w = 1， 使得天空盒保持在远平面
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj).xyww;

	vout.PosL = vin.PosL;

	return vout;
}