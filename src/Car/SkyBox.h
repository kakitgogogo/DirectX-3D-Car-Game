#pragma once

#include "Effects.h"
#include "Geometry.h"
#include "Camera.h"

// ��պ���
class SkyBox
{
public:
	SkyBox() = default;
	~SkyBox() = default;

	// ��ʼ����պе���������ͼ����������ӳ��������Լ�����Ķ��㡢����������
	void Init(ID3D11Device* device, const wchar_t* filename, float skySphereRadius);

	// ������պ�
	void Draw(ID3D11DeviceContext* deviceContext, SkyEffect* effect, ICamera* camera);

private:
	ID3D11ShaderResourceView* mCubeMapSRV;  // ��պе���������ͼ

	ID3D11Buffer* mVertexBuffer;            // �����Ķ��㻺����
	ID3D11Buffer* mIndexBuffer;             // ����������������

	UINT mVertexCount;                      // �����Ķ�����
	UINT mIndexCount;                       // ������������
};