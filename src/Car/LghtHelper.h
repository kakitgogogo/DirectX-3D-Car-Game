#pragma once

#include <Windows.h>
#include <DirectXMath.h>

// ƽ�й�
struct DirectionalLight
{
	DirectX::XMFLOAT4 Ambient;   // ������
	DirectX::XMFLOAT4 Diffuse;   // �������
	DirectX::XMFLOAT4 Specular;  // �߹�

	// �����4D����: (Direction, Pad)
	DirectX::XMFLOAT3 Direction; // ��ķ���
	float Pad;                   // ���
};

// ���
struct PointLight
{
	DirectX::XMFLOAT4 Ambient;   // ������
	DirectX::XMFLOAT4 Diffuse;   // �������
	DirectX::XMFLOAT4 Specular;  // �߹�

	// �����4D����: (Position, Range)
	DirectX::XMFLOAT3 Position;  // ����λ��
	float Range;                 // ���ķ�Χ

	// �����4D����: (A0, A1, A2, Pad)
	DirectX::XMFLOAT3 Att;       // ˥������
	float Pad;                   // ���
};

// �۹��
struct SpotLight 
{
	DirectX::XMFLOAT4 Ambient;   // ������
	DirectX::XMFLOAT4 Diffuse;   // �������
	DirectX::XMFLOAT4 Specular;  // �߹�

	// �����4D����: (Position, Range)
	DirectX::XMFLOAT3 Position;  // �۹�Ƶ�λ��
	float Range;                 // �۹�Ƶķ�Χ

	// �����4D����: (Direction, Spot)
	DirectX::XMFLOAT3 Direction; // �۹�Ƶķ���
	float Spot;                  // ���ƾ۹��Բ׶�������С��ϵ��

	// �����4D����: (Att, Pad)
	DirectX::XMFLOAT3 Att;       // ˥������
	float Pad;                   // ���
};

struct Material
{
	DirectX::XMFLOAT4 Ambient;   // ��������
	DirectX::XMFLOAT4 Diffuse;   // ���������
	DirectX::XMFLOAT4 Specular;  // �߹���ʣ�wΪ�߹�ǿ��
	DirectX::XMFLOAT4 Reflect;   // �������
};