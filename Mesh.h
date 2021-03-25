#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Vertex.h"

using namespace DirectX;

class Mesh
{
public:
	
	enum Object
	{
		cube,
		sphere,
		skybox
	};

	int init(ID3D11Device* pD3DDevice, Object obj);
	void render(ID3D11DeviceContext* pD3DDeviceContext);
	void deInit();

private:
	int createVertexBuffer(ID3D11Device* pD3DDevice, Vertex::MeshData& meshData);
	bool createBuffer(UINT byteWidth, Vertex* vertices, ID3D11Device* pD3DDevice);
	bool createBuffer(UINT byteWidth, WORD* vertices, ID3D11Device* pD3DDevice);
	int createIndexBuffer(ID3D11Device* pD3DDevice, Vertex::MeshData& meshData);

	ID3D11Buffer* _pVertexBuffer = nullptr;
	ID3D11Buffer* _pIndexBuffer = nullptr;

	Object _obj;
	
	UINT _vertexCount = 0; // amount of vertices
	UINT _vertexStride = 0; // size of one vertex instance in bytes
	UINT _indexCount = 0; // amount of indices;
};

