#pragma once

#include "Effects.h"
#include "Geometry.h"
#include "Camera.h"

// 天空盒类
class SkyBox
{
public:
	SkyBox() = default;
	~SkyBox() = default;

	// 初始化天空盒的立方体贴图、进行纹理映射的球体以及球体的顶点、索引缓冲区
	void Init(ID3D11Device* device, const wchar_t* filename, float skySphereRadius);

	// 绘制天空盒
	void Draw(ID3D11DeviceContext* deviceContext, SkyEffect* effect, ICamera* camera);

private:
	ID3D11ShaderResourceView* mCubeMapSRV;  // 天空盒的立方体贴图

	ID3D11Buffer* mVertexBuffer;            // 天空球的顶点缓冲区
	ID3D11Buffer* mIndexBuffer;             // 天空球的索引缓冲区

	UINT mVertexCount;                      // 天空球的顶点数
	UINT mIndexCount;                       // 天空球的索引数
};