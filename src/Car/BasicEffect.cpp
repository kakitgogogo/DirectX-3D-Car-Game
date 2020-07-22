#include <d3dcompiler.h>
#include "Effects.h"
#include "Utils.h"
#include "Vertex.h"
#include "RenderStates.h"

using namespace DirectX;

BasicEffect::~BasicEffect() 
{
}

bool BasicEffect::InitAll(ID3D11Device* device)
{
	if (!device)
		return false;

	ID3DBlob* blob;

	// 
	// 创建顶点着色器
	//
	HR(CreateShaderFromFile(L"HLSL\\BasicVS.cso", L"HLSL\\BasicVS.hlsl", "VS", "vs_5_0", &blob));
	HR(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &mVS));

	HR(device->CreateInputLayout(Vertex::inputLayoutDesc, ARRAYSIZE(Vertex::inputLayoutDesc), blob->GetBufferPointer(), blob->GetBufferSize(), &mVertexInputLayout));

	HR(D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&mVSReflector));

	// 
	// 创建像素着色器
	//

	HR(CreateShaderFromFile(L"HLSL\\BasicPS.cso", L"HLSL\\BasicPS.hlsl", "PS", "ps_5_0", &blob));
	HR(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &mPS));

	HR(D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&mPSReflector));

	//
	// 初始化常量缓冲区 
	//
	Reflect(device);

	return true;
}

void BasicEffect::ResetRenderStates(ID3D11DeviceContext* deviceContext)
{
	// 设置定点布局和图元拓扑
	deviceContext->IASetInputLayout(mVertexInputLayout);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 设置着色器
	deviceContext->VSSetShader(mVS, nullptr, 0);
	deviceContext->PSSetShader(mPS, nullptr, 0);
	deviceContext->GSSetShader(nullptr, nullptr, 0);

	// 设置默认状态
	deviceContext->PSSetSamplers(0, 1, &RenderStates::AnistropicWrapSS);
	deviceContext->RSSetState(nullptr);
	deviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
	deviceContext->OMSetDepthStencilState(nullptr, 0);
}

// In HLSL, matrix declarations default to column-major ordering
void XM_CALLCONV BasicEffect::SetWorldMatrix(DirectX::FXMMATRIX W)
{
	XMMATRIX world = XMMatrixTranspose(W);
	XMMATRIX worldInvTranspose = XMMatrixInverse(nullptr, W);
	SetLocalBufferVS("gWorld", (void*)&world);
	SetLocalBufferVS("gWorldInvTranspose", (void*)&worldInvTranspose);
}

void XM_CALLCONV BasicEffect::SetViewMatrix(DirectX::FXMMATRIX V)
{
	XMMATRIX view = XMMatrixTranspose(V);
	SetLocalBufferVS("gView", (void*)&view);
	SetLocalBufferVS("gView", (void*)&view);
}

void XM_CALLCONV BasicEffect::SetProjMatrix(DirectX::FXMMATRIX P)
{
	XMMATRIX proj = XMMatrixTranspose(P);
	SetLocalBufferVS("gProj", (void*)&proj);
}

void XM_CALLCONV BasicEffect::SetTexTransformMatrix(DirectX::FXMMATRIX T)
{
	XMMATRIX transform = XMMatrixTranspose(T);
	SetLocalBufferVS("gTexTransform", (void*)&transform);
}

void BasicEffect::SetDirLights(const DirectionalLight* dirLights)
{
	SetLocalBufferPS("gDirLights", (void*)dirLights);
}

void BasicEffect::SetMaterial(const Material& material)
{
	SetLocalBufferPS("gMaterial", (void*)&material);
}

void BasicEffect::SetTextureUsed(int isUsed)
{
	SetLocalBufferPS("gUseTexture", (void*)&isUsed);
}

void BasicEffect::SetTextureDiffuse(ID3D11ShaderResourceView* textureDiffuse)
{
	mTextureDiffuse = textureDiffuse;
}

void XM_CALLCONV BasicEffect::SetEyePos(DirectX::FXMVECTOR eyePos)
{
	SetLocalBufferPS("gEyePosW", (void*)&eyePos);
}

void XM_CALLCONV BasicEffect::SetColor(DirectX::FXMVECTOR color)
{
	SetLocalBufferPS("gColor", (void*)&color);
}

void BasicEffect::Apply(ID3D11DeviceContext* deviceContext)
{
	// 将常量缓冲区绑定到渲染管线上
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

	deviceContext->PSSetShaderResources(0, 1, &mTextureDiffuse);
}

