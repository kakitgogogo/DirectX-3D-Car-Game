#include "Basic.hlsli"
#include "LightHelper.hlsli"

cbuffer cbperobject : register(b0)
{
	Material gMaterial;
}

cbuffer cbPerFrame : register(b1)
{
	float4x4 gView;
	float3 gEyePosW;
	float pad1;
}

cbuffer cbRenderStates : register(b2)
{
	bool gUseTexture;
	bool gFogEnabled;
	float gFogStart;
	float gFogRange;
	float4 gColor;
}

cbuffer cbChangeRarely : register(b3)
{
	DirectionalLight gDirLights[3];
}

Texture2D gDiffuseMap : register(t0);

SamplerState gSamplerState : register(s0);

float4 PS(VertexOut pin) : SV_Target
{
	float4 texColor = gColor;
	if (gUseTexture)
	{
		// �������
		texColor = gDiffuseMap.Sample(gSamplerState, pin.Tex);
		clip(texColor.a - 0.1f);
	}
	
	// ��׼��������
	pin.NormalW = normalize(pin.NormalW);

	// ���㵽�����������
	float3 toEyeW = normalize(gEyePosW - pin.PosW);

	// ��ʼ�����ձ���
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	[unroll]
	for (int i = 0; i < 3; ++i)
	{
		float4 A, D, S;
		ComputeDirectionalLight(gMaterial, gDirLights[i], pin.NormalW, toEyeW, A, D, S);

		// �ۼ�ÿ����Դ�Ĺ���
		ambient += A;
		diffuse += D;
		spec += S;
	}

	float4 litColor = texColor * (ambient + diffuse) + spec;
	litColor.a = gMaterial.Diffuse.a * texColor.a;

	return litColor;
}