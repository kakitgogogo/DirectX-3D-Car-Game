#pragma once

#include "Utils.h"

// 一些渲染状态
class RenderStates
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	// Rasterizer states
	static ID3D11RasterizerState* NoCullRS;            // 禁用背面消隐

	// Sampler states
	static ID3D11SamplerState* LinearWrapSS;           // 线性过滤，重复寻址采样
	static ID3D11SamplerState* AnistropicWrapSS;       // 各向异性过滤，重复寻址采样

	// Blend states
	static ID3D11BlendState* TransparentBS;            // 透明混合

	// Depth/stencil states
	static ID3D11DepthStencilState* LessEqualDSS;      // 深度测试函数设为LESS_EQUAL，使z=1（NDC）处通过深度测试
	static ID3D11DepthStencilState* NoDoubleBlendDSS;  // 防止double blend
};