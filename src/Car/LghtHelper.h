#pragma once

#include <Windows.h>
#include <DirectXMath.h>

// 平行光
struct DirectionalLight
{
	DirectX::XMFLOAT4 Ambient;   // 环境光
	DirectX::XMFLOAT4 Diffuse;   // 漫反射光
	DirectX::XMFLOAT4 Specular;  // 高光

	// 打包成4D向量: (Direction, Pad)
	DirectX::XMFLOAT3 Direction; // 光的方向
	float Pad;                   // 填充
};

// 点光
struct PointLight
{
	DirectX::XMFLOAT4 Ambient;   // 环境光
	DirectX::XMFLOAT4 Diffuse;   // 漫反射光
	DirectX::XMFLOAT4 Specular;  // 高光

	// 打包成4D向量: (Position, Range)
	DirectX::XMFLOAT3 Position;  // 点光的位置
	float Range;                 // 点光的范围

	// 打包成4D向量: (A0, A1, A2, Pad)
	DirectX::XMFLOAT3 Att;       // 衰减参数
	float Pad;                   // 填充
};

// 聚光灯
struct SpotLight 
{
	DirectX::XMFLOAT4 Ambient;   // 环境光
	DirectX::XMFLOAT4 Diffuse;   // 漫反射光
	DirectX::XMFLOAT4 Specular;  // 高光

	// 打包成4D向量: (Position, Range)
	DirectX::XMFLOAT3 Position;  // 聚光灯的位置
	float Range;                 // 聚光灯的范围

	// 打包成4D向量: (Direction, Spot)
	DirectX::XMFLOAT3 Direction; // 聚光灯的方向
	float Spot;                  // 控制聚光灯圆锥体区域大小的系数

	// 打包成4D向量: (Att, Pad)
	DirectX::XMFLOAT3 Att;       // 衰减参数
	float Pad;                   // 填充
};

struct Material
{
	DirectX::XMFLOAT4 Ambient;   // 环境材质
	DirectX::XMFLOAT4 Diffuse;   // 漫反射材质
	DirectX::XMFLOAT4 Specular;  // 高光材质，w为高光强度
	DirectX::XMFLOAT4 Reflect;   // 反射材质
};