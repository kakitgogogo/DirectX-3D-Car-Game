#pragma once

#include "Utils.h"

// һЩ��Ⱦ״̬
class RenderStates
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	// Rasterizer states
	static ID3D11RasterizerState* NoCullRS;            // ���ñ�������

	// Sampler states
	static ID3D11SamplerState* LinearWrapSS;           // ���Թ��ˣ��ظ�Ѱַ����
	static ID3D11SamplerState* AnistropicWrapSS;       // �������Թ��ˣ��ظ�Ѱַ����

	// Blend states
	static ID3D11BlendState* TransparentBS;            // ͸�����

	// Depth/stencil states
	static ID3D11DepthStencilState* LessEqualDSS;      // ��Ȳ��Ժ�����ΪLESS_EQUAL��ʹz=1��NDC����ͨ����Ȳ���
	static ID3D11DepthStencilState* NoDoubleBlendDSS;  // ��ֹdouble blend
};