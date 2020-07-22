#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3d11shader.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "LghtHelper.h"

// 着色器的常量缓冲区的信息
struct ShaderCBuffer
{
	ID3D11Buffer* CBuffer;   // 常量缓冲区
	void* LocalBuffer;       // 本地缓冲区
	UINT Size;               // 常量缓冲区的大小
	bool IsDirty;            // 常量缓冲区是否被修改
};

// 着色器常量缓冲区的变量的信息
struct ShaderVariable
{
	UINT CBufferSlot;        // 变量所在的常量缓冲区的槽位
	UINT StartOff;           // 变量在其常量缓冲区的偏移量
	UINT Size;               // 变量的大小
};

// Effect接口
class IEffect
{
public:
	virtual ~IEffect();

	// 初始化各种资源
	virtual bool InitAll(ID3D11Device* device) = 0;

	// 重置为默认的渲染状态
	virtual void ResetRenderStates(ID3D11DeviceContext* deviceContext) = 0;

	// 使常量缓冲区和其他资源的更新生效
	virtual void Apply(ID3D11DeviceContext* deviceContext) = 0;

	// 获取着色器常量缓冲区的信息
	void Reflect(ID3D11Device* device);

	// 更新本地缓冲区
	void SetLocalBufferVS(LPCSTR name, void* data);
	void SetLocalBufferPS(LPCSTR name, void* data);

	// 更新着色器的常量缓冲区
	void UpdateBufferVS(ID3D11DeviceContext* deviceContext, UINT slot);
	void UpdateBufferPS(ID3D11DeviceContext* deviceContext, UINT slot);

protected:
	ID3D11VertexShader* mVS;                 // 顶点着色器
	ID3D11ShaderReflection* mVSReflector;    // 顶点着色器的反射

	ID3D11PixelShader* mPS;                  // 像素着色器
	ID3D11ShaderReflection* mPSReflector;    // 像素着色器的反射

	ID3D11InputLayout* mVertexInputLayout;   // 顶点布局

	std::vector<ShaderCBuffer> mCBuffersVS;                         // 顶点着色器的常量缓冲区信息
	std::unordered_map<std::string, ShaderVariable> mVariablesVS;   // 顶点着色器的变量信息

	std::vector<ShaderCBuffer> mCBuffersPS;                         // 像素着色器的常量缓冲区信息
	std::unordered_map<std::string, ShaderVariable> mVariablesPS;   // 像素着色器的变量信息
};


class BasicEffect : IEffect
{
public:
	~BasicEffect();

	bool InitAll(ID3D11Device* device);

	void ResetRenderStates(ID3D11DeviceContext* deviceContext);

	// 用于更新变换矩阵
	void XM_CALLCONV SetWorldMatrix(DirectX::FXMMATRIX W);
	void XM_CALLCONV SetViewMatrix(DirectX::FXMMATRIX V);
	void XM_CALLCONV SetProjMatrix(DirectX::FXMMATRIX P);

	// 用于更新材质变换矩阵
	void XM_CALLCONV SetTexTransformMatrix(DirectX::FXMMATRIX T);

	// 用于更新灯光
	void SetDirLights(const DirectionalLight* dirLights);

	// 用于更新材质
	void SetMaterial(const Material& material);

	// 用于更新纹理
	void SetTextureUsed(int isUsed);
	void SetTextureDiffuse(ID3D11ShaderResourceView* textureDiffuse);

	// 用于更新摄像机位置
	void XM_CALLCONV SetEyePos(DirectX::FXMVECTOR eyePos);

	// 用于更新颜色
	void XM_CALLCONV SetColor(DirectX::FXMVECTOR color);

	// 使常量缓冲区和其他资源的更新生效
	void Apply(ID3D11DeviceContext* deviceContext);

private:
	ID3D11ShaderResourceView* mTextureDiffuse;  // 物体的贴图视图
};

class SkyEffect : IEffect
{
public:
	~SkyEffect();

	bool InitAll(ID3D11Device* device);

	void ResetRenderStates(ID3D11DeviceContext* deviceContext);

	// 用于更新变换矩阵
	void XM_CALLCONV SetWorldViewProjMatrix(DirectX::FXMMATRIX WVP);

	// 用于更新天空盒纹理
	void SetTextureCube(ID3D11ShaderResourceView* textureCube);

	// 使常量缓冲区和其他资源的更新生效
	void Apply(ID3D11DeviceContext* deviceContext);

private:
	ID3D11ShaderResourceView* mTextureCube;  // 天空盒的贴图视图
};
