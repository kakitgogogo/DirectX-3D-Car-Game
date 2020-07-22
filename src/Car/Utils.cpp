#include "Utils.h"
#include <cstdio>
#include <d3dcompiler.h>

#define BUFFER_SIZE 3000

// 参考dxerr.cpp
HRESULT WINAPI DXTraceW(_In_z_ const WCHAR* strFile, _In_ DWORD dwLine, _In_ HRESULT hr,
	_In_opt_ const WCHAR* strMsg, _In_ bool bPopMsgBox)
{
	WCHAR strBufferLine[128];
	WCHAR strBufferError[256];
	WCHAR strBuffer[BUFFER_SIZE];

	swprintf_s(strBufferLine, 128, L"%lu", dwLine);
	if (strFile)
	{
		swprintf_s(strBuffer, BUFFER_SIZE, L"%ls(%ls): ", strFile, strBufferLine);
		OutputDebugStringW(strBuffer);
	}

	size_t nMsgLen = (strMsg) ? wcsnlen_s(strMsg, 1024) : 0;
	if (nMsgLen > 0)
	{
		OutputDebugStringW(strMsg);
		OutputDebugStringW(L" ");
	}

	// 使用FormatMessageW获取错误信息
	FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hr, MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), strBufferError, 256, nullptr);
	swprintf_s(strBuffer, BUFFER_SIZE, L"hr=%ls (0x%0.8x)", strBufferError, hr);
	OutputDebugStringW(strBuffer);

	OutputDebugStringW(L"\n");

#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
	if (bPopMsgBox)
	{
		WCHAR strBufferFile[MAX_PATH];
		wcscpy_s(strBufferFile, MAX_PATH, L"");
		if (strFile)
			wcscpy_s(strBufferFile, MAX_PATH, strFile);

		WCHAR strBufferMsg[1024];
		wcscpy_s(strBufferMsg, 1024, L"");
		if (nMsgLen > 0)
			swprintf_s(strBufferMsg, 1024, L"Calling: %ls\n", strMsg);

		swprintf_s(strBuffer, BUFFER_SIZE, L"File: %ls\nLine: %ls\nError Code: %ls (0x%0.8x)\n%lsDo you want to debug the application?",
			strBufferFile, strBufferLine, strBufferError, hr, strBufferMsg);

		int nResult = MessageBoxW(GetForegroundWindow(), strBuffer, L"Unexpected error encountered", MB_YESNO | MB_ICONERROR);
		if (nResult == IDYES)
			DebugBreak();
	}
#else
	UNREFERENCED_PARAMETER(bPopMsgBox);
#endif

	return hr;
}

HRESULT CreateShaderFromFile(const WCHAR* csoFileName, const WCHAR* hlslFileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;
	if (csoFileName && D3DReadFileToBlob(csoFileName, ppBlobOut) == S_OK)
	{
		return hr;
	}
	else
	{
		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
		dwShaderFlags |= D3DCOMPILE_DEBUG;
		dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
		ID3DBlob* errorBlob = nullptr;
		hr = D3DCompileFromFile(hlslFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, shaderModel, dwShaderFlags, 0, ppBlobOut, &errorBlob);
		if (FAILED(hr))
		{
			if (errorBlob != nullptr)
			{
				OutputDebugStringA(reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
			}
			ReleaseCOM(errorBlob);
			return hr;
		}
	}
	return hr;
}