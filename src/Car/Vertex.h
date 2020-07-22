#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

struct Vertex
{
	Vertex() : Position(0.0f, 0.0f, 0.0f), Normal(0.0f, 0.0f, 0.0f), TexCoord(0.0f, 0.0f) {}
	Vertex(float px, float py, float pz, float nx, float ny, float nz, float u, float v)
		: Position(px, py, pz), Normal(nx, ny, nz), TexCoord(u, v) {}

	DirectX::XMFLOAT3 Position; 
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 TexCoord;
	static const D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[3];
};

struct MeshData
{
	std::vector<Vertex> Vertices;
	std::vector<UINT> Indices;
};
