#pragma once

#include <d3d11.h>
#include "LghtHelper.h"
#include "Vertex.h"
#include "Effects.h"

// ��Ϸģ��
class Model
{
public:
	Model();
	~Model();

	// ����������ݵ�����
	MeshData& GetMeshData();

	// ����ģ�͵Ĳ���
	void SetMaterial(const DirectX::XMFLOAT4& ambient, const DirectX::XMFLOAT4& diffuse, const DirectX::XMFLOAT4& specular, const DirectX::XMFLOAT4& reflect);

	// ����ģ�͵�������ͼ
	void SetTexture(ID3D11Device* device, const wchar_t* filename);

	// ��ʼ�����㻺�����������
	void SetBuffer(ID3D11Device* md3dDevice);

	// ����ģ�͵���ɫ����ʹ��������ͼ
	void SetColor(const DirectX::XMFLOAT4& color);
	void XM_CALLCONV SetColor(DirectX::FXMVECTOR color);

	// ����ģ�͵��������
	void SetWorldMatrix(const DirectX::XMFLOAT4X4& world);
	void XM_CALLCONV SetWorldMatrix(DirectX::FXMMATRIX world);

	// ����ģ�͵�����任����
	void SetTexTransformMatrix(const DirectX::XMFLOAT4X4& transform);
	void XM_CALLCONV SetTexTransformMatrix(DirectX::FXMMATRIX transform);

	// ����ģ��
	void Draw(ID3D11DeviceContext* deviceContext, BasicEffect* effect);

	// ����ģ�͵���Ӱ��xzƽ���ϣ�
	void DrawShadow(ID3D11DeviceContext* deviceContext, BasicEffect* effect, DirectX::XMFLOAT3* direction);

private:
	MeshData mMeshData;          // ģ�͵���������

	ID3D11Buffer* mVertexBuffer; // ���㻺��
	ID3D11Buffer* mIndexBuffer;  // ��������

	UINT mVertexCount;           // ģ�͵Ķ����� 
	UINT mIndexCount;            // ģ�͵������� 

	Material mMaterial;          // ģ�͵Ĳ���
	DirectX::XMFLOAT4 mColor;    // ģ�͵���ɫ

	bool mUseTexture;            // �Ƿ�ʹ������
	ID3D11ShaderResourceView* mDiffuseMapSRV;  // ������ͼ

	DirectX::XMFLOAT4X4 mWorldMatrix;          // �������
	DirectX::XMFLOAT4X4 mTexTransform;         // ����任����
};
