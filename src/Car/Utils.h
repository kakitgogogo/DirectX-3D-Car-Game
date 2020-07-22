#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <Windows.h>

// 调试报错函数
HRESULT WINAPI DXTraceW(_In_z_ const WCHAR* strFile, _In_ DWORD dwLine, _In_ HRESULT hr, _In_opt_ const WCHAR* strMsg, _In_ bool bPopMsgBox);

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)                                                   \
	{                                                           \
		HRESULT hr = (x);                                       \
		if(FAILED(hr))                                          \
		{                                                       \
			DXTraceW(__FILEW__, (DWORD)__LINE__, hr, L#x, true);\
		}                                                       \
	}
#endif

#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 

// 释放COM资源
#define ReleaseCOM(x) { if(x){ x->Release(); x = nullptr; } }

// 释放内存并使指针为空
#define SafeDelete(x) { delete x; x = nullptr; }

// 从.cso文件或.hlsl文件中读取着色器
HRESULT CreateShaderFromFile(const WCHAR* csoFileName, const WCHAR* hlslFileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** ppBlobOut);