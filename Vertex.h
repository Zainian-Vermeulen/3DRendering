#pragma once
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

struct Vertex
{
public:
	XMFLOAT3 position;
	XMFLOAT4 color;
	XMFLOAT3 normal;
	XMFLOAT2 uv;
	XMFLOAT3 t;

	struct MeshData
	{
		std::vector<Vertex> Vertices;
		std::vector<WORD> Indices;
	};
	
	Vertex(void) {}
	
	Vertex(FLOAT x, FLOAT y, FLOAT z) : position(x, y, z), color(1.0f,1.0f,1.0f,1.0f), normal (0.0f,0.0f,0.0f), uv(0.0f,0.0f){}

	Vertex(FLOAT x, FLOAT y, FLOAT z, XMFLOAT4 c) : position(x, y, z), color(c), normal(0.0f, 0.0f, 0.0f), uv(0.0f, 0.0f) {}

	Vertex(FLOAT x, FLOAT y, FLOAT z, FLOAT u, FLOAT v) : position(x, y, z), color(1.0f, 1.0f, 1.0f, 1.0f), normal(0.0f, 0.0f, 0.0f), uv(u, v) {}

	Vertex(FLOAT x, FLOAT y, FLOAT z, FLOAT u, FLOAT v, FLOAT nx, FLOAT ny, FLOAT nz) : position(x, y, z), color(1.0f, 1.0f, 1.0f, 1.0f), normal(nx, ny, nz), uv(u, v) {}

	Vertex(FLOAT x, FLOAT y, FLOAT z, FLOAT u, FLOAT v, FLOAT nx, FLOAT ny, FLOAT nz, FLOAT tx, FLOAT ty, FLOAT tz) : position(x, y, z), color(1.0f, 1.0f, 1.0f, 1.0f), normal(nx, ny, nz), uv(u, v), t (tx, ty, tz) {}

};