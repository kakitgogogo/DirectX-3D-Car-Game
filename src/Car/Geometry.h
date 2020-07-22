#pragma once

#include "Vertex.h"
#include <vector>

// ��������
void CreateGrid(float width, float depth, UINT m, UINT n, MeshData& meshData);

// ����ƽ��
void CreatePlane(float width, float depth, MeshData& meshData);

// ���ɳ�����
void CreateCube(float width, float height, float depth, MeshData& meshData);

// ��������
void CreateSphere(float radius, UINT stackCount, UINT sliceCount, MeshData& meshData);

// ����Բ������
void CreateCylinder(float topRadius, float bottomRadius, float height, UINT stackCount, UINT sliceCount, MeshData& meshData);

// ����Բ���嶥��
void CreateCylinderTopCap(float topRadius, float height, UINT sliceCount, MeshData& meshData);

// ����Բ����ײ�
void CreateCylinderBottomCap(float bottomRadius, float height, UINT sliceCount, MeshData& meshData); 
