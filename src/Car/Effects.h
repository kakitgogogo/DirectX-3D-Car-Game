#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3d11shader.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "LghtHelper.h"

// ��ɫ���ĳ�������������Ϣ
struct ShaderCBuffer
{
	ID3D11Buffer* CBuffer;   // ����������
	void* LocalBuffer;       // ���ػ�����
	UINT Size;               // �����������Ĵ�С
	bool IsDirty;            // �����������Ƿ��޸�
};

// ��ɫ�������������ı�������Ϣ
struct ShaderVariable
{
	UINT CBufferSlot;        // �������ڵĳ����������Ĳ�λ
	UINT StartOff;           // �������䳣����������ƫ����
	UINT Size;               // �����Ĵ�С
};

// Effect�ӿ�
class IEffect
{
public:
	virtual ~IEffect();

	// ��ʼ��������Դ
	virtual bool InitAll(ID3D11Device* device) = 0;

	// ����ΪĬ�ϵ���Ⱦ״̬
	virtual void ResetRenderStates(ID3D11DeviceContext* deviceContext) = 0;

	// ʹ������������������Դ�ĸ�����Ч
	virtual void Apply(ID3D11DeviceContext* deviceContext) = 0;

	// ��ȡ��ɫ����������������Ϣ
	void Reflect(ID3D11Device* device);

	// ���±��ػ�����
	void SetLocalBufferVS(LPCSTR name, void* data);
	void SetLocalBufferPS(LPCSTR name, void* data);

	// ������ɫ���ĳ���������
	void UpdateBufferVS(ID3D11DeviceContext* deviceContext, UINT slot);
	void UpdateBufferPS(ID3D11DeviceContext* deviceContext, UINT slot);

protected:
	ID3D11VertexShader* mVS;                 // ������ɫ��
	ID3D11ShaderReflection* mVSReflector;    // ������ɫ���ķ���

	ID3D11PixelShader* mPS;                  // ������ɫ��
	ID3D11ShaderReflection* mPSReflector;    // ������ɫ���ķ���

	ID3D11InputLayout* mVertexInputLayout;   // ���㲼��

	std::vector<ShaderCBuffer> mCBuffersVS;                         // ������ɫ���ĳ�����������Ϣ
	std::unordered_map<std::string, ShaderVariable> mVariablesVS;   // ������ɫ���ı�����Ϣ

	std::vector<ShaderCBuffer> mCBuffersPS;                         // ������ɫ���ĳ�����������Ϣ
	std::unordered_map<std::string, ShaderVariable> mVariablesPS;   // ������ɫ���ı�����Ϣ
};


class BasicEffect : IEffect
{
public:
	~BasicEffect();

	bool InitAll(ID3D11Device* device);

	void ResetRenderStates(ID3D11DeviceContext* deviceContext);

	// ���ڸ��±任����
	void XM_CALLCONV SetWorldMatrix(DirectX::FXMMATRIX W);
	void XM_CALLCONV SetViewMatrix(DirectX::FXMMATRIX V);
	void XM_CALLCONV SetProjMatrix(DirectX::FXMMATRIX P);

	// ���ڸ��²��ʱ任����
	void XM_CALLCONV SetTexTransformMatrix(DirectX::FXMMATRIX T);

	// ���ڸ��µƹ�
	void SetDirLights(const DirectionalLight* dirLights);

	// ���ڸ��²���
	void SetMaterial(const Material& material);

	// ���ڸ�������
	void SetTextureUsed(int isUsed);
	void SetTextureDiffuse(ID3D11ShaderResourceView* textureDiffuse);

	// ���ڸ��������λ��
	void XM_CALLCONV SetEyePos(DirectX::FXMVECTOR eyePos);

	// ���ڸ�����ɫ
	void XM_CALLCONV SetColor(DirectX::FXMVECTOR color);

	// ʹ������������������Դ�ĸ�����Ч
	void Apply(ID3D11DeviceContext* deviceContext);

private:
	ID3D11ShaderResourceView* mTextureDiffuse;  // �������ͼ��ͼ
};

class SkyEffect : IEffect
{
public:
	~SkyEffect();

	bool InitAll(ID3D11Device* device);

	void ResetRenderStates(ID3D11DeviceContext* deviceContext);

	// ���ڸ��±任����
	void XM_CALLCONV SetWorldViewProjMatrix(DirectX::FXMMATRIX WVP);

	// ���ڸ�����պ�����
	void SetTextureCube(ID3D11ShaderResourceView* textureCube);

	// ʹ������������������Դ�ĸ�����Ч
	void Apply(ID3D11DeviceContext* deviceContext);

private:
	ID3D11ShaderResourceView* mTextureCube;  // ��պе���ͼ��ͼ
};
