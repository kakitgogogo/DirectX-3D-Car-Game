#include "Sky.hlsli"

cbuffer cbPerFrame : register(b0)
{
	matrix gWorldViewProj;
}

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	// �� z = w���� z/w = 1�� ʹ����պб�����Զƽ��
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj).xyww;

	vout.PosL = vin.PosL;

	return vout;
}