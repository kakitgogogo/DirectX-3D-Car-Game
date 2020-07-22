#include "Effects.h"
#include "Utils.h"

IEffect::~IEffect()
{
	ReleaseCOM(mVS);
	ReleaseCOM(mVSReflector);

	ReleaseCOM(mPS);
	ReleaseCOM(mPSReflector);

	ReleaseCOM(mVertexInputLayout);

	for (UINT i = 0; i < mCBuffersVS.size(); ++i)
	{
		ReleaseCOM(mCBuffersVS[i].CBuffer);
		free(mCBuffersVS[i].LocalBuffer);
	}
	for (UINT i = 0; i < mCBuffersPS.size(); ++i)
	{
		ReleaseCOM(mCBuffersPS[i].CBuffer);
		free(mCBuffersPS[i].LocalBuffer);
	}
	mVariablesVS.clear();
	mVariablesPS.clear();
}

void IEffect::Reflect(ID3D11Device* device)
{
	// 
	// 初始化顶点着色器的常量缓冲区
	//

	// 获取着色器的描述
	D3D11_SHADER_DESC VSDesc;
	mVSReflector->GetDesc(&VSDesc);
	mCBuffersVS.resize(VSDesc.ConstantBuffers);
	for (UINT i = 0; i < VSDesc.ConstantBuffers; ++i)
	{
		// 获取常量缓冲区的描述
		ID3D11ShaderReflectionConstantBuffer* pBuffer = mVSReflector->GetConstantBufferByIndex(i);
		D3D11_SHADER_BUFFER_DESC sbDesc;
		pBuffer->GetDesc(&sbDesc);

		D3D11_SHADER_INPUT_BIND_DESC sibDesc;
		mVSReflector->GetResourceBindingDescByName(sbDesc.Name, &sibDesc);
		UINT bindPoint = sibDesc.BindPoint;

		ID3D11Buffer* cbuffer;

		D3D11_BUFFER_DESC cbDesc;
		ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.ByteWidth = sbDesc.Size;
		HR(device->CreateBuffer(&cbDesc, nullptr, &cbuffer));

		// 根据常量缓冲区的大小创建本地缓冲区
		void* buffer = (void*)malloc(sbDesc.Size);
		mCBuffersVS[bindPoint] = { cbuffer, buffer, sbDesc.Size, false };

		UINT vars = sbDesc.Variables;
		for (UINT j = 0; j < sbDesc.Variables; ++j)
		{
			// 获取变量的描述
			ID3D11ShaderReflectionVariable* pVar = pBuffer->GetVariableByIndex(j);
			D3D11_SHADER_VARIABLE_DESC svDesc;
			pVar->GetDesc(&svDesc);
			mVariablesVS[svDesc.Name] = { bindPoint, svDesc.StartOffset, svDesc.Size };
		}
	}

	// 
	// 初始化像素着色器的常量缓冲区
	//
	D3D11_SHADER_DESC PSDesc;
	mPSReflector->GetDesc(&PSDesc);
	mCBuffersPS.resize(PSDesc.ConstantBuffers);
	for (UINT i = 0; i < PSDesc.ConstantBuffers; ++i)
	{
		ID3D11ShaderReflectionConstantBuffer* pBuffer = mPSReflector->GetConstantBufferByIndex(i);
		D3D11_SHADER_BUFFER_DESC sbDesc;
		pBuffer->GetDesc(&sbDesc);

		D3D11_SHADER_INPUT_BIND_DESC sibDesc;
		mPSReflector->GetResourceBindingDescByName(sbDesc.Name, &sibDesc);
		UINT bindPoint = sibDesc.BindPoint;

		ID3D11Buffer* cbuffer;

		D3D11_BUFFER_DESC cbDesc;
		ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.ByteWidth = sbDesc.Size;
		HR(device->CreateBuffer(&cbDesc, nullptr, &cbuffer));

		void* buffer = (void*)malloc(sbDesc.Size);
		mCBuffersPS[bindPoint] = { cbuffer, buffer, sbDesc.Size, false };

		UINT vars = sbDesc.Variables;
		for (UINT j = 0; j < sbDesc.Variables; ++j)
		{
			ID3D11ShaderReflectionVariable* pVar = pBuffer->GetVariableByIndex(j);
			D3D11_SHADER_VARIABLE_DESC svDesc;
			pVar->GetDesc(&svDesc);
			mVariablesPS[svDesc.Name] = { bindPoint, svDesc.StartOffset, svDesc.Size };
		}
	}
}

void IEffect::SetLocalBufferVS(LPCSTR name, void* data)
{
	ShaderVariable var = mVariablesVS[name];
	ShaderCBuffer& cbuffer = mCBuffersVS[var.CBufferSlot];
	char* pData = (char*)cbuffer.LocalBuffer;
	// 更新变量所在部分的本地缓冲区
	memcpy_s(pData + var.StartOff, var.Size, data, var.Size);

	cbuffer.IsDirty = true;
}

void IEffect::SetLocalBufferPS(LPCSTR name, void* data)
{
	ShaderVariable var = mVariablesPS[name];
	ShaderCBuffer& cbuffer = mCBuffersPS[var.CBufferSlot];
	char* pData = (char*)cbuffer.LocalBuffer;
	// 更新变量所在部分的本地缓冲区
	memcpy_s(pData + var.StartOff, var.Size, data, var.Size);

	cbuffer.IsDirty = true;
}

void IEffect::UpdateBufferVS(ID3D11DeviceContext* deviceContext, UINT slot)
{
	ShaderCBuffer& cbuffer = mCBuffersVS[slot];

	if (!cbuffer.IsDirty)
		return;

	// 写入GPU
	D3D11_MAPPED_SUBRESOURCE mappedData;
	deviceContext->Map(cbuffer.CBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	unsigned char* pData = (unsigned char*)mappedData.pData;
	memcpy_s(pData, cbuffer.Size, cbuffer.LocalBuffer, cbuffer.Size);
	deviceContext->Unmap(cbuffer.CBuffer, 0);

	cbuffer.IsDirty = false;
}

void IEffect::UpdateBufferPS(ID3D11DeviceContext* deviceContext, UINT slot)
{
	ShaderCBuffer& cbuffer = mCBuffersPS[slot];

	if (!cbuffer.IsDirty)
		return;

	// 写入GPU
	D3D11_MAPPED_SUBRESOURCE mappedData;
	deviceContext->Map(cbuffer.CBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	unsigned char* pData = (unsigned char*)mappedData.pData;
	memcpy_s(pData, cbuffer.Size, cbuffer.LocalBuffer, cbuffer.Size);
	deviceContext->Unmap(cbuffer.CBuffer, 0);

	cbuffer.IsDirty = false;
}