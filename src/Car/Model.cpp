#include <d3d11.h>
#include <DirectXMath.h>
#include "Model.h"
#include "Utils.h"
#include "DDSTextureLoader.h"
#include "RenderStates.h"

using namespace DirectX;

Model::Model()
	: mDiffuseMapSRV(nullptr), mVertexBuffer(nullptr), mIndexBuffer(nullptr),
	mUseTexture(false), mColor(1.0f, 1.0f, 1.0f, 1.0f), 
	mWorldMatrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	),
	mTexTransform(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	)
{
	
}

Model::~Model()
{
	ReleaseCOM(mDiffuseMapSRV);
	ReleaseCOM(mVertexBuffer);
	ReleaseCOM(mIndexBuffer);
}

MeshData& Model::GetMeshData()
{
	return mMeshData;
}

void Model::SetMaterial(const XMFLOAT4& ambient, const XMFLOAT4& diffuse, const XMFLOAT4& specular, const XMFLOAT4& reflect)
{
	mMaterial.Ambient = ambient;
	mMaterial.Diffuse = diffuse;
	mMaterial.Specular = specular;
	mMaterial.Reflect = reflect;
}

void Model::SetTexture(ID3D11Device* device, const wchar_t* filename)
{
	HR(CreateDDSTextureFromFile(device, filename, nullptr, &mDiffuseMapSRV));
	mUseTexture = true;
}

void Model::SetBuffer(ID3D11Device* md3dDevice)
{
	mVertexCount = (UINT)mMeshData.Vertices.size();
	mIndexCount = (UINT)mMeshData.Indices.size();

	// 创建顶点缓冲区
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * mVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &mMeshData.Vertices[0];
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVertexBuffer));

	// 创建索引缓冲区
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * mIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &mMeshData.Indices[0];
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIndexBuffer));
}

void Model::SetColor(const DirectX::XMFLOAT4& color)
{
	mColor = color;
}

void XM_CALLCONV Model::SetColor(DirectX::FXMVECTOR color)
{
	XMStoreFloat4(&mColor, color);
}

void Model::SetWorldMatrix(const XMFLOAT4X4& world)
{
	mWorldMatrix = world;
}

void XM_CALLCONV Model::SetWorldMatrix(FXMMATRIX world)
{
	XMStoreFloat4x4(&mWorldMatrix, world);
}

void Model::SetTexTransformMatrix(const DirectX::XMFLOAT4X4& transform)
{
	mTexTransform = transform;
}

void XM_CALLCONV Model::SetTexTransformMatrix(DirectX::FXMMATRIX transform)
{
	XMStoreFloat4x4(&mTexTransform, transform);
}

void Model::Draw(ID3D11DeviceContext* deviceContext, BasicEffect* effect)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// 将顶点缓冲区绑定到渲染管线上
	deviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	effect->SetWorldMatrix(XMLoadFloat4x4(&mWorldMatrix));

	effect->SetTexTransformMatrix(XMLoadFloat4x4(&mTexTransform));

	if (mUseTexture)
	{
		effect->SetTextureDiffuse(mDiffuseMapSRV);
		effect->SetTextureUsed(true);
	}
	else
	{
		effect->SetColor(XMLoadFloat4(&mColor));
		effect->SetTextureUsed(false);
	}

	effect->SetMaterial(mMaterial);

	effect->Apply(deviceContext);

	// 使用索引绘制模型
	deviceContext->DrawIndexed(mIndexCount, 0, 0);
}

void Model::DrawShadow(ID3D11DeviceContext* deviceContext, BasicEffect* effect, XMFLOAT3* direction)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 获取阴影投射变换矩阵
	XMVECTOR shadowPlane = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // xz 平面
	XMVECTOR toMainLight = -XMLoadFloat3(direction);
	XMMATRIX S = XMMatrixShadow(shadowPlane, toMainLight);
	XMMATRIX shadowOffsetY = XMMatrixTranslation(0.0f, 0.001f, 0.0f);

	effect->SetWorldMatrix(XMLoadFloat4x4(&mWorldMatrix) * S * shadowOffsetY);

	// 阴影颜色设为灰色，透明度50%
	XMVECTOR color =  XMVectorSetW(Colors::Gray, 0.5f);
	effect->SetColor(color);

	// 防止double blend；使用透明混合
	deviceContext->OMSetDepthStencilState(RenderStates::NoDoubleBlendDSS, 0);
	deviceContext->OMSetBlendState(RenderStates::TransparentBS, nullptr, 0xFFFFFFFF);

	effect->Apply(deviceContext);
	deviceContext->DrawIndexed(mIndexCount, 0, 0);

	// 恢复默认状态
	deviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
	deviceContext->OMSetDepthStencilState(nullptr, 0);
}

