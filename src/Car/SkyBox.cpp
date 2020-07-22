#include "SkyBox.h"
#include "Utils.h"
#include "DDSTextureLoader.h"
#include "Geometry.h"

using namespace DirectX;

void SkyBox::Init(ID3D11Device* device, const wchar_t* filename, float skySphereRadiu)
{
	// ������պе���������ͼ
	HR(CreateDDSTextureFromFile(device, filename, nullptr, &mCubeMapSRV));

	// ���������
	MeshData sphere;
	CreateSphere(skySphereRadiu, 30, 30, sphere);

	std::vector<XMFLOAT3> vertices(sphere.Vertices.size());
	for (UINT i = 0; i < sphere.Vertices.size(); ++i)
	{
		vertices[i] = sphere.Vertices[i].Position;
	}

	mVertexCount = (UINT)vertices.size();
	mIndexCount = (UINT)sphere.Indices.size();

	// �������㻺����
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(XMFLOAT3) * mVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(device->CreateBuffer(&vbd, &vinitData, &mVertexBuffer));

	// ��������������
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(USHORT) * mIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.StructureByteStride = 0;
	ibd.MiscFlags = 0;
	std::vector<USHORT> indices16;
	indices16.assign(sphere.Indices.begin(), sphere.Indices.end());
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices16[0];
	HR(device->CreateBuffer(&ibd, &iinitData, &mIndexBuffer));
}

void SkyBox::Draw(ID3D11DeviceContext* deviceContext, SkyEffect* effect, ICamera* camera)
{
	UINT stride = sizeof(XMFLOAT3);
	UINT offset = 0;

	// �����㻺�����󶨵���Ⱦ������
	deviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// ��պ���������ƶ�
	XMFLOAT3 pos = camera->GetPosition();
	effect->SetWorldViewProjMatrix(XMMatrixTranslation(pos.x, pos.y, pos.z) * camera->GetViewProj());

	effect->SetTextureCube(mCubeMapSRV);

	effect->Apply(deviceContext);

	// ʹ�������������
	deviceContext->DrawIndexed(mIndexCount, 0, 0);
}