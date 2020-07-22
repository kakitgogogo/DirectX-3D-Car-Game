#pragma once

#include <d3d11.h>
#include "LghtHelper.h"
#include "Vertex.h"
#include "Effects.h"

// 游戏模型
class Model
{
public:
	Model();
	~Model();

	// 获得网格数据的引用
	MeshData& GetMeshData();

	// 设置模型的材质
	void SetMaterial(const DirectX::XMFLOAT4& ambient, const DirectX::XMFLOAT4& diffuse, const DirectX::XMFLOAT4& specular, const DirectX::XMFLOAT4& reflect);

	// 设置模型的纹理贴图
	void SetTexture(ID3D11Device* device, const wchar_t* filename);

	// 初始化顶点缓冲和索引缓冲
	void SetBuffer(ID3D11Device* md3dDevice);

	// 设置模型的颜色，不使用纹理贴图
	void SetColor(const DirectX::XMFLOAT4& color);
	void XM_CALLCONV SetColor(DirectX::FXMVECTOR color);

	// 设置模型的世界矩阵
	void SetWorldMatrix(const DirectX::XMFLOAT4X4& world);
	void XM_CALLCONV SetWorldMatrix(DirectX::FXMMATRIX world);

	// 设置模型的纹理变换矩阵
	void SetTexTransformMatrix(const DirectX::XMFLOAT4X4& transform);
	void XM_CALLCONV SetTexTransformMatrix(DirectX::FXMMATRIX transform);

	// 绘制模型
	void Draw(ID3D11DeviceContext* deviceContext, BasicEffect* effect);

	// 绘制模型的阴影（xz平面上）
	void DrawShadow(ID3D11DeviceContext* deviceContext, BasicEffect* effect, DirectX::XMFLOAT3* direction);

private:
	MeshData mMeshData;          // 模型的网格数据

	ID3D11Buffer* mVertexBuffer; // 顶点缓冲
	ID3D11Buffer* mIndexBuffer;  // 索引缓冲

	UINT mVertexCount;           // 模型的顶点数 
	UINT mIndexCount;            // 模型的索引数 

	Material mMaterial;          // 模型的材质
	DirectX::XMFLOAT4 mColor;    // 模型的颜色

	bool mUseTexture;            // 是否使用纹理
	ID3D11ShaderResourceView* mDiffuseMapSRV;  // 纹理视图

	DirectX::XMFLOAT4X4 mWorldMatrix;          // 世界矩阵
	DirectX::XMFLOAT4X4 mTexTransform;         // 纹理变换矩阵
};
