#include "Geometry.h"
#include <DirectXMath.h>

using namespace DirectX;

void CreateGrid(float width, float depth, UINT m, UINT n, MeshData& meshData)
{
	UINT vertexCount = m * n;
	UINT faceCount = (m - 1) * (n - 1) * 2;

	float halfWidth = 0.5f*width;
	float halfDepth = 0.5f*depth;

	float dx = width / (n - 1);
	float dz = depth / (m - 1);

	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);

	//
	// 生成顶点
	// 

	meshData.Vertices.resize(vertexCount);
	for (UINT i = 0; i < m; ++i) {
		float z = halfDepth - i * dz;
		for (UINT j = 0; j < n; ++j) {
			float x = -halfWidth + j * dx;

			meshData.Vertices[i*n + j].Position = XMFLOAT3(x, 0.0f, z);
			meshData.Vertices[i*n + j].Normal = XMFLOAT3(0.0, 1.0f, 0.0f);

			meshData.Vertices[i*n + j].TexCoord.x = j * du;
			meshData.Vertices[i*n + j].TexCoord.y = i * dv;
		}
	}

	//
	// 生成索引
	// 

	meshData.Indices.resize(faceCount * 3);
	UINT k = 0;
	for (UINT i = 0; i < m - 1; ++i) {
		for (UINT j = 0; j < n - 1; ++j) {
			meshData.Indices[k] = i * n + j;
			meshData.Indices[k + 1] = i * n + (j + 1);
			meshData.Indices[k + 2] = (i + 1) * n + j;

			meshData.Indices[k + 3] = (i + 1) * n + j;
			meshData.Indices[k + 4] = i * n + (j + 1);
			meshData.Indices[k + 5] = (i + 1) * n + (j + 1);

			k += 6;
		}
	}
}

void CreatePlane(float width, float depth, MeshData& meshData)
{
	float x = 0.5f * width;
	float z = 0.5f * depth;

	meshData.Vertices.resize(4);
	meshData.Vertices[0] = Vertex(-x, 0.0f, -z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	meshData.Vertices[1] = Vertex(-x, 0.0f, z, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	meshData.Vertices[2] = Vertex(x, 0.0f, z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	meshData.Vertices[3] = Vertex(x, 0.0f, -z, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	meshData.Indices.resize(6);
	meshData.Indices = { 0, 1, 2, 2, 3, 0 };
}

void CreateCube(float width, float height, float depth, MeshData& meshData)
{
	meshData.Vertices.resize(24);

	float x = 0.5f * width;
	float y = 0.5f * height;
	float z = 0.5f * depth;

	//
	// 生成顶点
	// 

	// 前面（-Z）
	meshData.Vertices[0] = Vertex(-x, -y, -z, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	meshData.Vertices[1] = Vertex(-x, +y, -z, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	meshData.Vertices[2] = Vertex(+x, +y, -z, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	meshData.Vertices[3] = Vertex(+x, -y, -z, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// 后面（+Z）
	meshData.Vertices[4] = Vertex(-x, -y, +z, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	meshData.Vertices[5] = Vertex(+x, -y, +z, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	meshData.Vertices[6] = Vertex(+x, +y, +z, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	meshData.Vertices[7] = Vertex(-x, +y, +z, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);

	// 上面（+Y）
	meshData.Vertices[8] = Vertex(-x, +y, -z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	meshData.Vertices[9] = Vertex(-x, +y, +z, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	meshData.Vertices[10] = Vertex(+x, +y, +z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	meshData.Vertices[11] = Vertex(+x, +y, -z, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	// 下面（-Y）
	meshData.Vertices[12] = Vertex(-x, -y, -z, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
	meshData.Vertices[13] = Vertex(+x, -y, -z, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
	meshData.Vertices[14] = Vertex(+x, -y, +z, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	meshData.Vertices[15] = Vertex(-x, -y, +z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

	// 左面（-X）
	meshData.Vertices[16] = Vertex(-x, -y, +z, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	meshData.Vertices[17] = Vertex(-x, +y, +z, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	meshData.Vertices[18] = Vertex(-x, +y, -z, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	meshData.Vertices[19] = Vertex(-x, -y, -z, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// 右面（+X）
	meshData.Vertices[20] = Vertex(+x, -y, -z, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	meshData.Vertices[21] = Vertex(+x, +y, -z, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	meshData.Vertices[22] = Vertex(+x, +y, +z, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	meshData.Vertices[23] = Vertex(+x, -y, +z, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	//
	// 生成索引
	// 

	meshData.Indices.resize(36);
	meshData.Indices = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4,
		8, 9, 10, 10, 11, 8,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20
	};
}

void CreateSphere(float radius, UINT stackCount, UINT sliceCount, MeshData& meshData)
{
	float phiStep = XM_PI / stackCount;
	float thetaStep = XM_2PI / sliceCount;

	//
	// 生成顶点
	// 

	Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);

	// 从顶部，一圈一圈地生成顶点
	meshData.Vertices.push_back(topVertex);
	for (UINT i = 1; i <= stackCount - 1; ++i)
	{
		float phi = i * phiStep;
		for (UINT j = 0; j <= sliceCount; ++j)
		{
			float theta = j * thetaStep;

			Vertex vertex;

			vertex.Position = XMFLOAT3( radius * sinf(phi) * cosf(theta), radius * cosf(phi), radius * sinf(phi) * sinf(theta) );

			XMVECTOR normal = XMLoadFloat3(&vertex.Position);
			XMStoreFloat3(&vertex.Normal, XMVector3Normalize(normal));

			vertex.TexCoord = XMFLOAT2(theta / XM_2PI, phi / XM_2PI );

			meshData.Vertices.push_back(vertex);
		}
	}
	meshData.Vertices.push_back(bottomVertex);

	//
	// 生成索引
	// 

	// 顶点与其下一层的连接
	for (UINT i = 1; i <= sliceCount; ++i)
	{
		meshData.Indices.push_back(0);
		meshData.Indices.push_back(i + 1);
		meshData.Indices.push_back(i);
	}

	// 除顶点和底点外每一层的连接
	UINT baseIndex = 1;
	UINT ringVertexCount = sliceCount + 1;
	for (UINT i = 0; i < stackCount - 2; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			meshData.Indices.push_back(baseIndex + i * ringVertexCount + j);
			meshData.Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			meshData.Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);

			meshData.Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
			meshData.Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			meshData.Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
		}
	}

	// 底点与其上一层的连接
	UINT bottomIndex = (UINT)meshData.Vertices.size() - 1;
	baseIndex = bottomIndex - ringVertexCount;
	for (UINT i = 0; i < sliceCount; ++i)
	{
		meshData.Indices.push_back(bottomIndex);
		meshData.Indices.push_back(baseIndex + i);
		meshData.Indices.push_back(baseIndex + i + 1);
	}
}

void CreateCylinder(float topRadius, float bottomRadius, float height, UINT stackCount, UINT sliceCount, MeshData& meshData)
{
	float stackHeight = height / stackCount;

	float dRadius = bottomRadius - topRadius;
	float radiusStep = dRadius / stackCount;

	//
	// 生成顶点
	// 

	// 从底部往顶部，一圈一圈地生成顶点
	for (UINT i = 0; i < stackCount + 1; ++i)
	{
		float h = -0.5f * height + i * stackHeight;
		float r = bottomRadius + i * radiusStep;

		float dTheta = XM_2PI / sliceCount;
		for (UINT j = 0; j <= sliceCount; ++j)
		{
			Vertex vertex;

			float theta = j * dTheta;
			float c = cosf(theta);
			float s = sinf(theta);

			vertex.Position = XMFLOAT3(r*c, h, r*s);
			
			XMVECTOR tengent = XMVectorSet(-s, 0.0f, c, 0.0f);
			XMVECTOR bitengent = XMVectorSet(dRadius*c, -height, dRadius*s, 0.0f);
			XMVECTOR normal = XMVector3Normalize(XMVector3Cross(tengent, bitengent));
			XMStoreFloat3(&vertex.Normal, normal);

			vertex.TexCoord = XMFLOAT2((float)j / sliceCount, 1.0f - (float)i / stackCount);

			meshData.Vertices.push_back(vertex);
		}
	}

	//
	// 生成索引
	// 

	UINT ringVertexCount = sliceCount + 1;
	for (UINT i = 0; i < stackCount; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			meshData.Indices.push_back(i*ringVertexCount + j);
			meshData.Indices.push_back((i + 1)*ringVertexCount + j);
			meshData.Indices.push_back((i + 1)*ringVertexCount + j + 1);

			meshData.Indices.push_back(i*ringVertexCount + j);
			meshData.Indices.push_back((i + 1)*ringVertexCount + j + 1);
			meshData.Indices.push_back(i*ringVertexCount + j + 1);
		}
	}

	CreateCylinderTopCap(topRadius, height, sliceCount, meshData);
	CreateCylinderBottomCap(bottomRadius, height, sliceCount, meshData);
}

void CreateCylinderTopCap(float topRadius, float height, UINT sliceCount, MeshData& meshData)
{
	UINT baseIndex = (UINT)meshData.Vertices.size();
	float y = 0.5f * height;
	float dTheta = XM_2PI / sliceCount;

	//
	// 生成顶点
	// 

	for (UINT i = 0; i <= sliceCount; ++i)
	{
		float x = topRadius * cosf(i*dTheta);
		float z = topRadius * sinf(i*dTheta);

		float u = 0.5f + cosf(i*dTheta) / 2;
		float v = 0.5f + sinf(i*dTheta) / 2;

		meshData.Vertices.push_back(Vertex(x, y, z, 0.0f, 1.0f, 0.0f, u, v));
	}
	// 圆心
	meshData.Vertices.push_back(Vertex(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f));

	//
	// 生成索引
	// 

	UINT centerIndex = (UINT)meshData.Vertices.size() - 1;
	for (UINT i = 0; i < sliceCount; ++i)
	{
		meshData.Indices.push_back(centerIndex);
		meshData.Indices.push_back(baseIndex + i + 1);
		meshData.Indices.push_back(baseIndex + i);
	}
}

void CreateCylinderBottomCap(float bottomRadius, float height, UINT sliceCount, MeshData& meshData)
{
	UINT baseIndex = (UINT)meshData.Vertices.size();
	float y = -0.5f * height;
	float dTheta = XM_2PI / sliceCount;

	//
	// 生成顶点
	// 

	for (UINT i = 0; i <= sliceCount; ++i)
	{
		float x = bottomRadius * cosf(i*dTheta);
		float z = bottomRadius * sinf(i*dTheta);

		float u = 0.5f + cosf(i*dTheta) / 2;
		float v = 0.5f + sinf(i*dTheta) / 2;

		meshData.Vertices.push_back(Vertex(x, y, z, 0.0f, -1.0f, 0.0f, u, v));
	}
	// 圆心
	meshData.Vertices.push_back(Vertex(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 0.5f, 0.5f));

	//
	// 生成索引
	// 

	UINT centerIndex = (UINT)meshData.Vertices.size() - 1;
	for (UINT i = 0; i < sliceCount; ++i)
	{
		meshData.Indices.push_back(centerIndex);
		meshData.Indices.push_back(baseIndex + i);
		meshData.Indices.push_back(baseIndex + i + 1);
	}
}