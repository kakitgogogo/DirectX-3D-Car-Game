#pragma once

#include "Vertex.h"
#include <vector>

// 生成网格
void CreateGrid(float width, float depth, UINT m, UINT n, MeshData& meshData);

// 生成平面
void CreatePlane(float width, float depth, MeshData& meshData);

// 生成长方体
void CreateCube(float width, float height, float depth, MeshData& meshData);

// 生成球体
void CreateSphere(float radius, UINT stackCount, UINT sliceCount, MeshData& meshData);

// 生成圆柱体侧边
void CreateCylinder(float topRadius, float bottomRadius, float height, UINT stackCount, UINT sliceCount, MeshData& meshData);

// 生成圆柱体顶部
void CreateCylinderTopCap(float topRadius, float height, UINT sliceCount, MeshData& meshData);

// 生成圆柱体底部
void CreateCylinderBottomCap(float bottomRadius, float height, UINT sliceCount, MeshData& meshData); 
