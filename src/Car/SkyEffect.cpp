#include <d3dcompiler.h>
#include "Effects.h"
#include "Utils.h"
#include "RenderStates.h"

using namespace DirectX;

SkyEffect::~SkyEffect()
{
}

bool SkyEffect::InitAll(ID3D11Device* device)
{
	if (!device)
		return false;

	ID3DBlob* blob;

	// 
	// ����������ɫ��
	//
	HR(CreateShaderFromFile(L"HLSL\\SkyVS.cso", L"HLSL\\SkyVS.hlsl", "VS", "vs_5_0", &blob));
	HR(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &mVS));

	D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[1] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	HR(device->CreateInputLayout(inputLayoutDesc, ARRAYSIZE(inputLayoutDesc), blob->GetBufferPointer(), blob->GetBufferSize(), &mVertexInputLayout));

	HR(D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&mVSReflector));

	// 
	// ����������ɫ��
	//

	HR(CreateShaderFromFile(L"HLSL\\SkyPS.cso", L"HLSL\\SkyPS.hlsl", "PS", "ps_5_0", &blob));
	HR(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &mPS));

	HR(D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&mPSReflector));

	//
	// ��ʼ������������ 
	//
	Reflect(device);

	return true;
}

void SkyEffect::ResetRenderStates(ID3D11DeviceContext* deviceContext)
{
	// ���ö��㲼�ֺ�ͼԪ����
	deviceContext->IASetInputLayout(mVertexInputLayout);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ������ɫ��
	deviceContext->VSSetShader(mVS, nullptr, 0);
	deviceContext->PSSetShader(mPS, nullptr, 0);
	deviceContext->GSSetShader(nullptr, nullptr, 0);

	// ����Ĭ��״̬
	deviceContext->PSSetSamplers(0, 1, &RenderStates::LinearWrapSS);
	deviceContext->RSSetState(RenderStates::NoCullRS);
	deviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
	deviceContext->OMSetDepthStencilState(RenderStates::LessEqualDSS, 0);
}

void XM_CALLCONV SkyEffect::SetWorldViewProjMatrix(DirectX::FXMMATRIX WVP)
{
	XMMATRIX wvp = XMMatrixTranspose(WVP);
	SetLocalBufferVS("gWorldViewProj", (void*)&wvp);
}

void SkyEffect::SetTextureCube(ID3D11ShaderResourceView* textureCube)
{
	mTextureCube = textureCube;
}

void SkyEffect::Apply(ID3D11DeviceContext* deviceContext)
{
	// �������������󶨵���Ⱦ������
	for (UINT i = 0; i < mCBuffersVS.size(); ++i)
	{
		deviceContext->VSSetConstantBuffers(i, 1, &mCBuffersVS[i].CBuffer);
		UpdateBufferVS(deviceContext, i);
	}

	for (UINT i = 0; i < mCBuffersPS.size(); ++i)
	{
		deviceContext->PSSetConstantBuffers(i, 1, &mCBuffersPS[i].CBuffer);
		UpdateBufferPS(deviceContext, i);
	}

	deviceContext->PSSetShaderResources(0, 1, &mTextureCube);
}